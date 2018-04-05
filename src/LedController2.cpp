//
// Created by Holger on 12.1.2018.
//

#include "LedController2.h"
#include "pigpio_interface.h"

#include <iostream>
#include <time.h>
#include <cmath>
#include <numeric>

#define SPI_MOSI   10
#define SPI_CS1    7
#define SPI_CS0    8
#define SPI_CLK    11
#define SPI_MISO   9

#define DIST_TRIGGER_1   19
#define DIST_ECHO_1      26
#define DIST_ECHO_2      20

#define SPI_BUS_SPEED 921600


LedController2::LedController2(int pigpio_handle, int panel_count) {

    //std::cout << "LedController2! "  << std::endl;

    _fd = pigpio_handle;

    _panel_count = panel_count;

    set_mode(_fd, SPI_MOSI, PI_OUTPUT);
    set_mode(_fd, SPI_CS0, PI_OUTPUT);
    set_mode(_fd, SPI_CS1, PI_OUTPUT);
    set_mode(_fd, SPI_CLK, PI_OUTPUT);
    set_mode(_fd, SPI_MISO, PI_INPUT);

    //set channel to cs1 because we will manually toggle cs0
    unsigned spi_flags = 0;
    _spi = spi_open(_fd, 1, SPI_BUS_SPEED, spi_flags); // channel 1 -> cs1

    setTestMode(false);
    setDecodeMode(0);
    setScanLimit(7);
    setIntensity(2);
    setShutdownMode(false);

}


LedController2::~LedController2() {

    setShutdownMode(true);

    spi_close(_fd, _spi);

}

void LedController2::setDecodeMode(uint8_t mode)
{
    std::vector<uint8_t> values(_panel_count, mode);
    write(0x09, values);
}
void LedController2::setIntensity(uint8_t intensity) {
    std::vector<uint8_t> values(_panel_count, intensity);
    write(0x0a, values);
}

void LedController2::setScanLimit(uint8_t scanLimit) {
    std::vector<uint8_t> values(_panel_count, scanLimit);
    write(0x0b, values);
}
void LedController2::setShutdownMode(bool shutdown){
    std::vector<uint8_t> values(_panel_count, shutdown ? 0 : 1);
    write(0x0c, values);
}

void LedController2::setTestMode(bool test){
    std::vector<uint8_t> values(_panel_count, test ? 1: 0);
    write(0x0f, values); }


void LedController2::write(uint8_t adr, uint8_t data0, uint8_t data1, uint8_t data2, uint8_t data3) {
    write(((adr&0x0f)<<8) | data0, ((adr&0x0f)<<8) | data1, ((adr&0x0f)<<8) | data2, ((adr&0x0f)<<8) | data3);
}

void LedController2::write(uint16_t word0, uint16_t word1, uint16_t word2, uint16_t word3) {

    char buf[8];
    buf[0] = (word0 & 0xff00)  >> 8;
    buf[1] = word0 & 0xff;
    buf[2] = (word1 & 0xff00)  >> 8;
    buf[3] = word1 & 0xff;
    buf[4] = (word2 & 0xff00)  >> 8;
    buf[5] = word2 & 0xff;
    buf[6] = (word3 & 0xff00)  >> 8;
    buf[7] = word3 & 0xff;

    gpio_write(_fd, SPI_CS0, 0);
    spi_write(_fd, _spi, buf, 8);
    gpio_write(_fd, SPI_CS0, 1);
}

void LedController2::write(uint8_t adr, const std::vector<uint8_t>& data) {

    adr = adr & 0x0f;

    std::vector<char> buffer;
    buffer.reserve(data.size()*2);
    for (uint8_t b : data) {
        buffer.push_back(adr);
        buffer.push_back(b);
    }

    gpio_write(_fd, SPI_CS0, 0);
    spi_write(_fd, _spi, &buffer[0], data.size()*2);
    gpio_write(_fd, SPI_CS0, 1);
}

