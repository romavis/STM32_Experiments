/*
 * Custom linker script for STM32F407
 *
 * This script can be used to link code to be run from embedded flash (not SRAM
 * or external memory)
 *
 * NOTE: Preprocess with GCC first!
 * 
 * Copyright (C) 2013 Roman Dobrodiy <ztcoils _d@g_ gmail.com>
 * 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Lesser General Public License
 * (LGPL) version 2.1 which accompanies this distribution, and is available at
 * http://www.gnu.org/licenses/lgpl-2.1.html
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 */

ENTRY(_init_entry)

MEMORY
{
	code_alias (rx): org = 0x00000000, len = 0x00100000	/* 1 MiB */
	flash	   (rx): org = 0x08000000, len = 0x00100000	/* 1 MiB */
	ccm_sram   (rw): org = 0x10000000, len = 0x00010000	/* 64 KiB */
	sram	   (rwx): org = 0x20000000, len = 0x00020000	/* 128 KiB */
	bkpsram	   (rw): org = 0x40024000, len = 0x00001000	/* 4 KiB */
}


/******************************************************************************
 * USED DEFINES                                                               *
 ******************************************************************************
 * CONFIG_LD_HEAP_CCM - place .heap in CCM                                    *
 * CONFIG_LD_HEAP_SRAM - place .heap in SRAM                                  *
 * CONFIG_LD_MSTACK_SIZE - main stack size (.mstack)                          * 
 * CONFIG_LD_PSTACK_SIZE - program stack size (.pstack, if applicable)        *
 * CONFIG_LD_STACK_DUAL - allocate program stack (.pstack)                    *
 * CONFIG_LD_RELOCATE_VT_SRAM - relocate vector table in SRAM at init         *
 ******************************************************************************/

/******************************************************************************
 * Stack overflow protection                                                  *
 ******************************************************************************
 * According to PM2014 "STM32F3xxx and STM32F4xxx Cortex-M4 programming manual":
 * "If a bus fault occurs during a stack push when entering a bus fault handler,
 * the bus fault does not escalate to a hard fault. This means that if a
 * corrupted stack causes a fault, the fault handler executes even though the
 * stack push for the handler failed. The fault handler operates but the stack
 * contents are corrupted." (page 45, rev 3)
 * 
 * There are two choices:
 * 1) Single stack (main stack)
 * 	Linker allocates memory only for main stack (at the beginning of RAM),
 * 	init routine initializes MSP register, and don't use PSP. Main stack is
 * 	used both for program execution and exception handling. In case of stack
 * 	overflow bus fault exception occurs, BUT it can't be properly stacked!
 * 	(exceptions use MSP, which is corrupted).
 *
 * 	Because of corrupted stack, it is almost impossible to write fault
 * 	handler in C, OR there must be thin assembler layer that would reset SP
 * 	for C handler, which would overwrite stack.
 * 	
 * 
 * 2) Double stack (main + program)
 * 	Linker allocates memory both for main and program stacks. Init routine
 * 	selects PSP as active stack.
 * 	Program stack is located at the beginning of RAM to be able to detect
 * 	stack overflows. Program stack overflow leads to bus fault, and it can
 * 	be stacked correctly (because in this case exception stacks on main
 * 	stack, which is in correct state).
 * 	Backtrace is available in this case.
 */

