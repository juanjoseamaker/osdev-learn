#include <kernel/rtc.h>
#include <kernel/port.h>
#include <kernel/cpu.h>
#include <kernel/vga.h>

#include <stdbool.h>
#include <stdint.h>

time_t global_time;
bool bcd;

uint8_t read_register(uint8_t reg) {
    port_byte_out(0x70, reg);
    return port_byte_in(0x71);
}

void write_register(uint8_t reg, uint8_t value) {
    port_byte_out(0x70, reg);
    port_byte_out(0x71, value);
}

uint8_t bcd2bin(uint8_t bcd) {
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

time_t *get_global_time() {
    return &global_time;
}

static void rtc_callback(registers_t *regs) {
    if(read_register(0x0C) & 0x10) {
        if(bcd) {
            global_time.second = bcd2bin(read_register(0x00));
            global_time.minute = bcd2bin(read_register(0x02));
            global_time.hour   = bcd2bin(read_register(0x04));
            global_time.month  = bcd2bin(read_register(0x08));
            global_time.year   = bcd2bin(read_register(0x09));
            global_time.day_of_week  = bcd2bin(read_register(0x06));
            global_time.day_of_month = bcd2bin(read_register(0x07));
        } else {
            global_time.second = read_register(0x00);
            global_time.minute = read_register(0x02);
            global_time.hour   = read_register(0x04);
            global_time.month  = read_register(0x08);
            global_time.year   = read_register(0x09);
            global_time.day_of_week  = read_register(0x06);
            global_time.day_of_month = read_register(0x07);
        }
    }

    print_time(get_global_time());
}

void print_time(time_t *time) {
    print_string("Second: ");
    print_number((int) time->second);
    print_string(" Minute: ");
    print_number((int) time->minute);
    print_string(" Hour: ");
    print_number((int) time->hour);
    print_string(" Month: ");
    print_number((int) time->month);
    print_string(" Year: ");
    print_number((int) time->year);

    print_string("\n");
}

void init_rtc() {
    uint8_t status;

    status = read_register(0x0B);
    status |=  0x02;             // 24 hour clock
    status |=  0x10;             // update ended interrupts
    status &= ~0x20;             // no alarm interrupts
    status &= ~0x40;             // no periodic interrupt
    bcd     = !(status & 0x04); // check if data type is BCD
    write_register(0x0B, status);

    read_register(0x0C);

    register_interrupt_handler(IRQ8, rtc_callback);
}
