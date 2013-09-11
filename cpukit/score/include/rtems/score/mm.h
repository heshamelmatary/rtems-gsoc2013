/**
 * @file
 *
 * @brief Manages use of MPU/MMU units to provide memory management.
 */

/*
 * Copyright (c) 2013 Hesham Al-Matary.
 * Copyright (c) 2013 Gedare Bloom.
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.com/license/LICENSE.
 */

#ifndef _RTEMS_SCORE_MM_H
#define _RTEMS_SCORE_MM_H

#include <stdlib.h> 
/* @defgroup SuperCoreMM Memory Management Support
 *
 * @ingroup Score
 */
/**@{*/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief _Memory_management_Region Flags defs      
 */

#define RTEMS_MM_REGION_BIT_READ        0
#define RTEMS_MM_REGION_BIT_WRITE       1
#define RTEMS_MM_REGION_BIT_EXECUTE     2
#define RTEMS_MM_REGION_BIT_CACHE       3
#define RTEMS_MM_REGION_BIT_DEVICE      4
#define RTEMS_MM_REGION_BIT_SHARED      5

#define RTEMS_MM_REGION_READ    (1U << RTEMS_MM_REGION_BIT_READ)
#define RTEMS_MM_REGION_WRITE   (1U << RTEMS_MM_REGION_BIT_WRITE)
#define RTEMS_MM_REGION_EXECUTE (1U << RTEMS_MM_REGION_BIT_EXECUTE)
#define RTEMS_MM_REGION_CACHE   (1U << RTEMS_MM_REGION_BIT_CACHE)
#define RTEMS_MM_REGION_DEVICE  (1U << RTEMS_MM_REGION_BIT_DEVICE)
#define RTEMS_MM_REGION_SHARED  (1U << RTEMS_MM_REGION_BIT_SHARED)

void _Memory_management_Set_attributes(
  uintptr_t base,
  size_t size,
  uint32_t attr
);

#ifdef __cplusplus
}
#endif

/**@}*/

#endif
