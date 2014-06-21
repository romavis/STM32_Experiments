#ifndef STM32F4_MMIO_RCC_H
#define STM32F4_MMIO_RCC_H

/*
 * STM32F4 RCC functions
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

#include <stdint.h>

#include <arch/stm32f4_mmiomap.h>

/* RCC registers struct */

typedef volatile struct __attribute__((__packed__)) {
	int32_t		CR,
	int32_t		PLLCFGR,
	int32_t		CFGR,
	int32_t		CIR,
	int32_t		AHB1RSTR,
	int32_t		AHB2RSTR,
	int32_t		AHB3RSTR,
	int32_t		_rsvd1,
	int32_t		APB1RSTR,
	int32_t		APB2RSTR,
	int32_t		_rsvd2,
	int32_t		_rsvd3,
	int32_t		AHB1ENR,
	int32_t		AHB2ENR,
	int32_t		AHB3ENR,
	int32_t		_rsvd4,
	int32_t		APB1ENR,
	int32_t		APB2ENR,
	int32_t		_rsvd5,
	int32_t		_rsvd6,
	int32_t		AHB1LPENR,
	int32_t		AHB2LPENR,
	int32_t		AHB3LPENR,
	int32_t		_rsvd7,
	int32_t		APB1LPENR,
	int32_t		APB2LPENR,
	int32_t		_rsvd8,
	int32_t		_rsvd9,
	int32_t		BDCR,
	int32_t		CSR,
	int32_t		_rsvd10,
	int32_t		_rsvd11,
	int32_t		SSCGR,
	int32_t		PLLI2SCFGR,
	/* F42x, F43x only */
	int32_t		PLLSAICFGR,
	int32_t		DCKCFGR
} stm32f4_mmio_rcc_t;

stm32f4_mmio_rcc_t * const STM32F4_MMIO_RCC = (STM32F4_RCC_MMIO_t *)
					      STM32F4_MMIO_BASE_RCC;

#endif /* STM32F4_MMIO_RCC_H */
