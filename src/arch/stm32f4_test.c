/*
 * Sample C code for testing
 *
 * Copyright (C) 2014 Roman Dobrodiy <ztcoils _d@g_ gmail.com>
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
#include <stdarg.h>
#include <stdint.h>

#include <arch/stm32f4_gpio.h>
#include <util.h>
#include <printf/of_printf.h>
#include <memcpy.h>

char out_string[512];

volatile double testvar;

static void test_outpf(void *arg, const char *str, size_t len)
{
	char **ptr = (char**) arg;
	while(*str != '\0' && len > 0) {
		*(*ptr)++ = *str++;
		len--;
	}
	**ptr = '\0';
}

static uint32_t gpio_config [] = {
	STM32F4_GPIO_CONF_OUT(A, 4),
	STM32F4_GPIO_CONF_OUT(A, 5)
};

void main(void)
{
	char *outp_string_ptr = out_string;
	
	/* Enable coprocessor */
	volatile int *CPACR = 0xE000ED88;
	*CPACR |= 0x00F00000;
	__asm__("dsb");
	__asm__("isb");
	
	of_printf(test_outpf, &outp_string_ptr, "This is string: %s!\n", "yep, string");
	of_printf(test_outpf, &outp_string_ptr, "Another one: %s..\n", "ehe. another");
	of_printf(test_outpf, &outp_string_ptr, "Numbro!: %#25.16x\n", 0xDEADBEEF);
	of_printf(test_outpf, &outp_string_ptr, "Float: %e\n", (float) 1.3);
	of_printf(test_outpf, &outp_string_ptr, "Double: %le "
		  " %le %le %le\n", 1.3, 1e-47, 1e205, 1e-205);
	
	stm32f4_gpio_configure_batch(gpio_config, ARRAY_LEN(gpio_config));
	stm32f4_gpio_set(STM32F4_GPIO_PORTA, 4, 0);
	stm32f4_gpio_set(STM32F4_GPIO_PORTA, 4, 1);
	
	while(1);
}
