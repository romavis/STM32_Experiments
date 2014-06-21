#ifndef STM32F4_MMIO_GPIO_H
#define STM32F4_MMIO_GPIO_H

/*
 * STM32F4 GPIO MMIO registers
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

#define STM32F4_GPIO_MODER_SH(pin) ((pin) << 1)
#define STM32F4_GPIO_OTYPER_SH(pin) ((pin) << 0)
#define STM32F4_GPIO_OSPEEDR_SH(pin) ((pin) << 1)
#define STM32F4_GPIO_PUPDR_SH(pin) ((pin) << 1)

#define STM32F4_GPIO_DR_SH(pin) ((pin) << 0)
#define STM32F4_GPIO_BSRR_SH(pin) ((pin) << 2)

#define STM32F4_GPIO_AFR_SH(pin) ((pin & 0x7) << 2)

/* GPIO registers struct */

typedef volatile struct __attribute__((__packed__)) {
	int32_t		MODER;
	int32_t		OTYPER;
	int32_t		OSPEEDR;
	int32_t		PUPDR;
	int32_t		IDR;
	int32_t		ODR;
	int32_t		BSRR;
	int32_t		LCKR;
	int32_t		AFRL;
	int32_t		AFRH;
} stm32f4_mmio_gpio_t;

static stm32f4_mmio_gpio_t * const STM32F4_MMIO_GPIO [] = {
	(stm32f4_mmio_gpio_t *) STM32F4_MMIO_BASE_GPIOA,
	(stm32f4_mmio_gpio_t *) STM32F4_MMIO_BASE_GPIOB,
	(stm32f4_mmio_gpio_t *) STM32F4_MMIO_BASE_GPIOC,
	(stm32f4_mmio_gpio_t *) STM32F4_MMIO_BASE_GPIOD,
	(stm32f4_mmio_gpio_t *) STM32F4_MMIO_BASE_GPIOE,
	(stm32f4_mmio_gpio_t *) STM32F4_MMIO_BASE_GPIOF,
	(stm32f4_mmio_gpio_t *) STM32F4_MMIO_BASE_GPIOG,
	(stm32f4_mmio_gpio_t *) STM32F4_MMIO_BASE_GPIOH,
	(stm32f4_mmio_gpio_t *) STM32F4_MMIO_BASE_GPIOI,
	(stm32f4_mmio_gpio_t *) STM32F4_MMIO_BASE_GPIOJ,
	(stm32f4_mmio_gpio_t *) STM32F4_MMIO_BASE_GPIOK
};

#endif /* STM32F4_MMIO_GPIO_H */
