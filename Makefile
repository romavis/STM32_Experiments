#
# Core Makefile in build system. Use with GNU Make
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


$(info --Makefile entry--)
######COMMANDS######

#Host toolchain
HOST-CC := $(CC)
HOST-GCC := $(GCC)

#Target toolchain
TC_PREFIX := arm-cortex_m4-eabi

CC := $(TC_PREFIX)-cc
LD := $(TC_PREFIX)-gcc
AS := $(TC_PREFIX)-as
OBJCOPY := $(TC_PREFIX)-objcopy
SIZE := $(TC_PREFIX)-size

#Default commands
MKDIR ?= mkdir
SED ?= sed
MV ?= mv

######VARIABLES######
#Initial values
INCDIR :=
LIBS :=
CFLAGS :=
LDFLAGS :=
HOST-CFLAGS :=

#Default buildroot directory
BUILDROOT ?= ./build

#Include dirs
INCDIR += ./include

#Libs
LIBS += -lgcc

#Executable image
IMAGE := out/image

#Memory map
MMAP := out/map

#Linker script
LDS := link.lds

#Configuration file prefix
CONFIG_MK := ./config.mk

#Configuration header
CONFIG_H := $(BUILDROOT)/$(strip $(subst .mk,.h,$(CONFIG_MK)))

#Host progs directory
HOSTPREFIX := out/host

#Host programs output dir
HOST := $(BUILDROOT)/$(strip $(HOSTPREFIX))

#GCC &Co flags
CFLAGS += -nostdinc -nostdlib -iwithprefix include -ffreestanding
CFLAGS += -std=c99 -fdata-sections -ffunction-sections
CFLAGS += -O0 -g

HOST-CFLAGS += -std=c99 -O2

CFLAGS += -include $(CONFIG_H)
HOST-CFLAGS += -include $(CONFIG_H)

CFLAGS += $(foreach i, $(INCDIR), -I $i)
HOST-CFLAGS += $(foreach i, $(INCDIR), -I $i)

LDFLAGS += -Os -Wl,--gc-sections -nostdlib $(LIBS)

#MAKESTACK: stack of included makefiles
#MAKEFILE: always contains current makefile path
#MAKEDIR: always contains current makefile dir
#OBJDIR: contains directory to which object files from MAKEDIR would be placed

MAKESTACK := ./$(lastword $(MAKEFILE_LIST))
MAKEFILE = $(lastword $(MAKESTACK))
MAKEDIR = $(dir $(MAKEFILE))
OBJDIR = $(BUILDROOT)/$(MAKEDIR)

#$(call save_context, variable), then eval
define save_context
	$(strip $1)_$(MAKEFILE) := $$($(strip $1))
	$(strip $1) :=
endef

#$(call restore_context, variable), then eval
define restore_context
	$(strip $1) := $$($(strip $1)_$(MAKEFILE)) $$($(strip $1))
	undefine $(strip $1)_$(MAKEFILE)
endef

define inc_subdir__int
	SMFILE := $(firstword $(wildcard $(MAKEDIR)/$1/build.mk\
					 $(MAKEDIR)/$1/Makefile\
					 $(MAKEDIR)/$1/makefile))
	$(call save_context, obj-c)
	$(call save_context, obj-as)
	$(call save_context, obj)
	MAKESTACK += $$(SMFILE)
	
	include $$(SMFILE)
	
	MAKESTACK := $$(subst $$(lastword $$(MAKESTACK)),,$$(MAKESTACK))
	obj-c := $$(foreach x, $$(strip $$(obj-c)), $1/$$x)
	obj-as := $$(foreach x, $$(strip $$(obj-as)), $1/$$x)
	obj := $$(foreach x, $$(strip $$(obj)), $1/$$x)
	$(call restore_context, obj-c)
	$(call restore_context, obj-as)
	$(call restore_context, obj)
endef

#$(call inc_subdir, directory)
#Processes makefile from subdirectory
define inc_subdir
	$(eval $(call inc_subdir__int,$(strip $1)))
endef

#Default target
all :

