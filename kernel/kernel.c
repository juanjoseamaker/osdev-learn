#include <kernel/vga.h>
#include <kernel/cpu.h>
#include <kernel/keyboard.h>

void main() {
	set_color(BROWN, YELLOW);
	clean_screen();
	
	print_string("Starting OS kernel\n");

	print_string("Installing externel interrupts.\n");
	isr_install();
	
	print_string("Enabling external interrupts.\n");
	asm volatile("sti");

	print_string("Initializing keyboard (IRQ 1).\n");
	init_keyboard();
}
