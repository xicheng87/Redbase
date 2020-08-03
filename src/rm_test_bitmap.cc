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
void BitmapFindFirstUnsetTest(int len) {
  RC rc;
  int byteLen = len / 8 + (len % 8 != 0);
  char* data = (char*)malloc(byteLen * sizeof(char));
  memset(data, 0, byteLen);

  RM_Bitmap bitmap(len, data);

  // Find first unset bit and then set it sequentially
  for (int i = 0; i < len; i++) {
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
}

int main(int argc, char* argv[]) {
  BitmapSetAndTestTest(20);
  BitmapSetAndTestTest(1000);
  BitmapSetAndTestTest(12345);
  BitmapErrorTest();
  BitmapFindFirstUnsetTest(1234);
  BitmapFindFirstUnsetTest(4345);
  BitmapFindFirstUnsetTest(1 << 16);

  return 0;
}
