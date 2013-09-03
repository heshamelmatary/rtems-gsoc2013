/*
 *  ARM920 MMU functions
 *  Copyright (c) 2013 Gedare Bloom.
 *  Copyright (c) 2013 Hesham AL-Matary.
 *  Copyright (c) 2004 by Cogent Computer Systems
 *  Written by Jay Monkman <jtm@lopingdog.com>
 */
#include <libcpu/arm-cp15.h>
#include <libcpu/mm.h>
#include <libcpu/mmu.h>

typedef uint32_t mmu_lvl1_t;

extern uint32_t _ttbl_base;

static void mmu_set_map_inval(mmu_lvl1_t *base);

#define MMU_CTRL_MMU_EN             (1 << 0)
#define MMU_CTRL_MMU_DES            (0 << 0)
#define MMU_CTRL_ALIGN_FAULT_EN     (1 << 1)
#define MMU_CTRL_D_CACHE_EN         (1 << 2)
#define MMU_CTRL_D_CACHE_DES        (0 << 2)
#define MMU_CTRL_DEFAULT            (0xf << 3)
#define MMU_CTRL_LITTLE_ENDIAN      (0 << 7)
#define MMU_CTRL_BIG_ENDIAN         (1 << 7)
#define MMU_CTRL_SYS_PROT           (1 << 8)
#define MMU_CTRL_ROM_PROT           (1 << 9)
#define MMU_CTRL_I_CACHE_EN         (1 << 12)
#define MMU_CTRL_I_CACHE_DES        (0 << 12)
#define MMU_CTRL_LOW_VECT           (0 << 13)
#define MMU_CTRL_HIGH_VECT          (1 << 13)

#define ARM_MMU_AP_NOPR             0x03
#define ARM_MMU_AP_USER_READ_ONLY   0x02
#define ARM_MMU_AP_USER_NO_ACCESS   0x01
#define ARM_MMU_AP_NO_ACCESS        0x00

#define ARM_MMU_cb 0x0 //cache and buffer disabled
#define ARM_MMU_cB 0x1 // cache disable and buffer enabled
#define ARM_MMU_WT 0x2 // Write through
#define ARM_MMU_WB 0x3 // Write Back

#define MMU_SET_LVL1_SECT(addr, ap, dom, ce, be) \
          (((addr) & 0xfff00000) |     \
           (ap)                  |     \
           (dom)                 |     \
           ((ce) << 3)           |     \
           ((be) << 2)           |     \
           0x12)

#define MMU_SET_LVL1_INVAL (0x0)

#define MMU_SECT_AP_ALL (0x3 << 10)

uint32_t translation_table[] =
{
  ARMV7_MMU_DATA_READ_WRITE_CACHED,
  ARMV7_MMU_CODE_CACHED,
  ARMV7_MMU_DATA_READ_WRITE_CACHED,
  0U
};

/* Re-enable MMU (after updating PTs)
 * include two steps:
 * 1- flushing caches and TLB (to ignore old caches page tables).
 * 2- turn on MMU and caching unit after editing page tables.
 */
static void enable_mmu( void )
{
  /* flush the cache and TLB */
  arm_cp15_cache_invalidate();
  arm_cp15_tlb_invalidate();

  /*  I & D caches turned on */
  arm_cp15_set_control(
      MMU_CTRL_DEFAULT |
      MMU_CTRL_D_CACHE_EN |
      MMU_CTRL_I_CACHE_EN |
      MMU_CTRL_ALIGN_FAULT_EN |
      MMU_CTRL_LITTLE_ENDIAN |
      MMU_CTRL_MMU_EN
  );
}

/* Disable MMU (required before updating PTs)
 * include two steps:
 * 1- flushing caches and TLB (to ignore old caches page tables).
 * 2- turn of MMU and caching unit before editing page tables.
 */
static void disable_mmu( void )
{
  /* flush the cache and TLB */
  arm_cp15_cache_invalidate();
  arm_cp15_tlb_invalidate();

  /*  I & D caches turned off */
  arm_cp15_set_control(
      MMU_CTRL_DEFAULT |
      MMU_CTRL_D_CACHE_DES |
      MMU_CTRL_I_CACHE_DES |
      MMU_CTRL_ALIGN_FAULT_EN |
      MMU_CTRL_LITTLE_ENDIAN |
      MMU_CTRL_MMU_DES
  );
}

