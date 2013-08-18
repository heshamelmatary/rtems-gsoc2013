/*
 * Copyright (c) 2013 Gedare Bloom.
 * Copyright (c) 2012 Hesham Al-Matary.
 * Copyright (c) 2011 Quanming Shi.
 * Copyright (c) 2010 Aanjhan Rangathan.
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.com/license/LICENSE.
 */

//#include <libcpu/mm.h>
#include <rtems.h>
#include <libcpu/spr.h>
#include <libcpu/mmu_support.h>
#include <libcpu/bat.h>
#include <stdlib.h>

uint32_t translation_table[] =
{
  _PPC_MMU_ACCESS_NO_PROT,
  _PPC_MMU_ACCESS_READ_ONLY,
  _PPC_MMU_ACCESS_SUPERVISOR_WRITE_ONLY,
  _PPC_MMU_ACCESS_SUPERVISOR_ONLY
};

SPR_RW(SDR1);

void _CPU_Memory_management_Initialize(void)
{
  uintptr_t pt_base, pt_end;
  uint32_t cache_line_size;
  mpc6xx_mm_mme* pte;
  mpc6xx_mm_mme *pte_base;
  mpc6xx_mm_mme *pte_end;
  unsigned long msr;

  pt_base = _read_SDR1() & 0xffff0000U;
  pt_end = pt_base + (( ( _read_SDR1() & 0x000001ffU)+1U )<<16U);
  pte_base = (mpc6xx_mm_mme*)pt_base;
  pte_end = (mpc6xx_mm_mme*)pt_end;

  /* Switch MMU and other Interrupts off */
  msr = _read_MSR();
  //_write_MSR(msr & ~ (MSR_EE | MSR_DR | MSR_IR));

  asm volatile ("sync":::"memory");
   /*rtems_cache_flush_entire_data for mpc6XX is not actually implemented*/
  /*rtems_cache_flush_entire_data();*/

  /*I am not quite sure is it proper to flush each line of entire address*
   * space for flushing cache.*/
  rtems_cache_flush_multiple_data_lines((void*)RamBase, (size_t)RamSize);

  for ( pte = pt_base; pte < pt_end; pte++ ) {
    pte->ptew0 &= ~0x80000000U;
  }

  asm volatile(  \
    " sync ;  isync; \n"  \
    " tlbia ;  eieio;  \n"  \
    " tlbsync ;        \n"  \
    " sync ; isync;  \n"  \
    : : :"0");

  /* restore, i.e., switch MMU and IRQs back on */
  //_write_MSR( msr );
  setdbat(0, 0x0<<24, 0x0<<24, 2<<24, _PAGE_RW);
  setibat(0, 0x0<<24, 0x0<<24, 2<<24,        0);
  /* PCI    */
  setdbat(1, 0x8<<24, 0x8<<24, 1<<24,  IO_PAGE);
  setdbat(2, 0xc<<24, 0xc<<24, 1<<24,  IO_PAGE);
  /*
  CLRBAT (DBAT0);
  CLRBAT (DBAT1);
  CLRBAT (DBAT2);
  CLRBAT (DBAT3);
  CLRBAT (IBAT0);
  */
}

/*static void ppc_pte_change_attributes(
  uint32_t base,
  size_t size,
  uint32_t wimg,
  uint32_t pp
)
{
  mpc6xx_mm_mme* pt_entry;
  uintptr_t ea, block_end;
  unsigned long msr;

  if ( mme->bsp_mme != NULL ) {
    ea = base;
    block_end = (uint32_t) base + (uint32_t) size;
    rtems_cache_flush_multiple_data_lines(base, size);
    pt_entry = (mpc6xx_mm_mme *) mme->bsp_mme;

    // Switch MMU and other Interrupts off 
    msr = _read_MSR();
    _write_MSR(msr & ~ (MSR_EE | MSR_DR | MSR_IR));

    for ( ; ea < block_end; ea += RTEMS_MPE_PAGE_SIZE ) {
      pt_entry->ptew0 &= ~PTEW0_VALID;

      asm volatile ("sync":::"memory");

      pt_entry->ptew1 |= PTEW1_WIMG & wimg;

      pt_entry->ptew1 &= ~PTEW1_PROTP;
      pt_entry->ptew1 |= PTEW1_PROTP & pp;

      asm volatile ("sync; tlbie %0; eieio"::"r" (ea):"memory");
      pt_entry->ptew0 &= ~PTEW0_VALID;
      asm volatile ("tlbsync; sync":::"memory");
      pt_entry--;
    }
    // restore, i.e., switch MMU and IRQs back on 
    _write_MSR( msr );
  }
}*/

