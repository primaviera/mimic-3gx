#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

export TOPDIR ?= $(CURDIR)
include $(DEVKITARM)/3ds_rules

CTRPFLIB ?= $(DEVKITPRO)/libctrpf

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# DATA is a list of directories containing data files
# INCLUDES is a list of directories containing header files
# PSF is a file containing information about the plugin to build (for 3gxtool)
#---------------------------------------------------------------------------------
TARGET := $(notdir $(TOPDIR))
BUILD := build
SOURCES := src src/patches src/patches/skills external/inih
DATA := data
INCLUDES := include include/standalone external
PSF := $(notdir $(TOPDIR)).plgInfo

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
ARCH := -march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft

CFLAGS := -mword-relocations -ffunction-sections -fdata-sections \
	-fno-strict-aliasing -fomit-frame-pointer $(ARCH)

CFLAGS += $(INCLUDE) -D__3DS__ $(DEFINES) -Wall -Werror

CXXFLAGS := $(CFLAGS) -fno-rtti -fno-exceptions -std=c++17

ASFLAGS := $(ARCH)
LDFLAGS := -T $(TOPDIR)/3gx.ld $(ARCH) \
	-Wl,--strip-discarded,--strip-debug,--gc-sections,--section-start,.text=0x07000100

LIBS := -lm
LIBDIRS := $(CTRPFLIB) $(CTRULIB) $(PORTLIBS)

ifneq ($(DEBUG),)
CXXFLAGS += -DDEBUG -g -O2
CFLAGS += -DDEBUG -g -O2
LDFLAGS += -g -O2
LIBS += -lctrpfd -lctrud
ASFLAGS += -g
else
CXXFLAGS += -Os
CFLAGS += -Os
LDFLAGS += -Os
LIBS += -lctrpf -lctru
endif

ifneq ($(TESTING),)
CXXFLAGS += -DTESTING
CFLAGS += -DTESTING
endif

#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------
export OUTPUT := $(CURDIR)/$(TARGET)

export VPATH := $(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
	$(foreach dir,$(DATA),$(CURDIR)/$(dir))

export DEPSDIR := $(CURDIR)/$(BUILD)

CFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
BINFILES := $(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))

export LD := $(CXX)

export OFILES_SOURCES := $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)

export OFILES_BIN := $(addsuffix .o,$(BINFILES))

export OFILES := $(OFILES_BIN) $(OFILES_SOURCES)

export HFILES := $(addsuffix .h,$(subst .,_,$(BINFILES)))

export INCLUDE := $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
	$(foreach dir,$(LIBDIRS),-I$(dir)/include) -I$(CURDIR)/$(BUILD)

export LIBPATHS := $(foreach dir,$(LIBDIRS),-L $(dir)/lib)

.PHONY: $(BUILD) clean re all

#---------------------------------------------------------------------------------
all: $(BUILD)

$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------
clean:
	rm -rf build *.elf *.3gx

#---------------------------------------------------------------------------------

else

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------

DEPENDS := $(OFILES:.o=.d)

$(OUTPUT).3gx : $(OUTPUT).elf
$(OFILES_SOURCES) : $(HFILES)
$(OUTPUT).elf : $(OFILES)

%.3gx: %.elf
	@echo creating $(notdir $@)
	@3gxtool -s $^ $(TOPDIR)/$(PSF) $@

#---------------------------------------------------------------------------------
# you need a rule like this for each extension you use as binary data
#---------------------------------------------------------------------------------
%.bin.o %_bin.h : %.bin
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

#---------------------------------------------------------------------------------

-include $(DEPENDS)

#---------------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------------
