//
// Created by Holger on 04.03.2018.
//

#ifndef LEDBAR_BINARYDISPLAY_H
#define LEDBAR_BINARYDISPLAY_H

#include "FrameBuffer.h"

#include <cstdint>
#include <vector>
#include <string>

class BinaryDisplay {
public:
    BinaryDisplay(FrameBuffer& frame_buffer);

    void put(int pixel_x, int pixel_y, bool set = true);

    void line(int x, int y, int x2, int y2, bool set = true);
    void dotted_line(int x1, int y1, int x2, int y2, bool start, bool set = true);

    void box(int x, int y, int size, bool set = true);
    void box(int x, int y, int size_x, int size_y, bool set = true);

private:
    FrameBuffer& _frame_buffer;
};


#endif //LEDBAR_BINARYDISPLAY_H
