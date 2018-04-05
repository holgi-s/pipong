//
// Created by Holger on 17.03.2018.
//

#include "RangeController.h"

#include "sleep.h"
#include "pigpio_interface.h"

#include <iostream>
#include <time.h>
#include <cmath>
#include <numeric>
#include <algorithm>

#define MAX_DISTANCE_ENTRIES 2

#define DIST_TRIGGER_1   19
#define DIST_ECHO_1      26
#define DIST_ECHO_2      20

void fn_echo_callback(int pi, unsigned gpio, unsigned level, uint32_t tick, void* p_this);
void * fn_trigger_thread(void * p_this);

RangeController::RangeController(int pigpioHandle) {

    //std::cout << "RangeController! "  << std::endl;

    _fd = pigpioHandle;

    int err = 0;

    set_mode(_fd, DIST_TRIGGER_1, PI_OUTPUT);
    gpio_write(_fd, DIST_TRIGGER_1, 0);

    set_pull_up_down(_fd, DIST_ECHO_1, PI_PUD_OFF);
    set_mode(_fd, DIST_ECHO_1, PI_INPUT);

    _callback_id1 = callback_ex(_fd, DIST_ECHO_1, EITHER_EDGE, fn_echo_callback, this);
    //std::cout << "callback: " << _callback_id1 << std::endl;

    set_pull_up_down(_fd, DIST_ECHO_2, PI_PUD_OFF);
    set_mode(_fd, DIST_ECHO_2, PI_INPUT);

    _callback_id2 = callback_ex(_fd, DIST_ECHO_2, EITHER_EDGE, fn_echo_callback, this);
    //std::cout << "callback: " << _callback_id2 << std::endl;

    for(int i=0; i<MAX_DISTANCE_ENTRIES; ++i) {
        _last_distances1.push_back(0);
        _last_distances2.push_back(0);
    }

    _quit_trigger_thread = false;
    _trigger_thread = start_thread(fn_trigger_thread, this);
}


RangeController::~RangeController() {

    //std::cout << "stopDistance: " << std::endl;

    _quit_trigger_thread = true;
    stop_thread(_trigger_thread);

    int err = callback_cancel(_callback_id1);
    //std::cout << "callback_cancel: " << err << std::endl;

    err = callback_cancel(_callback_id2);
    //std::cout << "callback_cancel: " << err << std::endl;
}

void fn_echo_callback(int pi, unsigned gpio, unsigned level, uint32_t tick, void* p_this) {
    if(p_this) {
        static_cast<RangeController*>(p_this)->echo_callback(pi, gpio, level, tick);
    }
}

void RangeController::echo_callback(int pi, unsigned gpio, unsigned level, uint32_t tick) {

   //std::cout << "echo_callback: " << level << std::endl;

    if(level==1) {
        if(gpio == DIST_ECHO_1) {
            _start_tick1 = tick;
        } else {
            _start_tick2 = tick;
        }
    } else if(level == 0){

        float distance = calc_distance(( gpio == DIST_ECHO_1 ? _start_tick1 : _start_tick2), tick);

        if( gpio == DIST_ECHO_1 ){
            if(distance > 600) {
                distance = _last_filtered_distance1.load();
            }
            _last_distances1.push_back(distance);
            _last_distances1.pop_front();
            float min = *std::min_element(_last_distances1.begin(), _last_distances1.end());
            _last_filtered_distance1.store(filter_low_pass(_last_filtered_distance1.load(), min));
        }else{
            if(distance > 600) {
                distance = _last_filtered_distance2.load();
            }
            _last_distances2.push_back(distance);
            _last_distances2.pop_front();
            float min = *std::min_element(_last_distances2.begin(), _last_distances2.end());
            _last_filtered_distance2.store(filter_low_pass(_last_filtered_distance2.load(), min));
        }
        //std::cout << "echo_callback: " << lastLevel << " mm" << std::endl;
    }

}

float RangeController::filter_low_pass(float old_value, float new_value) {
    const float scale = 0.5f;
    return (1.0f-scale) * old_value + (scale) * new_value;
}

float RangeController::calc_distance(uint32_t start_tick, uint32_t stop_tick){
    const uint32_t peak_tick = stop_tick - start_tick;
    const float peak_time = (float)peak_tick / 1000000.0f; // [µsec] -> [sec]
    const float sound_velocity = 343.2f; // [m/sec] TODO: adjust for temperature
    const float distance_meter = peak_time * sound_velocity / 2; // [m], (roundtrip -> /2)
    const float distance_mm = distance_meter * 1000;
    return distance_mm;
}

void * fn_trigger_thread(void * p_this){
    if(p_this) {
        static_cast<RangeController*>(p_this)->trigger_thread();
    }
}

void RangeController::trigger_thread(){

    //std::cout << "starting trigger_thread..."  << std::endl;

    //documentations is not quite clear it could be a 40Hz measurement cycle or more likely a 16 Hz cycle
    //using the 60ms trigger cycle works quite reliable

    while(!_quit_trigger_thread){
        trigger();
        sleep_ms(60);
    }

    //std::cout << "quit trigger_thread!"  << std::endl;
}

void RangeController::trigger() {
    gpio_trigger(_fd, DIST_TRIGGER_1, 10, 1);
}

int RangeController::lastDistance(){
    return (int)std::round(_last_filtered_distance1.load());
}

int RangeController::lastDistance2(){
    return (int)std::round(_last_filtered_distance2.load());
}
