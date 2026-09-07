#ifndef GEMMINI_PFU_ABI_H
#define GEMMINI_PFU_ABI_H

#include <stdint.h>

/* PFU V1 uses dedicated funct7 values; CONFIG_NORM is never overloaded. */
#define PFU_LOAD_FUNCT7 0x1au
#define PFU_CONFIG_FUNCT7 0x1bu

#define PFU_MAX_SEGMENTS 64u
#define PFU_MAX_L2_ENTRIES 128u
#define PFU_RECORD_BYTES 8u
#define PFU_MAX_IMAGE_BYTES 2064u
#define PFU_SLOT_BYTES 2112u

/* PFU_LOAD: rs1=64-byte-aligned DDR source, rs2 descriptor. */
#define PFU_LOAD_RS2(bank, flags, n, image_size_bytes) \
  (((uint64_t)(bank) & UINT64_C(3)) | \
   (((uint64_t)(flags) & UINT64_C(0x3f)) << 2) | \
   (((uint64_t)(n) & UINT64_C(0xffff)) << 16) | \
   ((uint64_t)(image_size_bytes) << 32))
#define PFU_LOAD_BANK(rs2) ((uint64_t)(rs2) & UINT64_C(3))
#define PFU_LOAD_FLAGS(rs2) (((uint64_t)(rs2) >> 2) & UINT64_C(0x3f))
#define PFU_LOAD_RESERVED(rs2) (((uint64_t)(rs2) >> 8) & UINT64_C(0xff))
#define PFU_LOAD_N(rs2) (((uint64_t)(rs2) >> 16) & UINT64_C(0xffff))
#define PFU_LOAD_IMAGE_BYTES(rs2) ((uint32_t)((uint64_t)(rs2) >> 32))

/* PFU_CONFIG: rs1[1:0]=bank, rs1[2]=enable, all remaining bits zero. */
#define PFU_CONFIG_RS1(bank, enable) \
  (((uint64_t)(bank) & UINT64_C(3)) | \
   (((uint64_t)(enable) & UINT64_C(1)) << 2))
#define PFU_CONFIG_BANK(rs1) ((uint64_t)(rs1) & UINT64_C(3))
#define PFU_CONFIG_ENABLE(rs1) (((uint64_t)(rs1) >> 2) & UINT64_C(1))
#define PFU_CONFIG_RESERVED(rs1) ((uint64_t)(rs1) >> 3)

/* 64-bit logical table-image records. */
#define PFU_HEADER0(n, e, f, k1, k2, cell_shift, num_l2_blocks) \
  (((uint64_t)(n) & UINT64_C(0xffff)) | \
   (((uint64_t)(e) & UINT64_C(0xff)) << 16) | \
   (((uint64_t)(f) & UINT64_C(0xff)) << 24) | \
   (((uint64_t)(k1) & UINT64_C(0xff)) << 32) | \
   (((uint64_t)(k2) & UINT64_C(0xff)) << 40) | \
   (((uint64_t)(cell_shift) & UINT64_C(0xff)) << 48) | \
   (((uint64_t)(num_l2_blocks) & UINT64_C(0xff)) << 56))
#define PFU_HEADER1(b_end) ((uint64_t)(uint32_t)(b_end))
#define PFU_L1_LEAF(segment_id) (((uint64_t)(segment_id) & UINT64_C(0x3f)) << 1)
#define PFU_L1_REFINE(block_id) (UINT64_C(1) | (((uint64_t)(block_id) & UINT64_C(0x3f)) << 1))
#define PFU_L2_SPLIT(boundary_id, residual) \
  (UINT64_C(1) | (((uint64_t)(boundary_id) & UINT64_C(0x3f)) << 1) | \
   (((uint64_t)(residual) & UINT64_C(0xffffff)) << 7))
#define PFU_SEGMENT_ENTRY(b, cq, y) \
  ((uint64_t)(uint32_t)(b) | (((uint64_t)(uint16_t)(cq)) << 32) | \
   (((uint64_t)(uint16_t)(y)) << 48))

enum pfu_status_code {
  PFU_STATUS_SUCCESS = 0,
  PFU_STATUS_LOAD_DMA_ERROR = 1,
  PFU_STATUS_LOAD_BAD_TABLE = 2,
  PFU_STATUS_LOAD_BAD_NUM_SEGMENTS = 3,
  PFU_STATUS_LOAD_ACTIVE_BANK = 4,
  PFU_STATUS_CONFIG_BANK_NOT_READY = 6,
  PFU_STATUS_CONFIG_INVALID = 7,
  PFU_STATUS_LOAD_BUSY = 0xau
};

#endif