#Include configuration
include $(CONFIG_MK)

#Include pretty makefile
obj-c :=
obj-as :=
obj :=
host-progs :=
include build.mk

$(info --Hierarchical inclusion finished--)

#Sources list
src-c := $(subst .o,.c,$(obj-c))
src-as := $(subst .o,.S,$(obj-as))

#Object files to link
obj-c := $(foreach x, $(obj-c), $(BUILDROOT)/$x)
obj-as := $(foreach x, $(obj-as), $(BUILDROOT)/$x)
obj := $(foreach x, $(obj), $(BUILDROOT)/$x)

#Linked image filepath prefix
img := $(BUILDROOT)/$(strip $(IMAGE))

#Memory map file path
mmap := $(BUILDROOT)/$(strip $(MMAP))

#Linker script
lds := $(BUILDROOT)/$(strip $(LDS))

#Dependency makefiles...
depends-c := $(subst .o,.d,$(obj-c))
depends-as := $(subst .o,.d,$(obj-as))

######Host progs stuff######

#Dynamic rule generator
#$(call gen-host-rule, host-prog-name)
define gen-host-rule
$$(HOST)/$1 : $$(hostp-$1-obj-c) $(CONFIG_H) | $$(dtree)
	@echo "Linking host program $1"
	$$(HOST-CC) $$(HOST-CFLAGS) $$^ -o $$@
endef

#Generate list of host sources
host-src-c :=
$(foreach x,$(host-progs),\
  $(eval host-src-c +=$$(hostp-$x-obj-c)))
host-src-c := $(subst .o,.c,$(host-src-c))

#Prepend object files paths with BUILDROOT
$(foreach x,$(host-progs),\
  $(eval hostp-$x-obj-c :=$$(foreach y,\
  $$(hostp-$x-obj-c),$$(BUILDROOT)/$$y_host)))

#Concatenate list of host object files
host-obj-c :=
$(foreach x,$(host-progs),\
  $(eval host-obj-c +=$$(strip $$(hostp-$x-obj-c))))

#Concatenate list of host output binaries
host-bins :=
$(foreach x,$(host-progs),\
  $(eval host-bins +=$$(strip $$(HOST)/$x)))

#Host dependency files
host-depends-c := $(subst .o,.d,$(host-obj-c))

######Directory tree######
dtree := $(sort $(dir $(obj-c)) $(dir $(obj-as)) $(dir $(obj)))
dtree += $(sort $(dir $(host-obj-c)) $(dir $(host-bins)))
dtree += $(sort $(dir $(img)))
dtree := $(foreach x,$(dtree),$x/)

$(info Objs-C: $(obj-c))
$(info Objs-as: $(obj-as))
$(info Generic objs: $(obj))
$(info Depends-C: $(depends-c))
$(info Depends-as: $(depends-as))
$(info Image prefix: $(img))
$(info CFLAGS: $(CFLAGS))
$(info Host progs: $(host-progs))
$(info Host objs-C: $(host-obj-c))
$(info Host depends-C: $(host-depends-c))
$(info Host bins: $(host-bins))
$(info Host CFLAGS: $(HOST-CFLAGS))
$(info BUILDROOT dtree: $(dtree))

#Check if needed source files exist
$(foreach x,$(src-c) $(src-as) $(host-src-c),\
$(if $(wildcard $x),, $(error 'Source file doesn't exist: $x)))

$(info --Makefile prepaired--)

#PHONY targets
#Default target
all : image.elf image.hex

#Generate dependency makefiles
depends: $(depends-c) $(depends-as) $(host-depends-c)
	@echo "Dependency makefiles created"

#Create buildroot directory tree
buildroot_tree : | $(dtree)
	@echo "Buildroot directories created"

compile : $(obj-c) $(obj-as) $(obj)
	@echo "Object files created"

config : $(CONFIG_H)
	@echo "Configuration header generated"

image.elf : $(img).elf
	@echo "ELF binary linked at: $^"
	@echo "Size stats:"
	@$(SIZE) -A $^

