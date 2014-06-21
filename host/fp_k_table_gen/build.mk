#
# build.mk for host/fp_k_table_gen directory
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

hostp-fp_k_table_gen-obj-c := $(MAKEDIR)/fp_k_table_gen.o
hostp-fp_k_table_gen-obj-c += ./src/printf/fp_k_calc.o
