// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <dates.h>
#include <lib.h>

static uint8_t days_per_month[2][13] = {{0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
                                      {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};

uint8_t local_hours() {
    uint8_t hour = get_hours();
    if (hour <= 3) {
        return hour - 3 + 21;
    }
    return hour - 3;
}

uint8_t local_day() {
    uint8_t day = get_day();
    if (get_hours() <= 3) {
        if (day == 1) {
            uint8_t year = get_year();
            int leap_year = 0;
            if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) {
                leap_year = 1;
            }
            return days_per_month[leap_year][local_month()];
        }
        return day - 1;
    }
    return get_day();
}

uint8_t local_month() {
    if (get_hours() <= 3 && get_day() == 1) {
        if (get_month() == 1) {
            return 12;
        }
        return get_month() - 1;
    }
    return get_month();
}

uint8_t local_year() {
    if (get_hours() <= 3 && get_day() == 1 && get_month() == 1) {
        return get_year() - 1;
    }
    return get_year();
}