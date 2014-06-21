#
# Configuration file
# Can be included by GNU Make and translated in C header by GNU sed:
#  sed -n '/^[[:blank:]]*#.*/d;/^[[:blank:]]*$/d;/^[[:blank:]]*\(CONFIG\w\+\)
#  [[:blank:]]*:=[[:blank:]]*\(\w\+\)[[:blank:]]*$/{s,,#define \1 \2,;p;d};q 1'
#
# Syntax is limited by sed script. Only following lines are allowed:
# 1. "#<something>" - comment, truncated during processing
# 2. "CONFIG_<x> := <y>" - configuration parameter, translated into
#    "#define CONFIG_<x> <y>"
# If sed finds malformed string, it fails with non-zero error code
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

#Linker configuration

CONFIG_LD_HEAP_CCM 		:= y
# CONFIG_LD_HEAP_SRAM 		:= y
CONFIG_LD_MSTACK_SIZE 		:= 2048
# CONFIG_LD_PSTACK_SIZE 	:= 1024
# CONFIG_LD_STACK_DUAL 		:= y
# CONFIG_LD_RELOCATE_VT_SRAM 	:= y

#Printf configuration

CONFIG_PRINTF_FLOAT_KTABLE	:= y
CONFIG_PRINTF_DOUBLE_KTABLE	:= y
