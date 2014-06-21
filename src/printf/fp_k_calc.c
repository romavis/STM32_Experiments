/*
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

#include <assert.h>
#include <printf/fp_k.h>
#include <printf/fp_k_calc.h>

uint64_t fp_calc_k_64b(int bin_exp)
{
	int e_saved;
	int dec_exp = 0;
	uint64_t ret = 0x8000000000000000LL;
	uint32_t rem = 0;
	const uint64_t hb1_mask = 0x8000000000000000LL;
	const uint64_t m5_mask = 0x3333333333333333LL;
	const uint32_t rem2_thr = 0x10000000;	/* rem>=1.0 */
	const uint32_t rem5_thr = 0x28000000;	/* rem>=2.5 */

	if(!bin_exp)
		return 0;

	e_saved = bin_exp;
	bin_exp++;

	if(bin_exp < 0) {
		while(bin_exp < 0) {
			if(ret > m5_mask) {
				rem = (rem >>1) + ((uint32_t) (ret & 1) << 28);
				ret>>=1;
			} else {
				ret*= 5;
				rem*= 5;
				ret+= rem >> 29;
				rem&= 0x1FFFFFFF;
				dec_exp++;
			}
			bin_exp++;
		}
		if(rem >= rem2_thr)
			++ret;		/* Round up */
	} else if (bin_exp > 0) {
		while(bin_exp > 0) {
			if(ret & hb1_mask) {
				rem = (rem / 5) + ((uint32_t) (ret % 5) << 28);
				ret/= 5;
				dec_exp--;
			} else {
				ret<<=1;
				rem<<=1;
				ret+= rem / 0x50000000;
				rem = rem % 0x50000000;
			}
			bin_exp--;
		}
		if(rem >= rem5_thr)
			++ret;		/* Round up */
	}
	assert(dec_exp == FP_E_TO_D(e_saved));
	return ret;
}

uint32_t fp_calc_k_32b(int bin_exp)
{
	int e_saved;
	int dec_exp = 0;
	uint32_t ret = 0x80000000;
	uint32_t rem = 0;
	const uint32_t hb1_mask = 0x80000000;
	const uint32_t m5_mask = 0x33333333;
	const uint32_t rem2_thr = 0x10000000;	/* rem>=1.0 */
	const uint32_t rem5_thr = 0x28000000;	/* rem>=2.5 */

	if(!bin_exp)
		return 0;

	e_saved = bin_exp;
	bin_exp++;

	if(bin_exp < 0) {
		while(bin_exp < 0) {
			if(ret > m5_mask) {
				rem = (rem >>1) + ((uint32_t) (ret & 1) << 28);
				ret>>=1;
			} else {
				ret*= 5;
				rem*= 5;
				ret+= rem >> 29;
				rem&= 0x1FFFFFFF;
				dec_exp++;
			}
			bin_exp++;
		}
		if(rem >= rem2_thr)
			++ret;		/* Round up */
	} else if (bin_exp > 0) {
		while(bin_exp > 0) {
			if(ret & hb1_mask) {
				rem = (rem / 5) + ((uint32_t) (ret % 5) << 28);
				ret/= 5;
				dec_exp--;
			} else {
				ret<<=1;
				rem<<=1;
				ret+= rem / 0x50000000;
				rem = rem % 0x50000000;
			}
			bin_exp--;
		}
		if(rem >= rem5_thr)
			++ret;		/* Round up */
	}
	assert(dec_exp == FP_E_TO_D(e_saved));
	return ret;
}
