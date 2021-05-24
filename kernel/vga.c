#include <kernel/vga.h>
#include <kernel/port.h>

#define VGA_CTRL_REGISTER 0x3d4
#define VGA_DATA_REGISTER 0x3d5
#define VGA_OFFSET_LOW 0x0f
#define VGA_OFFSET_HIGH 0x0e

#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80

static char vga_color = 0x0f;

void set_color(char background, char foreground) {
    vga_color = background << 4 | foreground;
}

void set_cursor(int offset) {
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    port_byte_out(VGA_DATA_REGISTER, (unsigned char) (offset >> 8));
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    port_byte_out(VGA_DATA_REGISTER, (unsigned char) (offset & 0xff));
}

int get_cursor() {
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    int offset = port_byte_in(VGA_DATA_REGISTER) << 8;
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    offset += port_byte_in(VGA_DATA_REGISTER);
    return offset;
}

void set_char(int offset, char character) {
    unsigned char *video_memory = (unsigned char *) VIDEO_ADDRESS;
    video_memory[offset * 2] = character;
    video_memory[offset * 2 + 1] = vga_color;
}

void clean_screen() {
    for(int i = 0; i < MAX_COLS * MAX_ROWS; i++) {
        set_char(i, ' ');
    }

    set_cursor(0);
}

void print_string(char *string) {
    int offset = get_cursor();

    for(int i = 0; string[i] != 0; i++) {
        if(string[i] == '\n') {
            offset += MAX_COLS - (offset % MAX_COLS);
        } else {
            set_char(offset, string[i]);
            offset++;
        }

        if(offset > MAX_COLS * MAX_ROWS) {
            clean_screen();
            offset = 0;
        }
    }

    set_cursor(offset);
}

void print_backspace() {
    int offset = get_cursor();

    if(offset == 0) return;

    set_char(offset - 1, ' ');

    set_cursor(offset - 1);
}

void print_address(unsigned int address) {
    long quotient, remainder;
    int j = 0;
    char hexadecimalnum[100];

    quotient = address;

    while (quotient != 0) {
        remainder = quotient % 16;
        if (remainder < 10)
            hexadecimalnum[j++] = 48 + remainder;
        else
            hexadecimalnum[j++] = 55 + remainder;
        quotient = quotient / 16;
    }

    for(int i = j - 1; i >= 0; i--) {
        set_char(get_cursor(), hexadecimalnum[i]);
        set_cursor(get_cursor() + 1);
    }
}
