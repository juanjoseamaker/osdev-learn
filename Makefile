CC	= gcc
ASM	= nasm

SOURCE_PATH	= ./kernel
BUILD_PATH	= ./build

CC_FLAGS	= -m32 -ffreestanding -fno-pie -Wall -c -I $(SOURCE_PATH)/include
ASM_FLAGS	= -f elf

CFILES    := $(shell find $(SOURCE_PATH) -type f -name '*.c')
COBJFILES := $(patsubst %.c,$(BUILD_PATH)/%.o,$(CFILES))

ASMFILES  := $(shell find $(SOURCE_PATH) -type f -name '*.asm')
ASMOBJFILES := $(patsubst %.asm,$(BUILD_PATH)/%.o,$(ASMFILES))

os-image.iso: mbr.bin kernel.bin
	cat $^ > $@

$(BUILD_PATH)/%.o: %.c
	$(CC) $(CC_FLAGS) -c $< -o $@

$(BUILD_PATH)/%.o: %.asm
	$(ASM) $(ASM_FLAGS) $< -o $@

kernel.bin: $(COBJFILES) $(ASMOBJFILES)
	ld -m elf_i386 $(ASMOBJFILES) $(COBJFILES) -o $@ -Ttext 0x1000 --oformat binary

mbr.bin:
	nasm ./bootloader/mbr.asm -f bin -o $@

run: os-image.iso
	qemu-system-i386 $< -enable-kvm

clean:
	rm $(COBJFILES) $(ASMOBJFILES) *.bin *.iso

format:
	astyle $(CFILES)
