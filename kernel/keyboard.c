#include <kernel/cpu.h>
#include <kernel/keyboard.h>
#include <kernel/vga.h>
#include <kernel/port.h>
#include <kernel/mem.h>

#include <stdbool.h>

#define KM_MAX 57

static bool caps_lock = false;

const char keymap_us[] = {
  '?', '?', '1', '2', '3', '4', '5',
  '6', '7', '8', '9', '0', '-', '=',
  '\0', '?', 'q', 'w', 'e', 'r', 't',
  'y', 'u', 'i', 'o', 'p', '[', ']',
  '\0', '?', 'a', 's', 'd', 'f', 'g',
  'h', 'j', 'k', 'l', ';', '\'', '`',
  '?', '\\', 'z', 'x', 'c', 'v', 'b',
  'n', 'm', ',', '.', '/', '?', '?',
  '?', ' '
};

static char *buffer;
static int buffer_index = 0;

void buffer_append_char(char ch) {
	buffer[buffer_index] = ch;
	buffer_index++;

	if(buffer_index == BLOCK_SIZE) buffer_index--;
}

void buffer_remove_char() {
	buffer_index--;

	if(buffer_index < 0) buffer_index = 0;
	
	buffer[buffer_index] = 0;
}

void buffer_clear() {
	for(int i = 0; i < BLOCK_SIZE; i++) {
		buffer[i] = 0;
	}

	buffer_index = 0;
}

char scancode_to_char(uint8_t scancode) {
	char ch = keymap_us[(int) scancode];

	if(ch >= 'a' && ch <= 'z' && caps_lock) {
		return ch - 32;
	}

	return ch;
}

static void keyboard_callback(registers_t *regs) {
    uint8_t scancode = port_byte_in(0x60);

	if(scancode == 0x3a) {
    	caps_lock = !caps_lock;
	} else if(scancode == 0x0e) {
		print_backspace();
		buffer_remove_char();
	} else if(scancode == 0x1c) {
		print_string("\n");

		// Enter
		print_string(buffer);
		print_string("\n");
		
		buffer_clear();
	} else {
	    if(scancode > KM_MAX) return;

	    char ch = scancode_to_char(scancode);
	    char str[2] = {ch, '\0'};

		buffer_append_char(ch);
	    
	    print_string(str);
    }
}

void init_keyboard() {
	buffer = (char *) mem_alloc();

	print_string("[KEYBOARD] Buffer = 0x");
	print_address((uint32_t) buffer);
	print_string("\n");
	
    register_interrupt_handler(IRQ1, keyboard_callback);
}
