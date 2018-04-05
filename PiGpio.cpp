//
// Created by Holger on 17.03.2018.
//

#include "PiGpio.h"
#include "pigpio_interface.h"

PiGpio::PiGpio(){
    _fd = pigpio_start(nullptr, nullptr);
}

PiGpio::~PiGpio(){
    pigpio_stop(_fd);
}
