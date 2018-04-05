//
// Created by Holger on 17.03.2018.
//
#include "pigpio_interface.h"

int pigpio_start(void*, void*) {
    return 0;
}
int set_mode(int, int, int){
    return 0;
}
int set_pull_up_down(int, int, int){
    return 0;
}
int gpio_write(int, int, int ){
    return 0;
}
int gpio_trigger(int, int, int, int){
    return 0;
}
int spi_write(int, int, char*, int ){
    return 0;
}
int spi_open(int, int, int, unsigned ) {
    return 0;
}
int spi_close(int, int){
    return 0;
}
void pigpio_stop(int) {
    return;
}

int callback(int pi, unsigned user_gpio, unsigned edge, CBFunc_t f) {
    return 0;
}
int callback_ex(int pi, unsigned user_gpio, unsigned edge, CBFuncEx_t f, void *userdata) {
    return 0;
}
int callback_cancel(unsigned callback_id) {
    return 0;
}

int wave_clear(int pi) {
    return 0;
}
int wave_add_generic(int pi, unsigned numPulses, gpioPulse_t *pulses) {
    return 0;
}
int wave_create(int) {
    return 0;
}
int wave_send_repeat(int pi, unsigned wave_id) {
    return 0;
}
int wave_tx_stop(int pi) {
    return 0;
}

pthread_t *start_thread(gpioThreadFunc_t thread_func, void *userdata){
    return nullptr;
}

void stop_thread(pthread_t *pth) {
    return;
}
