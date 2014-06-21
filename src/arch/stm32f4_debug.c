/*
 * In-memory circular buffer-based reporting
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

#define CONFIG_MCBDIAG_BUFSIZE 1024

__attribute__((section(".bkpsram")))
char _stm_mcbdiag_buffer[CONFIG_MCBDIAG_BUFSIZE];

void _stm_mcbdiag_init()
{
	/* Gain access to backup domain (backup SRAM) */
	
}