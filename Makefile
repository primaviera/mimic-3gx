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
# SOURCES is a list of directories containing source code
# DATA is a list of directories containing data files
# INCLUDES is a list of directories containing header files
# PSF is a file containing information about the plugin to build
#---------------------------------------------------------------------------------
TARGET		:=	$(notdir $(CURDIR))
SOURCES 	:=	src src/hacks
DATA		:=	data
INCLUDES	:=	include
PSF 		:=	$(notdir $(TOPDIR)).plgInfo

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
ARCH		:=	-march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft

CFLAGS		:=	-mword-relocations -ffunction-sections -fdata-sections -fno-strict-aliasing \
			$(ARCH) $(BUILD_FLAGS) $(G)

CFLAGS		+=	$(INCLUDE) -D__3DS__ $(DEFINES) -Wall -Werror

CXXFLAGS	:=	$(CFLAGS) -fno-rtti -fno-exceptions -std=c++17

ASFLAGS		:=	$(ARCH) $(G)
LDFLAGS		:=	-T $(TOPDIR)/3gx.ld $(ARCH) -Os -Wl,$(WL)--gc-sections,--section-start,.text=0x07000100

LIBS 		:=	$(BUILD_LIBS) -lm
LIBDIRS		:=	$(CTRPFLIB) $(CTRULIB) $(PORTLIBS)

#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------
export VPATH	:= 	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
		   	$(foreach dir,$(DATA),$(CURDIR)/$(dir))

export DEPSDIR	:=	$(CURDIR)/$(BUILD)

CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
BINFILES	:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))

export LD 	:= 	$(CXX)

export OFILES_SOURCES 	:=	$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)

export OFILES_BIN	:=	$(addsuffix .o,$(BINFILES))

export OFILES 	:= 	$(OFILES_BIN) $(OFILES_SOURCES)

export HFILES	:=	$(addsuffix .h,$(subst .,_,$(BINFILES)))

export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
			$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
			-I$(CURDIR)/$(BUILD)

export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L $(dir)/lib)

.PHONY: $(BUILD) clean re all

#---------------------------------------------------------------------------------
all: $(TARGET).3gx

release:
	@[ -d $@ ] || mkdir -p $@

debug:
	@[ -d $@ ] || mkdir -p $@

$(TARGET).3gx : release
	@$(MAKE) BUILD=release OUTPUT=$(CURDIR)/$@ BUILD_LIBS="-lctrpf -lctru" WL=--strip-discarded,--strip-debug, \
	BUILD_CFLAGS="-DNDEBUG=1 -O2 -fomit-frame-pointer" DEPSDIR=$(CURDIR)/release \
	--no-print-directory --jobs=$(shell nproc) -C release -f $(CURDIR)/Makefile

$(TARGET)-debug.3gx : debug
	@$(MAKE) BUILD=debug OUTPUT=$(CURDIR)/$@ BUILD_LIBS="-lctrpfd -lctrud" BUILD_CFLAGS="-DDEBUG=1 -Og" G=-g \
	DEPSDIR=$(CURDIR)/debug --no-print-directory --jobs=$(shell nproc) -C debug -f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------
clean:
	rm -fr release debug *.elf *.3gx

re: clean all

#---------------------------------------------------------------------------------

else

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------

DEPENDS	:=	$(OFILES:.o=.d)

$(OUTPUT) : $(basename $(OUTPUT)).elf
$(OFILES_SOURCES) : $(HFILES)
$(basename $(OUTPUT)).elf : $(OFILES)

#---------------------------------------------------------------------------------
# you need a rule like this for each extension you use as binary data
#---------------------------------------------------------------------------------
%.bin.o	:	%.bin
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

#---------------------------------------------------------------------------------
%.3gx: %.elf
	@echo creating $(notdir $@)
	@3gxtool -s $^ $(TOPDIR)/$(PSF) $@

-include $(DEPENDS)

#---------------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------------
