/*
 * STM32F4 GPIO driver
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

#include <arch/stm32f4_gpio.h>
#include <arch/stm32f4_mmio_gpio.h>
#include <util.h>

void stm32f4_gpio_configure(uint32_t config)
{
	unsigned int port = STM32F4_GPIO_GET_PORT(config);
	unsigned int pin = STM32F4_GPIO_GET_PIN(config);
	unsigned int mode = STM32F4_GPIO_GET_MODE(config);
	unsigned int otype = STM32F4_GPIO_GET_OUTP(config);
	unsigned int speed = STM32F4_GPIO_GET_SPEED(config);
	unsigned int pull = STM32F4_GPIO_GET_PULL(config);
	unsigned int af = STM32F4_GPIO_GET_AF(config);
	
	/* Get MMIO struct */
	stm32f4_mmio_gpio_t * mmio = STM32F4_MMIO_GPIO[port];

	if(pin < 8)
		REG_WRITE_SH(mmio->AFRL, STM32F4_GPIO_AFR_SH(pin),
			     STM32F4_GPIO_AF_MASK, af);
	else
		REG_WRITE_SH(mmio->AFRH, STM32F4_GPIO_AFR_SH(pin),
			     STM32F4_GPIO_AF_MASK, af);

	REG_WRITE_SH(mmio->OTYPER, STM32F4_GPIO_OTYPER_SH(pin),
		     STM32F4_GPIO_OUTP_MASK, otype);
	REG_WRITE_SH(mmio->OSPEEDR, STM32F4_GPIO_OSPEEDR_SH(pin),
		     STM32F4_GPIO_SPEED_MASK, speed);
	REG_WRITE_SH(mmio->PUPDR, STM32F4_GPIO_PUPDR_SH(pin),
		     STM32F4_GPIO_PULL_MASK, pull);
	REG_WRITE_SH(mmio->MODER, STM32F4_GPIO_MODER_SH(pin),
		     STM32F4_GPIO_MODE_MASK, mode);
}

void stm32f4_gpio_configure_batch(uint32_t *config, size_t count)
{
	for(size_t i = 0; i < count; ++i)
		stm32f4_gpio_configure(config[i]);
}

void stm32f4_gpio_set(unsigned int port, unsigned int pin, unsigned int val)
{
	stm32f4_mmio_gpio_t * mmio = STM32F4_MMIO_GPIO[port];
	
	REG_WRITE_SH(mmio->ODR, STM32F4_GPIO_DR_SH(pin), 0x1, val ? 1 : 0);
}

int stm32f4_gpio_get(unsigned int port, unsigned int pin)
{
	stm32f4_mmio_gpio_t * mmio = STM32F4_MMIO_GPIO[port];
	
	return ((mmio->IDR >> STM32F4_GPIO_DR_SH(pin)) & 0x1);
}
