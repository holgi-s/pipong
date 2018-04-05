//
// Created by Holger on 17.03.2018.
//

#ifndef LEDBAR_FRAMEBUFFER_H
#define LEDBAR_FRAMEBUFFER_H

#include "LedController2.h"

#include <cstdint>
#include <vector>
#include <list>


#define BLOCK_COUNT_X (7)
#define BLOCK_COUNT_Y (4)

#define PIXEL_COUNT_X (BLOCK_COUNT_X*8)
#define PIXEL_COUNT_Y (BLOCK_COUNT_Y*8)


class FrameBuffer {

public:
    FrameBuffer();

    void clear();

    void pixel(int pixel_x, int pixel_y, bool set = true);

    void block(int block_x, int block_y, int led_row, uint8_t block);
    void block(int block_x, int block_y, std::vector<uint8_t> block_rows);

    void display(LedController2& led);
    void debug();

private:
    int block_offset(int x, int y);
    std::vector<std::vector <uint8_t>> memory;
};


#endif //LEDBAR_FRAMEBUFFER_H
