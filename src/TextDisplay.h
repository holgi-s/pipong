//
// Created by Holger on 04.03.2018.
//

#ifndef LEDBAR_DISPLAYCONTROLLER_H
#define LEDBAR_DISPLAYCONTROLLER_H

#include "FrameBuffer.h"

#include <string>

class TextDisplay {

public:
    TextDisplay(FrameBuffer& frame_buffer);

    void put(int block_x, int block_y, char character);
    void write(int line, std::string text);

    void flip(bool flip) { _flip = flip; }
private:
    uint8_t mirror(uint8_t ch);

    FrameBuffer& _frame_buffer;
    bool _flip = false;
};


#endif //LEDBAR_DISPLAYCONTROLLER_H
