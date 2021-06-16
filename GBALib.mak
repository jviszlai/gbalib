GCC_VERSION = $(shell arm-none-eabi-gcc -dumpversion)
LINKSCRIPT_DIR = $(GBALIB_DIR)

CROSS   := arm-none-eabi-
AS      := $(CROSS)as
CC      := $(CROSS)gcc
LD 		:= $(CROSS)g++
OBJCOPY := $(CROSS)objcopy

ARMINC = /usr/arm-none-eabi/include
ARMLIB = /usr/arm-none-eabi/lib
GCCLIB = /usr/lib/gcc/arm-none-eabi/$(GCC_VERSION)

# Tools and flags.
CC = $(CROSS)gcc
CCPP = $(CROSS)g++
CFLAGS = -mthumb-interwork -fomit-frame-pointer -mcpu=arm7tdmi -ffast-math -fno-exceptions -g
# added "-ffixed-r14" and "-mlong-calls" below to work around compiler bugs
THUMB = -mthumb -O3 -ffixed-r14 -funroll-loops
ARM = -marm -Os -mlong-calls
DEFAULT = $(THUMB)
# DEFAULT was $(THUMB)

AS = $(CROSS)as
ASFLAGS = -mthumb-interwork


# This rule builds a .h file of all your raw input file exports.
# So this creates a file (say) 'symbol.h' which has lines like (say):
# extern const u8 _binary_bob_raw_start[];
SYMBOLS = symbols.h
SYMBOL_PREFIX = "extern const u8 _binary_"
SYMBOL_SUFFIX1 = "_start[];"
SYMBOL_SUFFIX2 = "_end[];"

AAS_DIR = $(GBALIB_DIR)/src/apex-audio-system

# Library and include paths.
LIBS = -L$(AAS_DIR)/build/aas/lib -lAAS
INCLUDES = -I$(AAS_DIR)/build/aas/include -I$(GBALIB_DIR)/include

# Mapping & data objects for cursed solution
SPRITE_OBJS = $(BUILD_DIR)/sprite_data.o $(BUILD_DIR)/sprite_map.o
BACKGROUND_OBJS = $(BUILD_DIR)/background_data.o $(BUILD_DIR)/background_map.o
SOUND_OBJS = $(BUILD_DIR)/sound_map.o

# Files you want to go in ROM (AAS_Data.o must go first)
SRC = $(SPRITE_OBJS) $(BACKGROUND_OBJS) $(SOUND_OBJS)

MAP = $(BUILD_DIR)/map.out
LDFLAGS = -mthumb-interwork -Xlinker -Map $(MAP) -nostartfiles -T$(LINKSCRIPT_DIR)/lnkscript


# Rule to build raw files into .o files, noting their exported symbols.
%.o: %.raw
	@$(CROSS)objcopy -B arm -I binary -O elf32-little $< temp.o 2> /dev/null
	@$(CROSS)ld -T convert.ls temp.o -o $@
	@interflip -mthumb-interwork $@
	@echo $(CROSS)objcopy -I binary -O elf32-little $< $@
	@echo -n $(SYMBOL_PREFIX) >> $(SYMBOLS)
	@echo -n "$<" | tr "[:punct:]" "_" >> $(SYMBOLS)
	@echo $(SYMBOL_SUFFIX1) >> $(SYMBOLS)
	@echo -n $(SYMBOL_PREFIX) >> $(SYMBOLS)
	@echo -n "$<" | tr "[:punct:]" "_" >> $(SYMBOLS)
	@echo $(SYMBOL_SUFFIX2) >> $(SYMBOLS)

%.o: %.c 
	$(CC) $(DEFAULT) $(INCLUDES) $(CFLAGS) -c $< -o $@

%.text.iwram.o: %.o
	cp $< $@

CONV2AAS_FILE = conv2aas

$(BUILD_DIR)/AAS_Data.o: sound_map 
	$(AAS_DIR)/build/conv2aas/$(CONV2AAS_FILE) $(BUILD_DIR)/AAS_Data
	$(AS) $(ASFLAGS) -o $@ AAS_Data.s
	@mv AAS_Data.s $(BUILD_DIR)
	@mv AAS_Data.h $(BUILD_DIR)

$(BUILD_DIR)/$(NAME): $(GBALIB_DIR)/build/crt/crt0.o $(BUILD_DIR)/AAS_Data.o $(SRC) $(GBALIB_OBJS) $(GAME_OBJS)
	touch $(MAP)
	$(LD) $(LDFLAGS) -o $@ $(GBALIB_DIR)/build/crt/crt0.o $(BUILD_DIR)/AAS_Data.o $(SRC) $(GBALIB_OBJS) $(LIBS) $(GAME_OBJS)
	$(CROSS)size $@

$(TARGET): $(BUILD_DIR)/$(NAME)
	$(CROSS)objcopy -v -O binary $< $@
