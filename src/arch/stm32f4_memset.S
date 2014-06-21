/*
 * Cortex-M4 memset routine in assembly
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
	
	@Code section
	.section .text, "ax", %progbits

	.weak memset
	.thumb_set memset, _asm_memset
	
	.global _asm_memset
	.align 2			@Align to a 4-byte boundary
	.thumb_func
_asm_memset:
	/*
	 * Fills specified memory with zeros
	 * Preserves r4 - r11
	 * Args:
	 * 	r0 - dest_start
	 * 	r1 - value
	 * 	r2 - size (becomes dest_end)
	 */
	and	r1, #0xFF
	orr	r1, r1, r1, LSL #8
	orr	r1, r1, r1, LSL #16
	
	add	r2, r0
	sub	r2, #8
	1:				@Write 8 bytes at once
	cmp	r2, r0
	itt ge
	strdge	r1, r1, [r0], #8
	bge	1b

	add	r2, #8
	1:				@Finish, 1 byte/loop cycle
	cmp	r2, r0
	itt gt
	strbgt	r1, [r0], #1
	bgt	1b

	bx	lr			@return
