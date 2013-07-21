/*
 * Copyright (c) 2013 Hesham AL-Matary
 * Copyright (c) 2013 Gedare Bloom.
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.com/license/LICENSE.
 */

#include <rtems.h>
#include <libcpu/mm.h>

void _CPU_Memory_management_Initialize( void ) { }

void _CPU_Memory_management_Install_entry( uintptr_t base, size_t size, uint32_t flags ) { }

void _CPU_Memory_management_Uninstall_entry( uintptr_t base, size_t size ) { }