static void translate_attributes(uint32_t high_level_attr, uint32_t *PPC_CPU_ATTR)
{
  //TODO: Checking for invalid flags combinations.
  
  /* Clear flags attributes */
  *PPC_CPU_ATTR = 0;

  if ( high_level_attr == 0 ) 
    *PPC_CPU_ATTR |= _PPC_MMU_ACCESS_NO_PROT;
  
  /* Read access */
  if ( high_level_attr & 0x1 ) 
    *PPC_CPU_ATTR |= _PPC_MMU_ACCESS_READ_ONLY;

  /* Write access */
  if ( high_level_attr & 0x2 )
    *PPC_CPU_ATTR |= _PPC_MMU_ACCESS_SUPERVISOR_WRITE_ONLY;

  if ( high_level_attr & 0x4 )
    *PPC_CPU_ATTR |= _PPC_MMU_ACCESS_SUPERVISOR_ONLY; 
}

void _CPU_Memory_management_Set_attributes(
    uintptr_t base,
    size_t size,
    uint32_t attr
)
{
  mpc6xx_mm_mme* pt_entry, * ppteg,*spteg;
  int status;
  unsigned long msr;
  uint32_t   sr_data, vsid, pi, hash1, hash2, api;
  int ppteg_search_status, spteg_search_status;
  int alut_access_attrb;
  uintptr_t ea, block_end;
  int wimg, pp;

  ea = base;
  block_end = (uint32_t) base + (uint32_t)  size;
  rtems_cache_flush_multiple_data_lines(base, size);

  for ( ; ea < block_end; ea += RTEMS_MPE_PAGE_SIZE ) {

    /* Read corresponding SR Data */
    sr_data = _read_SR((void *) ea);

    /* Extract VSID */
    vsid = sr_data & SR_VSID;

    /* get page index (PI) from EA */
    pi = (ea >> 12U) & 0x0000ffffU;
    api = pi >> 10U;

    /* Compute HASH 1 */
    hash1 = PTE_HASH_FUNC1(vsid, pi);

    /* Compute PTEG Address from the hash 1 value */
    get_pteg_addr(&ppteg, hash1);

    //pp = _PPC_MMU_ACCESS_NO_PROT;
    //tanslate attributes from high-level layers
    pp = translation_table[attr];
    //ppc_pte_change_attributes(base, size, 0x0, pp);
    pt_entry = BSP_ppc_add_pte(ppteg, spteg, vsid, pi, wimg, pp);
  }

    msr = _read_MSR();

    _write_MSR(msr & ~ (MSR_EE | MSR_DR | MSR_IR));

    for ( ; ea < block_end; ea += RTEMS_MPE_PAGE_SIZE ) {
      pt_entry->ptew0 &= ~PTEW0_VALID;

      asm volatile ("sync":::"memory");

      pt_entry->ptew1 |= PTEW1_WIMG & wimg;

      pt_entry->ptew1 &= ~PTEW1_PROTP;
      pt_entry->ptew1 |= PTEW1_PROTP & pp;

      asm volatile ("sync; tlbie %0; eieio"::"r" (ea):"memory");
      pt_entry->ptew0 &= ~PTEW0_VALID;
      asm volatile ("tlbsync; sync":::"memory");
      pt_entry--;
    }
    /* restore, i.e., switch MMU and IRQs back on */
    _write_MSR( msr );

}

