//
// Created by Holger on 18.11.2017.
//

#ifndef LEDCONTROLLER2_H
#define LEDCONTROLLER2_H

#include <cstdint>
#include <vector>
#include <list>
#include <pthread.h>


class LedController2 {

public:
    LedController2(int pigpio_handle, int panel_count);
    ~LedController2();

    void setDecodeMode(uint8_t mode); //0: no decoding -> bit row
    void setIntensity(uint8_t mode); // brightness => 0: min -> 1/32 duty cycle, 0xf: max -> 31/32 duty cycle
    void setScanLimit(uint8_t mode); // 7: display all 8 bit
    void setShutdownMode(bool shutdown); //0: shutdown, 1: normal operation
    void setTestMode(bool test); //0: normal operation, 1: test mode, all bits onn, full brightness

    void write(uint8_t adr, uint8_t data);
    void write(uint8_t adr, uint8_t data0, uint8_t data1, uint8_t data2, uint8_t data3);
    void write(uint8_t adr, const std::vector<uint8_t>& data);

private:
    void write(uint16_t word);
    void write(uint16_t word0, uint16_t word1, uint16_t word2, uint16_t word3);

private:
    int _panel_count = 4;
    int _fd = -1;
    int _spi = -1;
};


#endif //LEDCONTROLLER2_H
