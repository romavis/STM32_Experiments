#ifndef STM32F4_GPIO_H
#define STM32F4_GPIO_H

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
#include <stddef.h>

#include <arch/stm32f4_mmiomap.h>
#include <arch/stm32f4_mmio_gpio.h>


/*
 * Pin configuration word (uint32_t)
 * 
 * 31               19   15     13      11             10     8       4        0
 *  ___________________________________________________________________________
 * |                 |    |      |       |             |      |       |        |
 * |    RESERVED     | AF | Pull | Speed | Output type | Mode | Pin # | Port # |
 * |_________________|____|______|_______|_____________|______|_______|________|
 *        13 bits      4b    2b     2b        1 bit       2b      4b      4b
 * 
 * AF:
 *  0..15 - AF number
 * Pull:
 *  00 - none
 *  01 - pull up
 *  10 - pull down
 * Speed:
 *  00 - slow
 *  01 - medium
 *  10 - fast
 *  11 - high
 * Output type:
 *  0 - normal (push-pull)
 *  1 - open drain
 * Mode:
 *  00 - general purpose input
 *  01 - general purpose output
 *  10 - AF controlled
 *  11 - analog
 */

#define	STM32F4_GPIO_PORT_MASK	0xF
#define	STM32F4_GPIO_PIN_MASK	0xF
#define	STM32F4_GPIO_MODE_MASK	0x3
#define	STM32F4_GPIO_OUTP_MASK	0x1
#define	STM32F4_GPIO_SPEED_MASK	0x3
#define	STM32F4_GPIO_PULL_MASK	0x3
#define	STM32F4_GPIO_AF_MASK	0xF

#define STM32F4_GPIO_PORT(x)	((x) & 0xF)
#define STM32F4_GPIO_PIN(x)	(((x) & 0xF) << 4)
#define STM32F4_GPIO_MODE(x)	(((x) & 0x3) << 8)
#define STM32F4_GPIO_OUTP(x)	(((x) & 0x1) << 10)
#define STM32F4_GPIO_SPEED(x)	(((x) & 0x3) << 11)
#define STM32F4_GPIO_PULL(x)	(((x) & 0x3) << 13)
#define STM32F4_GPIO_AF(x)	(((x) & 0xF) << 15)

#define STM32F4_GPIO_GET_PORT(x)	((x) & 0xF)
#define STM32F4_GPIO_GET_PIN(x)		(((x) >> 4) & 0xF)
#define STM32F4_GPIO_GET_MODE(x)	(((x) >> 8) & 0x3)
#define STM32F4_GPIO_GET_OUTP(x)	(((x) >> 10) & 0x1)
#define STM32F4_GPIO_GET_SPEED(x)	(((x) >> 11) & 0x3)
#define STM32F4_GPIO_GET_PULL(x)	(((x) >> 13) & 0x3)
#define STM32F4_GPIO_GET_AF(x)		(((x) >> 15) & 0xF)

#define	STM32F4_GPIO_PORTA	0
#define	STM32F4_GPIO_PORTB	1
#define	STM32F4_GPIO_PORTC	2
#define	STM32F4_GPIO_PORTD	3
#define	STM32F4_GPIO_PORTE	4
#define	STM32F4_GPIO_PORTF	5
#define	STM32F4_GPIO_PORTG	6
#define	STM32F4_GPIO_PORTH	7
#define	STM32F4_GPIO_PORTI	8
#define	STM32F4_GPIO_PORTJ	9
#define	STM32F4_GPIO_PORTK	10

