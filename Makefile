GCC_VERSION = $(shell arm-none-eabi-gcc -dumpversion)

SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))

CROSS   := arm-none-eabi-
AS      := $(CROSS)as
CC      := $(CROSS)gcc
LD      := $(CROSS)ld
OBJCOPY := $(CROSS)objcopy

ARMINC = /usr/arm-none-eabi/include
ARMLIB = /usr/arm-none-eabi/lib
GCCLIB = /usr/lib/gcc/arm-none-eabi/$(GCC_VERSION)

CRELEASE = -O2
CDEBUG = -g -DDEBUG
LDRELEASE = -s
LDDEBUG = -g

MODEL    = -mthumb-interwork -mthumb
CFLAGS   = -std=c99 -fno-common $(MODEL) -mlong-calls -I $(ARMINC) -I $(INCLUDE_DIR) $(CRELEASE)
LDFLAGS = -nostartfiles -lc -lgcc -L $(ARMLIB) $(LDRELEASE) \
	  -L $(ARMLIB)/thumb \
	  -L $(GCCLIB) \
	  -T arm-gba.ld

crt0.o : $(SRC_DIR)/crt0.s
	@$(AS) $(MODEL) $^ -o $(BUILD_DIR)/crt0.o

libc_sbrk.o : $(SRC_DIR)/libc_sbrk.c
	@$(CC) $(CFLAGS) $(CRELEASE) -c $< -o $(BUILD_DIR)/libc_sbrk.o

.PHONY : build_dir
build_dir:
	@mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY : all
all: build_dir crt0.o libc_sbrk.o $(OBJECTS)

.PHONY : clean
clean:
	rm -r $(BUILD_DIR)/


