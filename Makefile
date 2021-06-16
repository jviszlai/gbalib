GCC_VERSION = $(shell arm-none-eabi-gcc -dumpversion)

SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))

TARGET_LIB = $(BUILD_DIR)/libGBA.a

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
CFLAGS = -mthumb-interwork -fomit-frame-pointer -mcpu=arm7tdmi -ffast-math -fno-exceptions -Wno-discarded-qualifiers -g
# added "-ffixed-r14" and "-mlong-calls" below to work around compiler bugs
THUMB = -mthumb -O3 -ffixed-r14 -funroll-loops
ARM = -marm -Os -mlong-calls
DEFAULT = $(THUMB)
# DEFAULT was $(THUMB)

AS = $(CROSS)as
ASFLAGS = -mthumb-interwork

AAS_DIR = $(SRC_DIR)/apex-audio-system

# Library and include paths.
LIBS = -L$(AAS_DIR)/build/aas/lib -lAAS
INCLUDES = -I$(AAS_DIR)/build/aas/include

.PHONY : build_dir
build_dir:
	@mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/crt/crt0.o: $(SRC_DIR)/crt0.s
	@mkdir -p $(BUILD_DIR)/crt
	$(AS) -o $@ $<

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

.PHONY : all
all: build_dir $(BUILD_DIR)/crt/crt0.o $(OBJECTS)

.PHONY : clean
clean:
	rm -r $(BUILD_DIR)/