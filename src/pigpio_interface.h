//
// Created by Holger on 11.03.2018.
//

#ifndef LEDBAR_PIGPIO_INTERFACE_H
#define LEDBAR_PIGPIO_INTERFACE_H


#ifdef __WIN32__

#include <cstdint>

// empty stubs to allow win32 compilation

    int pigpio_start(void*, void*);
    int set_mode(int, int, int);
    int set_pull_up_down(int, int, int);
    int gpio_write(int, int, int );
    int gpio_trigger(int, int, int, int);
    int spi_write(int, int, char*, int );
    int spi_open(int, int, int, unsigned );
    int spi_close(int, int);
    void pigpio_stop(int);

    typedef void (*CBFunc_t) (int pi, unsigned user_gpio, unsigned level, uint32_t tick);
    typedef void (*CBFuncEx_t) (int pi, unsigned user_gpio, unsigned level, uint32_t tick, void * userdata);
    int callback(int pi, unsigned user_gpio, unsigned edge, CBFunc_t f);
    int callback_ex(int pi, unsigned user_gpio, unsigned edge, CBFuncEx_t f, void *userdata);
    int callback_cancel(unsigned callback_id);

    typedef struct {
        uint32_t gpioOn;
        uint32_t gpioOff;
        uint32_t usDelay;
    } gpioPulse_t;

    int wave_clear(int pi);
    int wave_add_generic(int pi, unsigned numPulses, gpioPulse_t *pulses);
    int wave_create(int);
    int wave_send_repeat(int pi, unsigned wave_id);
    int wave_tx_stop(int pi);

#include <pthread.h>

    typedef void *(gpioThreadFunc_t) (void *);
    pthread_t *start_thread(gpioThreadFunc_t thread_func, void *userdata);
    void stop_thread(pthread_t *pth);

    #define EITHER_EDGE 0
    #define PI_OUTPUT 1
    #define PI_INPUT  0
    #define PI_PUD_OFF 0

#else

    #include <pigpiod_if2.h>

#endif


#endif //LEDBAR_PIGPIO_INTERFACE_H
