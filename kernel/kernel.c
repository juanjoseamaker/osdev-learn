#include <kernel/vga.h>
#include <kernel/cpu.h>
#include <kernel/keyboard.h>
#include <kernel/mem.h>
#include <kernel/rtc.h>
#include <kernel/ata.h>

#include <stdbool.h>

// ----------------------------------------------------

int compare_string(char *s1, char *s2) {
    int i;
    for(i = 0; s1[i] == s2[i]; i++) {
        if (s1[i] == '\0') return 0;
    }
    return s1[i] - s2[i];
}

void shell() {
    char *input_buffer = (char *) mem_alloc();
    bool running = true;

    while(running) {
        print_string("> ");
        read_string(input_buffer, 1024);

        if(compare_string("date", input_buffer) == 0) {
            print_time(get_global_time());
        } else if(compare_string("clear", input_buffer) == 0) {
            clean_screen();
        } else if(compare_string("echo", input_buffer) == 0) {
            do {
                read_string(input_buffer, 1024);
            } while(compare_string("quit", input_buffer) != 0);
        } else if(compare_string("help", input_buffer) == 0) {
            print_string("Osdev-learn is a very simple operating system for x86 32 bit machines and is compatible with x86 64 bit machines\n");
            print_string("Commands:\n");
            print_string("    date  -- Print current date from RTC\n");
            print_string("    echo  -- Let write things, type \"quit\" to exit\n");
            print_string("    clear -- Clear screen\n");
            print_string("    help  -- Print this help\n");
        } else {
            print_string("Command not found");
            print_nl();
        }
    }
}

// ----------------------------------------------------

void main() {
    set_color(BROWN, YELLOW);
    clean_screen();

    print_string("Starting OS kernel\n");

    print_string("Installing memory manager.\n");
    init_mem();

    print_string("Installing external interrupts.\n");
    isr_install();

    print_string("Enabling external interrupts.\n");
    asm volatile("sti");

    print_string("Initializing keyboard (IRQ 1).\n");
    init_keyboard();

    print_string("Initializing RTC (IRQ 8).\n");
    init_rtc();

    print_string("Initializing ATA PIO driver.\n");
    init_ata();

    shell();
}

