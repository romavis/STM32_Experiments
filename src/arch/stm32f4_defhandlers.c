/*
 * Default exception handlers for STM32F4
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

__attribute__((interrupt, used))
void _hfault_default_handler();

__attribute__((interrupt, used))
void _mmfault_default_handler();

__attribute__((interrupt, used))
void _bfault_default_handler();

__attribute__((interrupt, used))
void _ufault_default_handler();

__attribute__((interrupt, used))
void _irq_default_handler();

void _hfault_default_handler()
{
	while(1);
	return;
}

void _mmfault_default_handler()
{
	while(1);
	return;
}

void _bfault_default_handler()
{
	while(1);
	return;
}

void _ufault_default_handler()
{
	while(1);
	return;
}

void _irq_default_handler()
{
	while(1);
	return;
}
