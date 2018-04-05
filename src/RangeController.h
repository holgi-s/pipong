//
// Created by Holger on 17.03.2018.
//

#ifndef LEDBAR_RANGECONTROLLER_H
#define LEDBAR_RANGECONTROLLER_H

#include <cstdint>
#include <vector>
#include <list>
#include <pthread.h>
#include <atomic>

class RangeController {
public:
    RangeController(int pigpioHandle);
    ~RangeController();

    void trigger();
    int lastDistance();
    int lastDistance2();

    void echo_callback(int pi, unsigned user_gpio, unsigned level, uint32_t tick);
    void trigger_thread();

private:
    float calc_distance(uint32_t start_tick, uint32_t stop_tick);
    float filter_low_pass(float old_value, float new_value);

    int _fd = -1; //_pigpio_file_descriptor

    /*
    struct echo_context {
        int callback_id = 0;
        uint32_t start_tick = 0;
        std::list<float> last_distances;
        std::atomic<int> filtered_distance = 0;
    };

    std::map<int, struct echo_context> echo_context_map;
*/
    int _callback_id1 = 0;
    int _callback_id2 = 0;

    uint32_t _start_tick1 = 0;
    uint32_t _start_tick2 = 0;

    std::list<float> _last_distances1;
    std::list<float> _last_distances2;

    std::atomic<uint32_t> _last_filtered_distance1 = {0};
    std::atomic<uint32_t> _last_filtered_distance2 = {0};


    bool _quit_trigger_thread = false;
    pthread_t* _trigger_thread = nullptr;

    friend void fn_echo_callback(int _pi, unsigned gpio, unsigned level, uint32_t tick, void* p_this);
    friend void * fn_trigger_thread(void* p_this);
};

#endif //LEDBAR_RANGECONTROLLER_H
