//
// rm_bitmap.h
// 
// Record Manager's bitmap interface

#ifndef RM_BITMAP_H
#define RM_BITMAP_H

#include "rm.h"

// A bitmap used by the Record Management component to record the slots that are
// current used by a page
class RM_Bitmap {
 public:
  // Initialize a Bitmap for n slots and the bits information is stored in the 
  // given pointer. The bitmap does not make deep copy and directly manipulate
  // the bits (so that this information gets reflected in a page header). Thus,
  // it is the caller's responsbility to ensure that the given char has a valid
  // length of upper_bound(n / 8)
  RM_Bitmap(unsigned int n, char* bits); 
  
  // Set and unset a bit, possible error case RM_BITMAP_OUTOFRANGE
  RC Set(unsigned int idx);
  RC Unset(unsigned int idx);

  // Test a bit
  RC Test(unsigned int idx, bool& val);

  // Find the first slot that is unset, possible error case RM_BITMAP_FULL
  RC FindFirstUnset(unsigned int& idx);

 private:
  // The number of bits under management
  unsigned int len;
  // Point to the start of the char array that represents the bitmap
  unsigned char* data;
};

#endif
