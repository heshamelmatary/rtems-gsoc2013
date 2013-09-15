/*
 * Copyright (c) 2013 Gedare Bloom.
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.com/license/LICENSE.
 */

#ifndef __LIBBSP_MM_H
#define __LIBBSP_MM_H

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

void bsp_memory_management_initialize(void);

void bsp_memory_management_set_attributes(
  uintptr_t base,
  size_t size,
  uint32_t attr
);

#ifdef __cplusplus
}
#endif
#endif