#define	STM32F4_GPIO_AF_AF0	STM32F4_GPIO_AF(0)
#define	STM32F4_GPIO_AF_AF1	STM32F4_GPIO_AF(1)
#define	STM32F4_GPIO_AF_AF2	STM32F4_GPIO_AF(2)
#define	STM32F4_GPIO_AF_AF3	STM32F4_GPIO_AF(3)
#define	STM32F4_GPIO_AF_AF4	STM32F4_GPIO_AF(4)
#define	STM32F4_GPIO_AF_AF5	STM32F4_GPIO_AF(5)
#define	STM32F4_GPIO_AF_AF6	STM32F4_GPIO_AF(6)
#define	STM32F4_GPIO_AF_AF7	STM32F4_GPIO_AF(7)
#define	STM32F4_GPIO_AF_AF8	STM32F4_GPIO_AF(8)
#define	STM32F4_GPIO_AF_AF9	STM32F4_GPIO_AF(9)
#define	STM32F4_GPIO_AF_AF10	STM32F4_GPIO_AF(10)
#define	STM32F4_GPIO_AF_AF11	STM32F4_GPIO_AF(11)
#define	STM32F4_GPIO_AF_AF12	STM32F4_GPIO_AF(12)
#define	STM32F4_GPIO_AF_AF13	STM32F4_GPIO_AF(13)
#define	STM32F4_GPIO_AF_AF14	STM32F4_GPIO_AF(14)
#define	STM32F4_GPIO_AF_AF15	STM32F4_GPIO_AF(15)

#define	STM32F4_GPIO_MODE_IN	STM32F4_GPIO_MODE(0x0)
#define	STM32F4_GPIO_MODE_OUT	STM32F4_GPIO_MODE(0x1)
#define	STM32F4_GPIO_MODE_AF	STM32F4_GPIO_MODE(0x2)
#define	STM32F4_GPIO_MODE_ANALOG STM32F4_GPIO_MODE(0x3)

#define	STM32F4_GPIO_OUTP_PP	STM32F4_GPIO_OUTTYPE(0x0)
#define	STM32F4_GPIO_OUTP_OD	STM32F4_GPIO_OUTTYPE(0x1)

#define	STM32F4_GPIO_SPEED_LOW	STM32F4_GPIO_SPEED(0x0)
#define	STM32F4_GPIO_SPEED_MEDIUM STM32F4_GPIO_SPEED(0x1)
#define	STM32F4_GPIO_SPEED_FAST	STM32F4_GPIO_SPEED(0x2)
#define	STM32F4_GPIO_SPEED_HIGH	STM32F4_GPIO_SPEED(0x3)

#define	STM32F4_GPIO_PULL_NONE	STM32F4_GPIO_PULL(0x0)
#define	STM32F4_GPIO_PULL_UP	STM32F4_GPIO_PULL(0x1)
#define	STM32F4_GPIO_PULL_DOWN	STM32F4_GPIO_PULL(0x2)

#define STM32F4_GPIO_CONF_AF(port, pin, af) (\
	STM32F4_GPIO_PORT##port |\
	STM32F4_GPIO_PIN(pin) |\
	STM32F4_GPIO_AF_##af) |\
	STM32F4_GPIO_MODE_AF)

#define STM32F4_GPIO_CONF_MODE(port, pin, mode) (\
	STM32F4_GPIO_PORT##port |\
	STM32F4_GPIO_PIN(pin) |\
	STM32F4_GPIO_MODE_##mode))

#define STM32F4_GPIO_CONF_OUT(port, pin) (\
	STM32F4_GPIO_PORT##port |\
	STM32F4_GPIO_PIN(pin) |\
	STM32F4_GPIO_MODE_OUT)	

#define STM32F4_GPIO_CONF_IN(port, pin) (\
	STM32F4_GPIO_PORT##port |\
	STM32F4_GPIO_PIN(pin) |\
	STM32F4_GPIO_MODE_IN)

#define STM32F4_GPIO_CONF_ANALOG(port, pin) (\
	STM32F4_GPIO_PORT##port |\
	STM32F4_GPIO_PIN(pin) |\
	STM32F4_GPIO_MODE_ANALOG)

void stm32f4_gpio_configure(uint32_t config);
void stm32f4_gpio_configure_batch(uint32_t *config, size_t count);

void stm32f4_gpio_set(unsigned int port, unsigned int pin, unsigned int val);
int stm32f4_gpio_get(unsigned int port, unsigned int pin);

#endif /* STM32F4_GPIO_H */
