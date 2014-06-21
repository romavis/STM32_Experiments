#
# build.mk for building precomputed float-to-string coefficient tables
#
# Copyright (C) 2014 Roman Dobrodiy <ztcoils _d@g_ gmail.com>
# 
# All rights reserved. This program and the accompanying materials
# are made available under the terms of the GNU Lesser General Public License
# (LGPL) version 2.1 which accompanies this distribution, and is available at
# http://www.gnu.org/licenses/lgpl-2.1.html
# 
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# Lesser General Public License for more details.

$(warning MAKESTACK: $(MAKESTACK))
$(warning MAKEDIR: $(MAKEDIR))

$(call inc_subdir, src)
$(call inc_subdir, host)

obj += fp_k_table_d.o
obj += fp_k_table_f.o

#Rules to get these objects

%/fp_k_table_d.o : $(HOST)/fp_k_table_gen
	$(HOST)/fp_k_table_gen -d $@.tmp
	$(OBJCOPY) -I binary -O elf32-littlearm -B arm\
		--rename-section .data=.fp_k_table_d,alloc,contents,load,readonly\
		$@.tmp $@
	rm -f $@.tmp

%/fp_k_table_f.o : $(HOST)/fp_k_table_gen
	$(HOST)/fp_k_table_gen -f $@.tmp
	$(OBJCOPY) -I binary -O elf32-littlearm -B arm\
		--rename-section .data=.fp_k_table_f,alloc,contents,load,readonly\
		$@.tmp $@
	rm -f $@.tmp
