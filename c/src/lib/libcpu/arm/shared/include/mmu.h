/*
 *  ARM MMU header file
 *  Copyright (c) 2013 Gedare Bloom.
 *  Copyright (c) 2013 Hesham AL-Matary.
 *  Copyright (c) 2004 by Cogent Computer Systems
 *  Written by Jay Monkman <jtm@lopingdog.com>
 */
#ifndef __MMU_H__
#define __MMU_H__

#include <stdint.h>

#define MMU_SECT_SIZE         0x100000

#define MMU_CACHE_NONE        0x0
#define MMU_CACHE_BUFFERED    0x1
#define MMU_CACHE_WTHROUGH    0x2
#define MMU_CACHE_WBACK       0x3

typedef struct {
    uint32_t paddr;
    uint32_t vaddr;
    uint32_t size;         /* in MB */
    uint8_t  cache_flags;
} mmu_sect_map_t;

/* This Control Structure applies for most ARM architectures */
typedef struct {
  uintptr_t vAddress;
  uintptr_t ptAddress;  /* physical address for page table lvl2 or lvl1 */
  uintptr_t ptlvl1Address; /* physical address for  level 1 page table */
  uint16_t pagesNumber; /* Number of used PTEs from this page table */
  //pt_type  type; /* lvl2 , lvl1 */
  uint8_t  ap;
  uint8_t  cb;
  uint8_t domain;
} arm920_mm_mme;

void mmu_init(mmu_sect_map_t *map);

#endif /* __MMU_H__ */
