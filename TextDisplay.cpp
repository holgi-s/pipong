//
// Created by Holger on 04.03.2018.
//

#include "TextDisplay.h"

#include "font8x8_basic.h"

TextDisplay::TextDisplay(FrameBuffer& frame_buffer)
    : _frame_buffer(frame_buffer) {
}

void TextDisplay::write(int line, std::string text) {
    for (int i = 0; i < 7; ++i) {
        put(i, line, text[i]);
    }
}

void TextDisplay::put(int block_x, int block_y, char character){

    if(_flip){
        for (int led_row = 0; led_row < 8; ++led_row) {
            const uint8_t char_row = font8x8_basic[character][7-led_row];
            _frame_buffer.block(6-block_x, 3-block_y, led_row, char_row);
        }
    } else {
        for (int led_row = 0; led_row < 8; ++led_row) {
            const uint8_t char_row = font8x8_basic[character][led_row];
            const uint8_t mirrored_char_row = mirror(char_row);
            _frame_buffer.block(block_x, block_y, led_row, mirrored_char_row);
        }
    }
}

uint8_t TextDisplay::mirror(const uint8_t ch){
    uint8_t m = 0;
    for(int i=0; i<8; ++i){
        if(ch & 0x01 << i){
            m |= 0x01 << (7-i);
        }
    }
    return m;
}