image.hex : $(img).hex
	@echo "HEX binary crafted at: $^"
	@echo "Size stats:"
	@$(SIZE) -A $^

#Host progs
host.% : $(HOST)/%
	@echo "Built host program $* at: $^"

.PHONY: all buildroot_tree depends compile config image.% host.%

#Include auto-generated dependency information (and regenerate if necessary)
include $(depends-c) $(depends-as) $(host-depends-c)

######RULES######
#Generate config.h from config.mk
$(CONFIG_H) : $(CONFIG_MK) | $(dtree)
	@echo "/*" > $@.tmp
	@echo " * Auto-generated configuration header" >> $@.tmp
	@echo " * Do not edit!" >> $@.tmp
	@echo " * Regenerate using 'make config'" >> $@.tmp
	@echo " */" >> $@.tmp
	cat $(CONFIG_MK) |\
	sed -n '/^[[:blank:]]*#.*/d;/^[[:blank:]]*$$/d;/^[[:blank:]]*\(CONFIG\w\+\)[[:blank:]]*:=[[:blank:]]*\(\w\+\)[[:blank:]]*$$/{s,,#define \1 \2,;p;d};q 1' >>\
	$@.tmp
	mv $@.tmp $@

#Rule to preprocess .lds.S into .lds
$(lds): $(BUILDROOT)/%.lds : %.lds.S $(CONFIG_H) | $(dtree)
	$(CC) $(CFLAGS) $*.lds.S -P -E -o $@

#Rule to generate dependency files for C sources
$(depends-c): $(BUILDROOT)/%.d : %.c $(CONFIG_H) | $(dtree)
	set -o pipefail; \
	$(CC) $(CFLAGS) $*.c -MP -MM |\
	$(SED) 's,\($(basename $(notdir $@))\).o *:,$(dir $@)\1.o $(dir $@)\1.d:,' > $@.tmp
	$(MV) $@.tmp $@
$(depends-as): $(BUILDROOT)/%.d : %.S $(CONFIG_H) | $(dtree)
	set -o pipefail; \
	$(CC) $(CFLAGS) $*.S -MP -MM |\
	$(SED) 's,\($(basename $(notdir $@))\).o *:,$(dir $@)\1.o $(dir $@)\1.d:,' > $@.tmp
	$(MV) $@.tmp $@

#Rule to compile .c into .o
$(obj-c): $(BUILDROOT)/%.o : %.c $(CONFIG_H) | $(dtree)
	$(CC) $(CFLAGS) $*.c -c -o $@

#Rule to assemble .S into .o
$(obj-as): $(BUILDROOT)/%.o : %.S $(CONFIG_H) | $(dtree)
	$(CC) $(CFLAGS) $*.S -c -o $@

#Rule to link ELF
$(img).elf : $(obj-c) $(obj-as) $(obj) $(lds) | $(dtree)
	$(LD) -T $(lds) $(obj-c) $(obj-as) $(obj) $(LDFLAGS)\
		-Wl,-Map=$(mmap) -o $@

#Rule to make HEX from ELF
%.hex : %.elf
	$(OBJCOPY) -S -O ihex $^ $@

#Rule to create directories in buildroot tree
$(dtree) :
	$(MKDIR) -vp $@

######HOST RULES######

#Dependency generation
$(host-depends-c): $(BUILDROOT)/%.d_host : %.c $(CONFIG_H) | $(dtree)
	set -o pipefail; \
	$(HOST-CC) $(HOST-CFLAGS) $*.c -MP -M |\
	$(SED) 's,\($(basename $(notdir $@))\).o *:,$(dir $@)\1.o $(dir $@)\1.d:,' > $@.tmp
	$(MV) $@.tmp $@

#C compile rule
$(host-obj-c): $(BUILDROOT)/%.o_host : %.c $(CONFIG_H) | $(dtree)
	$(HOST-CC) $(HOST-CFLAGS) $*.c -c -o $@

#Autogenerate linkage rules
$(foreach x,$(host-progs),\
  $(eval $(call gen-host-rule,$(strip $x))))
