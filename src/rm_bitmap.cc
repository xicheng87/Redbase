//
// rm_bitmap.cc
//
// Implementation of Record Manager's bitmap

#include "rm_bitmap.h"

RM_Bitmap::RM_Bitmap(unsigned int n, char* bits) : len(n), data(bits) {}

RC RM_Bitmap::Set(unsigned int idx) {
  RC rc;
  if (idx >= len) {
    rc = RM_BITMAP_OUTOFRANGE;
    RM_PrintError(RM_BITMAP_OUTOFRANGE);
    return rc;
  }

  // The idx at the char array that we look for
  auto data_idx = idx / 8; 
  // The bit position at the char we look for
  auto bit_idx = idx % 8;
  // Set the corresponding bit 
  data[data_idx] |= (1 << bit_idx);
  return OK_RC;
}

RC RM_Bitmap::Unset(unsigned int idx) {
  RC rc;
  if (idx >= len) {
    rc = RM_BITMAP_OUTOFRANGE;
    RM_PrintError(RM_BITMAP_OUTOFRANGE);
    return rc;
  }

  // The idx at the char array that we look for
  auto data_idx = idx / 8; 
  // The bit position at the char we look for
  auto bit_idx = idx % 8;
  // Unset the corresponding bit 
  data[data_idx] &= ~(1 << bit_idx);
  return OK_RC;
}

RC RM_Bitmap::Test(unsigned int idx, bool& val) {
  RC rc;
  if (idx >= len) {
    rc = RM_BITMAP_OUTOFRANGE;
    RM_PrintError(RM_BITMAP_OUTOFRANGE);
    return rc;
  }

  // The idx at the char array that we look for
  auto data_idx = idx / 8; 
  // The bit position at the char we look for
  auto bit_idx = idx % 8;
  
  val = (data[data_idx] & (1 << bit_idx)); 
  return OK_RC;
}

