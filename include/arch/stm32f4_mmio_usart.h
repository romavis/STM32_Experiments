#ifndef STM32F4_MMIO_USART_H
#define STM32F4_MMIO_USART_H

/*
 * STM32F4 USART MMIO regs
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
	int32_t		SR;
	int32_t		DR;
	int32_t		BRR;
	int32_t		CR1;
	int32_t		CR2;
	int32_t		CR3;
	int32_t		GTPR;
} stm32f4_mmio_usart_t;

enum stm32f4_usart_port {
	STM32F4_USART1 = 0,
	STM32F4_USART2,
	STM32F4_USART3,
	STM32F4_UART4,
	STM32F4_UART5,
	STM32F4_USART6,
	STM32F4_UART7,
	STM32F4_UART8
};

stm32f4_mmio_usart_t * const STM32F4_MMIO_USART [] = {
	(stm32f4_mmio_usart_t *) STM32F4_MMIO_BASE_USART1,
	(stm32f4_mmio_usart_t *) STM32F4_MMIO_BASE_USART2,
	(stm32f4_mmio_usart_t *) STM32F4_MMIO_BASE_USART3,
	(stm32f4_mmio_usart_t *) STM32F4_MMIO_BASE_UART4,
	(stm32f4_mmio_usart_t *) STM32F4_MMIO_BASE_UART5,
	(stm32f4_mmio_usart_t *) STM32F4_MMIO_BASE_USART6,
	(stm32f4_mmio_usart_t *) STM32F4_MMIO_BASE_UART7,
	(stm32f4_mmio_usart_t *) STM32F4_MMIO_BASE_UART8
};

#endif /* STM32F4_MMIO_USART_H */
