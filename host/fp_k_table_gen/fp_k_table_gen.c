/*
 * Program that generates precomputed tables for float-to-string conversion
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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <libgen.h>

#include <printf/fp_k_table.h>
#include <printf/fp_k_calc.h>

int fp_k_table_gen_d(FILE *ofile, int verbose)
{
	uint64_t k;
	int exp;
	for(unsigned idx = 0; idx < FP_TABLE_SZ_D; ++idx) {
		exp = FP_IDX_TO_EXP(idx, D);
		k = fp_calc_k_64b(exp);
		if(verbose)
			printf("Gen64: idx %8d, exp %8d, k %#.16llx\n",
			       idx, exp, k);
		if(fwrite(&k, sizeof(uint64_t), 1, ofile) < 1)
			return -1;
	}
	return 0;
}

int fp_k_table_gen_f(FILE *ofile, int verbose)
{
	uint32_t k;
	int exp;
	for(unsigned idx = 0; idx < FP_TABLE_SZ_F; ++idx) {
		exp = FP_IDX_TO_EXP(idx, F);
		k = fp_calc_k_32b(exp);
		if(verbose)
			printf("Gen32: idx %8d, exp %8d, k %#.8x\n",
			       idx, exp, k);
		if(fwrite(&k, sizeof(uint32_t), 1, ofile) < 1)
			return -1;
	}
	return 0;
}

void show_help(const char *argv0)
{
	printf("%s -f <filename> -d <filename> [-v]\n"
	       "\t-f\tgenerate floats K table\n"
	       "\t-d\tgenerate double K table\n"
	       "\t-v\tbe verbose: print tables to stdout\n",
	       argv0);
}

int main(int argc, char **argv)
{
	char *filename_float = 0;
	char *filename_double = 0;
	char *argv0;
	FILE *ofile;
	int opt;
	int verbose = 0;

	argv0 = basename(argv[0]);

	while((opt = getopt(argc, argv, "f:d:hv")) != -1) {
		switch(opt) {
		case 'f':
			filename_float = optarg;
			break;
		case 'd':
			filename_double = optarg;
			break;
		case 'v':
			verbose = 1;
			break;
		case 'h':
			show_help(argv0);
			return EXIT_SUCCESS;
		default:
			printf("%s: Unknown option supplied (-%c)\n", argv0,
			       optopt);
			return EXIT_FAILURE;
		}
	}

	if(!filename_float && !filename_double) {
		show_help(argv0);
		return EXIT_FAILURE;
	}

	if(filename_float) {
		ofile = fopen(filename_float, "w+");
		if(!ofile) {
			perror("open_file_float");
			return EXIT_FAILURE;
		}
		errno = 0;
		if(fp_k_table_gen_f(ofile, verbose)) {
			perror("fp_k_table_gen_f");
			return EXIT_FAILURE;
		}
		fclose(ofile);
	}

	if(filename_double) {
		ofile = fopen(filename_double, "w+");
		if(!ofile) {
			perror("open_file_double");
			return EXIT_FAILURE;
		}
		errno = 0;
		if(fp_k_table_gen_d(ofile, verbose)) {
			perror("fp_k_table_gen_d");
			return EXIT_FAILURE;
		}
		fclose(ofile);
	}

	return EXIT_SUCCESS;
}
