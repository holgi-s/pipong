//
// Created by Holger on 17.03.2018.
//

#ifndef LEDBAR_PIGPIO_H
#define LEDBAR_PIGPIO_H


class PiGpio {
public:
    PiGpio();
    ~PiGpio();

    int get_handle() { return _fd; }
private:
    int _fd = -1;
};


#endif //LEDBAR_PIGPIO_H