SECTIONS
{
	/**********************************************************************
	 * Flash memory sections (at runtime)                                 *
	 **********************************************************************/
	
	/*
	 * Special section that contains vector table and init-related code
	 * Placed exactly at 0x0
	 */
	.init 0x0 :
	{
		/* Vector table */
		KEEP(*(.stm_vectable))
		/* Init routines, debug */
		KEEP(*(.stm_init))
	} >code_alias AT> flash

	/* Generic program code section. Placed in flash region */
	.text : ALIGN(4) 
	{
		_text_start = ABSOLUTE(.);
		*(.text)
		*(.text.*)
		_text_end = ABSOLUTE(.);
	} >code_alias AT> flash

	/* Constants in flash */
	.flash_rodata : ALIGN(4)
	{
		_fp_k_table_f_start = ABSOLUTE(.);
		KEEP(*(.fp_k_table_f))
		_fp_k_table_d_start = ABSOLUTE(.);
		KEEP(*(.fp_k_table_d))
		
		*(.flash_rodata)
	} >flash
	
	/* Unwind tables section required to link libgcc */
        .ARM.exidx   :
        {
		__exidx_start = .;
		*(.ARM.exidx* .gnu.linkonce.armexidx.*)
		__exidx_end = .;
	} >flash
	
	/**********************************************************************
	 * CCM SRAM sections                                                   *
	 **********************************************************************/

#ifdef CONFIG_LD_STACK_DUAL
	/* Program stack */
	.pstack ORIGIN(ccm_ram) (NOLOAD) : ALIGN(8)
	{
		_pstack_start = ABSOLUTE(.);
		. += CONFIG_LD_PSTACK_SIZE;
		. = ALIGN(8);
		_pstack_end = ABSOLUTE(.);
	} >ccm_sram
	_pstack_top = _pstack_end - 8;
#endif	/* CONFIG_LD_STACK_DUAL */
	/* Main stack */
	.mstack (NOLOAD) : ALIGN(8)
	{
		_mstack_start = ABSOLUTE(.);
		. += CONFIG_LD_MSTACK_SIZE;
		. = ALIGN(8);
		_mstack_end = ABSOLUTE(.);
	} >ccm_sram
	_mstack_top = _mstack_end - 8;

	/* Static data in CCM */
	.ccm_data : ALIGN(4)
	{
		_ccm_data_start = ABSOLUTE(.);
		*(.ccm_data)
		*(.mcbdiag)
		_ccm_data_end = ABSOLUTE(.);
	} >ccm_sram AT> flash
	_ccm_data_lma = LOADADDR(.ccm_data);
	
#ifdef CONFIG_LD_HEAP_CCM
	/* Heap in CCM */
	/* It is the latest section of CCM and occupies all the space left */
	.heap (NOLOAD) : ALIGN(4)
	{
		_heap_begin = ABSOLUTE(.);
		. += ORIGIN(ccm_sram) + LENGTH(ccm_sram);
		_heap_end = ABSOLUTE(.);
	} >ccm_sram
#endif	/* CONFIG_LD_HEAP_CCM */

	/**********************************************************************
	 * Ordinal SRAM sections (at runtime)                                 *
	 **********************************************************************/

#ifdef CONFIG_LD_RELOCATE_VT_SRAM
	/* Vector table in SRAM */
	.sram_vt (NOLOAD) : ALIGN(512)
	{

		_sram_vt_start = ABSOLUTE(.);
		/* Allocate space for vector table here.. */
		. += 256*4;
		_sram_vt_end = ABSOLUTE(.);
	} >sram
#endif	/* CONFIG_LD_RELOCATE_VT_SRAM */

	
	.data : ALIGN(4)
	{
		_data_start = ABSOLUTE(.);
		*(.data)
		*(.data.*)
		*(.rodata)
		*(.rodata.*)
		. = ALIGN(4);
		*(.fastrun)
		_data_end = ABSOLUTE(.);
	} >sram AT> flash
	_data_lma = LOADADDR(.data);

	
	.bss (NOLOAD) : ALIGN(4)
	{
		_bss_start = ABSOLUTE(.);
		*(.bss)
		*(COMMON)
		_bss_end = ABSOLUTE(.);
	} >sram

#ifdef CONFIG_LD_HEAP_SRAM
	/* Heap in SRAM */
	/* It is the latest section of SRAM and occupies all the space left */
	.heap (NOLOAD) : ALIGN(4)
	{
		_heap_begin = ABSOLUTE(.);
		. += ORIGIN(sram) + LENGTH(sram);
		_heap_end = ABSOLUTE(.);
	} >sram
#endif	/* CONFIG_LD_HEAP_SRAM */

	/**********************************************************************
	 * Backup SRAM sections (Beware: absolutely uninitialized!)           *
	 **********************************************************************/
	.bkpsram (NOLOAD) :
	{
		_bkpsram_begin = ABSOLUTE(.);
		*(.bkpsram)
		_bkpsram_end = ABSOLUTE(.);
	} >bkpsram
}
