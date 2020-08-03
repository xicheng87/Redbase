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

int main(int argc, char* argv[]) {
  BitmapSetAndTestTest(20);
  BitmapSetAndTestTest(1000);
  BitmapSetAndTestTest(12345);


  return 0;
}
