//
// File: rm_test_bitmap.cc
// Description: Test RM_Bitmap
// Author: Xi Cheng 
//
// This test verifies the behavior of the RM_Bitmap class
//
#include <assert.h>
#include <iostream>
#include <cstring>

#include "rm_bitmap.h"

using namespace std;

//
// PrintError
//
// Desc: Print an error message by calling the proper component-specific
//       print-error function
// TODO(Xi): Refactor this to a rm_test_util.h
void PrintError(RC rc)
{
    if (abs(rc) <= END_PF_WARN)
        PF_PrintError(rc);
    else if (abs(rc) <= END_RM_WARN)
        RM_PrintError(rc);
    else
        cerr << "Error code out of range: " << rc << "\n";
}

//
// BitmapSetAndTestTest
//
// Desc: Basic testing of bitmap's set, unset and test functionalities
void BitmapSetAndTestTest(int len) {
  RC rc;
  int byteLen = len / 8 + (len % 8 != 0);
  char* data = (char*)malloc(byteLen * sizeof(char));
  memset(data, 0, byteLen);

  RM_Bitmap bitmap(len, data);
  
  // Verify that all bits are unset at beginning
  for (int i = 0; i < len; i++) {
    bool val;
    if ((rc = bitmap.Test(i, val))) {
      PrintError(rc);
      exit(1);
    }
    assert(!val);
  }

  // Set and test all bits
  for (int i = 0; i < len; i++) {
    if ((rc = bitmap.Set(i))) {
      PrintError(rc);
      exit(1);
    }
  }

  for (int i = 0; i < len; i++) {
    bool val;
    if ((rc = bitmap.Test(i, val))) {
      PrintError(rc);
      exit(1);
    }
    assert(val);
  }

  // Unset and test all bits
  for (int i = 0; i < len; i++) {
    if ((rc = bitmap.Unset(i))) {
      PrintError(rc);
      exit(1);
    }
  }

  for (int i = 0; i < len; i++) {
    bool val;
    if ((rc = bitmap.Test(i, val))) {
      PrintError(rc);
      exit(1);
    }
    assert(!val);
  }

  free(data);
}

//
// BitmapErrorTest
//
// Desc: Test negative cases when using bitmap incorretly
void BitmapErrorTest() {
  RC rc;
  int len = 100;
  int byteLen = len / 8 + (len % 8 != 0);
  char* data = (char*)malloc(byteLen * sizeof(char));
  memset(data, 0, byteLen);

  RM_Bitmap bitmap(len, data);
  
  // Test the error case for RM_BITMAP_OUTOFRANGE
  rc = bitmap.Set(len + 100);
  assert(rc == RM_BITMAP_OUTOFRANGE);
  rc = bitmap.Unset(len + 1000);
  assert(rc == RM_BITMAP_OUTOFRANGE);
  bool val;
  rc = bitmap.Test(len + 1234, val);
  assert(rc == RM_BITMAP_OUTOFRANGE);
}

//
// BitmapFindFirstUnsetTest
//
// Desc: Test the use of FindFirstUnset 
void BitmapFindFirstUnsetTest(unsigned int len) {
  RC rc;
  unsigned int byteLen = len / 8 + (len % 8 != 0);
  char* data = (char*)malloc(byteLen * sizeof(char));
  memset(data, 0, byteLen);

  RM_Bitmap bitmap(len, data);

  // Find first unset bit and then set it sequentially
  for (unsigned int i = 0; i < len; i++) {
    unsigned int firstUnsetIdx;
    if ((rc = bitmap.FindFirstUnset(firstUnsetIdx))) {
      PrintError(rc);
      exit(1);
    }

    assert(firstUnsetIdx == (unsigned int)i);
    if ((rc = bitmap.Set(firstUnsetIdx))) {
      PrintError(rc);
      exit(1);
    }
  }

  unsigned int idx;
  assert(bitmap.FindFirstUnset(idx) == RM_BITMAP_FULL);

  // Unset all bits
  for (unsigned int i = 0; i < len; i++) {
    if ((rc = bitmap.Unset(i))) {
      PrintError(rc);
      exit(1);
    }
  }

  // Let's set every three-bit and then verify FindFirstUnset gives the correct
  // bit position when walking through the bits again
  for (unsigned int i = 0; i < len; i++) {
    if ((i % 3 == 0) && (rc = bitmap.Set(i))) {
      PrintError(rc);
      exit(1);
    }
  }

  for (unsigned int i = 0; i < len; i++) {
    unsigned int firstUnsetIdx;
    rc = bitmap.FindFirstUnset(firstUnsetIdx);
    if (rc) {
      if (rc == RM_BITMAP_FULL) {
        // If bitmap is full, this means the total length is a multiple of 3
        // and we are at the tail of the bitmap
        assert(i + 1 == len && i % 3 ==0);
      } else {
        PrintError(rc);
        exit(1);
      }
    }
    
    // If i is a multiple of 3 that means it has been taken so the next 
    // available bit is i+1, otherwise i should be available
    if (!rc) {
      assert(firstUnsetIdx == ((i % 3 == 0) ? i + 1 : i));
    }

    // Set the i-th bit sequentially
    if ((rc = bitmap.Set(i))) {
      PrintError(rc);
      exit(1);
    }
  }
}

int main(int argc, char* argv[]) {
  BitmapSetAndTestTest(20);
  BitmapSetAndTestTest(1000);
  BitmapSetAndTestTest(12345);
  BitmapErrorTest();
  BitmapFindFirstUnsetTest(1234);
  BitmapFindFirstUnsetTest(4345);
  BitmapFindFirstUnsetTest(1 << 16);
  // Test a case with len a multiple of 3
  BitmapFindFirstUnsetTest(9000);

  return 0;
}
