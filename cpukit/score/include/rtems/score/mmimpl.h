/**
 * @file
 *
 * @brief Inlined Routines from the Memory Management Manager
 */

/*
 * Copyright (c) 2013 Hesham AL-Matary
 * Copyright (c) 2013 Gedare Bloom.
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.com/license/LICENSE.
 */

#ifndef _RTEMS_SCORE_MMIMPL_H
#define _RTEMS_SCORE_MMIMPL_H

#ifdef RTEMS_SMP
#include <rtems/score/smplock.h>
#endif

#include <stdint.h>
#include <stdlib.h>
#include <rtems/score/mm.h>
#include <libcpu/mm.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup SuperCoreMM
 */
/**@{**/

#ifdef RTEMS_SMP
SMP_lock_Control mm_lock;
#endif

/**
 * @brief Calls _CPU_Memory_management_Initialize.
 */
inline void _Memory_management_Initialize( void )
{
#ifdef RTEMS_SMP   
  _SMP_lock_Initialize( &mm_lock );
#endif 

  _CPU_Memory_management_Initialize();
}

/**
 * @brief Calls _CPU_Memory_management_Set_attributes.
 */
inline void _Memory_management_Set_attributes(
  uintptr_t base,
  size_t size,
  uint32_t attr
)
{
#ifdef RTEMS_SMP   
  _SMP_lock_Acquire( &mm_lock );
#endif

  _CPU_Memory_management_Set_attributes(base, size, attr);

#ifdef RTEMS_SMP    
  _SMP_lock_Release( &mm_lock );
#endif
}

/** @}*/

#ifdef __cplusplus
}
#endif

#endif
/* end of include file */
