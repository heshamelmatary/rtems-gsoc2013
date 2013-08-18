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
#define RTEMS_MM_REGION_NO_PROTECTION           0x0
#define RTEMS_MM_REGION_PROTECTION_READ_ONLY    0x1
#define RTEMS_MM_REGION_PROTECTION_WRITE        0x2
#define RTEMS_MM_REGION_NO_ACCESS               0x3
//#define RTEMS_MM_REGION_PROTECTION_EXEC   0x4

#ifdef __cplusplus
}
#endif

/**@}*/

#endif
