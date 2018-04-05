//
// Created by Holger on 17.03.2018.
//

#include "sleep.h"
#include <time.h>

void sleep_s(uint16_t sec) {
    struct timespec req;
    req.tv_sec = sec;
    req.tv_nsec = 0;
    nanosleep(&req, nullptr);
}

void sleep_ms(uint16_t ms) {
    struct timespec req;
    req.tv_sec = 0;
    req.tv_nsec = ms * 1000 * 1000;
    nanosleep(&req, nullptr);
}

void sleep_us(uint16_t us) {
    struct timespec req;
    req.tv_sec = 0;
    req.tv_nsec = us * 1000;
    nanosleep(&req, nullptr);
}