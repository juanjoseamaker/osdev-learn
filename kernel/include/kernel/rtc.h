#pragma once

typedef struct {
    unsigned char second;
    unsigned char minute;
    unsigned char hour;
    unsigned char day_of_week;
    unsigned char day_of_month;
    unsigned char month;
    unsigned char year;
} time_t;

time_t* get_global_time();
void init_rtc();
void print_time(time_t *time);
