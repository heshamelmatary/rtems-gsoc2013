/*
 *  This set of routines starts the application.  It includes application,
 *  board, and monitor specific initialization and configuration.
 *  The generic CPU dependent initialization has been performed
 *  before any of these are invoked.
 *
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 */

#include <string.h>
#include <fcntl.h>
#include <bsp.h>
#include <bsp/irq.h>
#include <psim.h>
#include <bsp/bootcard.h>
#include <bsp/linker-symbols.h>
#include <bsp/openpic.h>
#include <rtems/bspIo.h>
#include <rtems/powerpc/powerpc.h>
#include <bsp.h>
//#include <bsp/pci.h>
#include <rtems/bspIo.h>
#include <libcpu/cpuIdent.h>
#include <libcpu/bat.h>
#include <libcpu/pte121.h>
#include <libcpu/spr.h>
#include <libcpu/mmu_support.h>

 
SPR_RW(SPRG1)

/*  On psim, each click of the decrementer register corresponds
 *  to 1 instruction.  By setting this to 100, we are indicating
 *  that we are assuming it can execute 100 instructions per
 *  microsecond.  This corresponds to sustaining 1 instruction
 *  per cycle at 100 Mhz.  Whether this is a good guess or not
 *  is anyone's guess.
 */
extern int PSIM_INSTRUCTIONS_PER_MICROSECOND[];

/*
 * PCI Bus Frequency
 */
unsigned int BSP_bus_frequency;

/*
 *  Driver configuration parameters
 */
uint32_t   bsp_clicks_per_usec;

/*
 * Memory on this board.
 */
uint32_t BSP_mem_size = (uint32_t)RamSize;

/*
 * Time base divisior (how many tick for 1 second).
 */
unsigned int BSP_time_base_divisor;

extern Triv121PgTbl BSP_pgtbl_setup(unsigned int *);
extern void BSP_pgtbl_activate(Triv121PgTbl);

extern void ShowBATS(void);

extern unsigned long __rtems_end[];

void BSP_panic(char *s)
{
  printk("%s PANIC %s\n",_RTEMS_version, s);
  __asm__ __volatile ("sc");
}

void _BSP_Fatal_error(unsigned int v)
{
  printk("%s PANIC ERROR %x\n",_RTEMS_version, v);
  __asm__ __volatile ("sc");
}

/*
 *  bsp_start
 *
 *  This routine does the bulk of the system initialization.
 */
void bsp_start( void )
{

  Triv121PgTbl  pt=0;
  /*
   * Note we can not get CPU identification dynamically.
   * PVR has to be set to PPC_PSIM (0xfffe) from the device
   * file.
   */

  get_ppc_cpu_type();

  /*
   *  initialize the device driver parameters
   */
  BSP_bus_frequency        = (unsigned int)PSIM_INSTRUCTIONS_PER_MICROSECOND;
  bsp_clicks_per_usec      = BSP_bus_frequency;
  BSP_time_base_divisor    = 1;

  /*
   * Initialize default raw exception handlers.
   */
  ppc_exc_initialize_with_vector_base(
    PPC_INTERRUPT_DISABLE_MASK_DEFAULT,
    (uintptr_t) bsp_section_work_begin,
    rtems_configuration_get_interrupt_stack_size(),
    (void *) 0xfff00000
  );

  /*
  // * Initalize RTEMS IRQ system
   */

  setdbat(0, 0x0<<24, 0x0<<24, 2<<24, _PAGE_RW);
  setibat(0, 0x0<<24, 0x0<<24, 2<<24,        0);
  /* PCI    */
  setdbat(1, 0x8<<24, 0x8<<24, 1<<24,  IO_PAGE);
  setdbat(2, 0xc<<24, 0xc<<24, 1<<24,  IO_PAGE);

  //setdbat(1, _IO_BASE, _IO_BASE, 0x10000000, IO_PAGE);

 // setdbat(1, 0xc<<24, 0xc<<24, 0x10000000, IO_PAGE);
  //setdbat(2, PCI_MEM_BASE+PCI_MEM_WIN0, PCI_MEM_BASE+PCI_MEM_WIN0, 0x10000000, IO_PAGE);
  //setdbat(2, 0xc<<24, 0xc<<24, 0x10000000, IO_PAGE);
  //setdbat(3, 0xb0000000, 0xb0000000, 0x10000000, IO_PAGE);

  pt = BSP_pgtbl_setup(&BSP_mem_size);

  if (!pt || TRIV121_MAP_SUCCESS != triv121PgTblMap(
       pt, TRIV121_121_VSID,
#ifndef qemu
      0xfeff0000,
#else
      0xbffff000,
#endif
      1,
      TRIV121_ATTR_IO_PAGE, TRIV121_PP_RW_PAGE)) {
      printk("WARNING: unable to setup page tables VME "
      "bridge must share PCI space\n");
      }

  BSP_rtems_irq_mng_init(0);

    /* Activate the page table mappings only after
     *    * initializing interrupts because the irq_mng_init()
     *       * routine needs to modify the text
     *          */
    if (pt) {
       printf("Page table setup finished; will activate it NOW...\n");
       BSP_pgtbl_activate(pt);
       /* finally, switch off DBAT3 */
       setdbat(3, 0, 0, 0, 0);
       }

      ShowBATS();

      printk("Exit from bspstart\n");
  //mmu_init();
  /*
   * Setup BATs and enable MMU
   */
  /* Memory */
   /* Clear BAT registers*/

  //setdbat(0, 0x0<<24, 0x0<<24, 2<<24, _PAGE_RW);
  //setibat(0, 0x0<<24, 0x0<<24, 2<<24,        0);
  /* PCI    */
  //setdbat(1, 0x8<<24, 0x8<<24, 1<<24,  IO_PAGE);
  //setdbat(2, 0xc<<24, 0xc<<24, 1<<24,  IO_PAGE);

/* Invalidate all TLB Entries */
   asm volatile("sync; isync; tlbia; sync; isync");
   _write_MSR(_read_MSR() | MSR_DR | MSR_IR);

  __asm__ volatile("sync; isync");
}
