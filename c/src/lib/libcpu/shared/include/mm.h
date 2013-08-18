/*
 * Copyright (c) 2013 Gedare Bloom.
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.com/license/LICENSE.
 */

#ifndef __LIBCPU_MM_H
#define __LIBCPU_MM_H

#include <rtems/score/mmimpl.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Attributes' translation table.
 * This table should be filled by every
 * low level libmm implementation to 
 * translate high-level attributes into 
 * CPU specific attributes.
 */
//uint32_t translation_table[] = {0, 0, 0, 0};

#ifdef __cplusplus
}
#endif
#endif
