/*
 * Init routines for STM32F4
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
	.thumb
	.syntax unified
	
	@Init routines section
	.section .stm_init, "ax", %progbits

	.align 2			@Align to a 4-byte boundary
	.global _init_entry
	.thumb_func
_init_entry:
	@Init routine basically prepares MCU to execute C code, i.e.:
	@ -fills .bss with zeros
	@ -initializes .data / .rodata in RAM
	@ -maybe, relocates vector table into SRAM?
	
	/* NOTE: stack (main) is already working here */	
	cpsid	f			@Disable exceptions
	
	@ fill .bss section with zeros
	ldr	r0, =_bss_start
	mov	r1, #0
	ldr	r2, =_bss_end
	sub	r2, r0
	bl	_asm_memset
	

#ifdef CONFIG_LD_RELOCATE_VT_SRAM
	@ copy VT from flash into .sram_vt section
	ldr	r0, =_sram_vt_start
	ldr	r1, =_init_vectable
	ldr	r2, =_sram_vt_end
	sub	r2, r0
	bl	_asm_memcpy

	@ load VTOR with new value
	ldr	r0, =_sram_vt_start
	ldr	r1, =0xE000ED08		@VTOR address
	str	r0, [r1]
#endif	/* CONFIG_LD_RELOCATE_VT_SRAM */

	@ init .data section (copy from flash)
	ldr	r0, =_data_start
	ldr	r1, =_data_lma
	ldr	r2, =_data_end
	sub	r2, r0
	bl	_asm_memcpy

	@ init .ccm_data section (copy from flash)
	ldr	r0, =_ccm_data_start
	ldr	r1, =_ccm_data_lma
	ldr	r2, =_ccm_data_end
	sub	r2, r0
	bl	_asm_memcpy

	cpsie	f			@Enable exceptions

	ldr	r0, =0xE000ED24		@SHCSR
	ldr	r1, [r0]
	orr	r1, #0x00070000
	str	r1, [r0]		@Enable all types of faults :3

#ifdef CONFIG_LD_STACK_DUAL
	@Change stack to program stack
	mrs	r0, control
	orr	r0, #0x00000002		@Set SPSEL=1 (use program stack)
	msr	control, r0
	ldr	sp, =_pstack_top	@Load new SP with top address
#endif	/* CONFIG_LD_STACK_DUAL */
	
	@Call main() function, maybe?
	bl	main
	@Terminate work (this is somewhat unusual for MCU, yeah?)
	cpsid	f			@Disable interrupts  (except NMI)
	1:				@Loop forever (try to sleep)
	wfi
	b 1b
	
stm_init_ltorg:
	.ltorg				@Literal pool for pseudo-LDRs

	/*
	 * Default exception handlers
	 * -NMI handler must be done in assembly, because NMI can't be masked or
	 *  disabled, and can arise anytime, even when C routines are unable to
	 *  be executed
	 * -IRQ and fault handlers are going to be done in C
	 *  Note that in this case it is _required_ to have working stack!
	 */
	
	.align 2
	.global _nmi_default_handler
	.thumb_func
_nmi_default_handler:
	bx lr				@Just return
