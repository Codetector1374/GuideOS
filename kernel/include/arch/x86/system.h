//
// Created by Codetector on 3/6/22.
//
#pragma once

#define GDT_KERNEL_CODE     0x1
#define GDT_KERNEL_DATA     0x2
#define GDT_USER_CODE       0x3
#define GDT_USER_DATA       0x4
#define GDT_TSS             0x5

struct cpu;

#pragma pack(push, 1)
struct tss_descriptor {
  u16 limit;
  u16 base_15_0;
  u8 base_23_16;
  u8 type:5;
  u8 dpl:2;
  u8 p:1;
  u8 limit_19_16:4;
  u8 flags:4;
  u8 base_31_24;
  u32 base_63_32;
  u32 _resvd;
};
_Static_assert(sizeof(struct tss_descriptor) == 16, "tss_descriptor packing wrong");

struct segment_descriptor {
  u16 limit;
  u16 base_15_0;
  u8 base_23_16;
  u8 type:5;
  u8 dpl:2;
  u8 p:1;
  u8 limit_19_16:4;
  u8 flags:4;
  u8 base_31_24;
};
_Static_assert(sizeof(struct segment_descriptor) == 8, "segment_descriptor packing wrong");

#define GDT_DESC_TYPE_CODE  0b11000
#define GDT_DESC_TYPE_DATA  0b10000
#define GDT_DESC_TYPE_TSS   0b01001

#define GDT_DESC_FLAGS_L    0b0010
#define GDT_DESC_FLAGS_DB   0b0100
#define GDT_DESC_FLAGS_G    0b1000


struct gdt {
  struct segment_descriptor seg_descs[5];
  struct tss_descriptor tss_entry;
};
#pragma pack(pop)

void gdt_setup(struct cpu*);
