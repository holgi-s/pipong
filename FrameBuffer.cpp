//
// Created by Holger on 17.03.2018.
//

#include "FrameBuffer.h"

#include <iostream>


FrameBuffer::FrameBuffer() {
    clear();
}

void FrameBuffer::clear() {
    memory = std::vector<std::vector <uint8_t>>(8,std::vector <uint8_t>(BLOCK_COUNT_X*BLOCK_COUNT_Y,0));
}

void FrameBuffer::pixel(int pixel_x, int pixel_y, bool set) {

    if(pixel_x<0 || pixel_x >= PIXEL_COUNT_X) return;
    if(pixel_y<0 || pixel_y >= PIXEL_COUNT_Y) return;

    int led_row = pixel_y % 8;
    int led_col = 7 - (pixel_x % 8);
    const int block_pos = block_offset(pixel_x / 8, pixel_y / 8);
    if (block_pos >= 0 && block_pos < memory[led_row].size()) {
        int data = memory[led_row][block_pos];
        if (set) {
            data |= 1 << led_col;
        } else {
            data &= ~(1 << led_col);
        }
        memory[led_row][block_pos] = data;
    }
}

void FrameBuffer::block(int block_x, int block_y, int led_row, uint8_t block) {
    if(led_row<0 || led_row >= 8) return;
    const int block_position = block_offset(block_x, block_y);
    if(block_position>=0 && block_position<memory[led_row].size()){
        memory[led_row][block_position] = block;
    }
}

void FrameBuffer::block(int block_x, int block_y, std::vector <uint8_t> block_rows) {
    const int block_position = block_offset(block_x, block_y);
    if(block_position>=0 && block_position<memory[0].size()) { //check size of row 0,
        for (int led_row = 0; led_row < 8; ++led_row) {
            memory[led_row][block_position] = block_rows[led_row];
        }
    }
}

int FrameBuffer::block_offset(int x, int y) {
    return ((BLOCK_COUNT_Y-1)-y) * BLOCK_COUNT_X + x;
}

void FrameBuffer::display(LedController2& led) {

    for (int led_row = 0; led_row < 8; ++led_row) {
        led.write(led_row + 1, memory[led_row]);
    }

#ifdef __WIN32__
    debug();
#endif
}

void FrameBuffer::debug() {

    std::cout << std::endl << "NEW FRAME:" << std::endl<< std::endl;
    for(int y = 0; y < BLOCK_COUNT_Y; ++y){
        for(int led_row = 0; led_row < 8; ++led_row){
            for(int x = 0; x < BLOCK_COUNT_X; ++x){
                int o = block_offset(x, y);
                uint8_t line8 = memory[led_row][o];
                for (int led_col = 7;led_col>=0;--led_col){
                    int mask = 1 << led_col;
                    std::cout << ((line8 & mask) == mask ? "#" : ".");
                }
                std::cout << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}