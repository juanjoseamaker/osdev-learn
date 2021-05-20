#include <kernel/vga.h>

void main() {
	set_color(BROWN, YELLOW);
	clean_screen();
	print_string("Hello from kernel\n");
}
