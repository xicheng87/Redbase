//
// rm_bitmap.cc
//
// Implementation of Record Manager's bitmap

#include "rm_bitmap.h"

RM_Bitmap::RM_Bitmap(unsigned int n, char* bits) : len(n), 
    data((unsigned char*)bits) {}

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

RC RM_Bitmap::FindFirstUnset(unsigned int& idx) {
  RC rc;
  unsigned int slot_num = len / 8 + (len % 8 != 0);
  for (unsigned int i = 0; i < slot_num; i++) {
    if (data[i] != 0xFF) {
      // The first thing is to compute the index of the least significant 0-bit
      // which represents the first unset bit on this 8-bit slot. Since 
      // __builtin_ffs returns the one plus index of the least signficatn 1-bit
      // we can compute thie via __builtin_ffs(~data[i]) - 1. 
      unsigned int res = i * 8 + __builtin_ffs(~data[i]) - 1;
      // Note that we can have unused bit if there is a cut off of the last
      // char bytes, so we still need to check if res is in the currect range. 
      // If not, then it means bitmap is full
      if (res >= len) {
        rc = RM_BITMAP_FULL;
        return rc;
      } 
      idx = res;
      return OK_RC;
    }
  }

  return RM_BITMAP_FULL;
}
