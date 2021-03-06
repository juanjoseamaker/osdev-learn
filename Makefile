CC	= gcc
ASM	= nasm

SOURCE_PATH	= ./kernel
BUILD_PATH	= ./build

CC_FLAGS	= -m32 -ffreestanding -fno-pie -fno-stack-protector -Wall -c -I $(SOURCE_PATH)/include
ASM_FLAGS	= -f elf

CFILES    := $(sort $(shell find $(SOURCE_PATH) -type f -name '*.c'))
COBJFILES := $(patsubst %.c,$(BUILD_PATH)/%.o,$(CFILES))

ASMFILES  := $(sort $(shell find $(SOURCE_PATH) -type f -name '*.asm'))
ASMOBJFILES := $(patsubst %.asm,$(BUILD_PATH)/%.o,$(ASMFILES))

os-image.iso: mbr.bin kernel.bin
	cat $^ > $@

$(BUILD_PATH)/%.o: %.c
	$(CC) $(CC_FLAGS) $< -o $@

$(BUILD_PATH)/%.o: %.asm
	$(ASM) $(ASM_FLAGS) $< -o $@

kernel.bin: $(COBJFILES) $(ASMOBJFILES)
	ld -m elf_i386 $(ASMOBJFILES) $(COBJFILES) -o $@ -Ttext 0x1000 --oformat binary

mbr.bin:
	nasm ./bootloader/mbr.asm -f bin -o $@

run: os-image.iso
	qemu-system-i386 $< -rtc base=localtime -enable-kvm

clean:
	rm $(COBJFILES) $(ASMOBJFILES) *.bin *.iso

format:
	astyle $(CFILES)
	find . -type f -name "*.c.orig" -exec rm -f {} \;