void mmu_init(mmu_sect_map_t *map)
{
    mmu_lvl1_t *lvl1_base;
    int i;

    /* flush the cache and TLB */
    arm_cp15_cache_invalidate();
    arm_cp15_tlb_invalidate();

    /* set manage mode access for all domains */
    arm_cp15_set_domain_access_control(0xffffffff);

    lvl1_base = (mmu_lvl1_t *)&_ttbl_base;

    /* set up the trans table */
    mmu_set_map_inval(lvl1_base);
    arm_cp15_set_translation_table_base(lvl1_base);

    /* create a 1:1 mapping of the entire address space */
    i = 0;
    while(map[i].size != 0) {
        int c = 0;  /* to avoid uninitialized warnings */
        int b = 0;  /* to avoid uninitialized warnings */
        int pbase;
        int vbase;
        int sects;

        switch (map[i].cache_flags) {
        case MMU_CACHE_NONE:
            c = 0;
            b = 0;
            break;
        case MMU_CACHE_BUFFERED:
            c = 0;
            b = 1;
            break;
        case MMU_CACHE_WTHROUGH:
            c = 1;
            b = 0;
            break;
        case MMU_CACHE_WBACK:
            c = 1;
            b = 1;
            break;
        }

        pbase = (map[i].paddr & 0xfff00000) >> 20;
        vbase = (map[i].vaddr & 0xfff00000) >> 20;
        sects = map[i].size;

        while (sects > 0) {
            lvl1_base[vbase] = MMU_SET_LVL1_SECT(pbase << 20,
                                                 MMU_SECT_AP_ALL,
                                                 0,
                                                 c,
                                                 b);
            pbase++;
            vbase++;
            sects--;
        }
        i++;
    }

    /* flush the cache and TLB */
    arm_cp15_cache_invalidate();
    arm_cp15_tlb_invalidate();

    /*  I & D caches turned on */
    arm_cp15_set_control(MMU_CTRL_DEFAULT |
                         MMU_CTRL_D_CACHE_EN |
                         MMU_CTRL_I_CACHE_EN |
                         MMU_CTRL_ALIGN_FAULT_EN |
                         MMU_CTRL_LITTLE_ENDIAN |
                         MMU_CTRL_MMU_EN);

    return;
}

/* Changing Page table attributes to new attributes */
void arm_Region_Change_Attr(
    uintptr_t base,
    size_t size,
    uint32_t AP,
    uint32_t CB
)
{
  uintptr_t      *lvl1_pt;
  int             sectionsNumber; /* 1MB sections */
  int             PTEIndex;
  uintptr_t       paddr;
  uintptr_t       paddr_base;

  int i;
  int c;
  int b;

  sectionsNumber = size / MMU_SECT_SIZE;

  lvl1_pt = &_ttbl_base;
  PTEIndex = ((base & 0xfff00000U) >> 20U);
  paddr = (base & 0xfff00000U);

  disable_mmu();

  switch (CB) {
    case ARM_MMU_cb:
      c = 0;
      b = 0;
      break;
    case ARM_MMU_cB:
      c = 0;
      b = 1;
      break;
    case ARM_MMU_WT:
      c = 1;
      b = 0;
      break;
    case ARM_MMU_WB:
      c = 1;
      b = 1;
      break;
  }

  /* Return AP/CB for this region to defaults */
  for ( i = 0; i < sectionsNumber; i++) {
    paddr_base = (i<<20U) + paddr;
    lvl1_pt[PTEIndex++] = MMU_SET_LVL1_SECT(
        paddr_base,
        AP,
        0,
        c,
        b
    );
  }

  enable_mmu();

  return RTEMS_SUCCESSFUL;
}

/* Initialize first page table level with no protected entries */
void bsp_memory_management_Initialize( void )
{
  uintptr_t *lvl1_base;
  uintptr_t paddr;
  int i;

  /* flush the cache and TLB */
  arm_cp15_cache_invalidate();
  arm_cp15_tlb_invalidate();

  /* set manage mode access for all domains */
  arm_cp15_set_domain_access_control(0xffffffff);

  lvl1_base = &_ttbl_base;

  /* set up the trans table */
  /*mmu_set_map_inval(lvl1_base);*/
  arm_cp15_set_translation_table_base(lvl1_base);
  //puts(" Before loopint \n" );
  /* fill level 1 pagetable with no protection slots, Cache through attributes
   * and 1:1 address mapping
   */
  for (i = 0; i < (0x4000 / 4); i++) {
    paddr = i; /* i = 1MB page size */
    lvl1_base[i] = MMU_SET_LVL1_SECT(
        (paddr << 20U),
        ARM_MMU_AP_NOPR,
        0,
        1,
        0
    );
  }

  enable_mmu();

  return RTEMS_SUCCESSFUL;
}

/** 
 * Set attributes for a block.
 */
void _CPU_Memory_management_Set_attributes(
  uintptr_t base,
  size_t size,
  uint32_t attr
)
{
  uint32_t arm_mmu_attr;
  uintptr_t      *lvl1_pt;
  int             sectionsNumber; /* 1MB sections */
  int             PTEIndex;
  uintptr_t        paddr;
  uintptr_t        paddr_base;
  
  int             i;

  lvl1_pt = &_ttbl_base;
  PTEIndex = ((base & 0xfff00000U) >> 20U);
  paddr = base & 0xfff00000U;
  size = size;

  sectionsNumber = (size / MMU_SECT_SIZE);

  disable_mmu();
  
  arm_mmu_attr = translation_table[attr];

  for ( i = 0; i < sectionsNumber; i++ ) {
    paddr_base = (i<<20U) + paddr;

    lvl1_pt[PTEIndex++] = MMU_SET_LVL1_SECT(
        paddr_base,
        arm_mmu_attr,
        0,
        1,
        0
    );
  }

  enable_mmu();

  return RTEMS_SUCCESSFUL;
}

/* set all the level 1 entrys to be invalid descriptors */
static void mmu_set_map_inval(mmu_lvl1_t *base)
{
    int i;
    for (i = 0; i < (0x4000 / 4); i++) {
        base[i] = MMU_SET_LVL1_INVAL;
    }
}

void mmu_set_cpu_async_mode(void)
{
    uint32_t reg;
    reg = arm_cp15_get_control();
    reg |= 0xc0000000;
    arm_cp15_set_control(reg);
}
