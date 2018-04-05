//
// Created by Holger on 04.03.2018.
//

#include "BinaryDisplay.h"
#include <cmath>

BinaryDisplay::BinaryDisplay(FrameBuffer& frame_buffer)
:   _frame_buffer(frame_buffer) {
}

void BinaryDisplay::put(int pixel_x, int pixel_y, bool set) {
    _frame_buffer.pixel(pixel_x, pixel_y, set);
}

void BinaryDisplay::line(int x1, int y1, int x2, int y2, bool set) {

    float dX = x2 - x1;
    float dY = y2 - y1;

    float signX = dX >= 0 ? 1 : -1;
    float signY = dY >= 0 ? 1 : -1;

    float absX = std::fabs(dX);
    float absY = std::fabs(dY);

    if(absX > absY) {
        if(absX == 0 ) absX = 1;
        float sy = dY / absX;
        for(float x = 0;x <= absX; ++x){
            put((int)(x1 + x*signX), (int)std::round(y1 + x*sy), set);
        }
    } else {
        if(absY == 0 ) absY = 1;
        float sx = dX / absY;
        for(float y = 0;y <= absY; ++y){
            put((int)std::round(x1+(y*sx)), (int)(y1+y*signY),set);
        }
    }
}

void BinaryDisplay::dotted_line(int x1, int y1, int x2, int y2, bool start, bool set) {

    float dX = x2 - x1;
    float dY = y2 - y1;

    float signX = dX >= 0 ? 1 : -1;
    float signY = dY >= 0 ? 1 : -1;

    float absX = std::fabs(dX);
    float absY = std::fabs(dY);

    bool toggle = start;
    if(absX > absY) {
        if(absX == 0 ) absX = 1;
        float sy = dY / absX;
        for(float x = 0;x < absX; ++x){
            if(toggle){
                put((int)(x1 + x*signX), (int)std::round(y1 + x*sy), set);
            }
            toggle = !toggle;
        }
    } else {
        if(absY == 0 ) absY = 1;
        float sx = dX / absY;
        for(float y = 0;y < absY; ++y){
            if(toggle) {
                put((int) std::round(x1 + (y * sx)), (int) (y1 + y * signY), set);
            }
            toggle = !toggle;
        }
    }
}

void BinaryDisplay::box(int x, int y, int size, bool set) {
    line(x,y, x+size,y,set);
    line(x,y+size, x+size,y+size,set);
    line(x,y, x,y+size,set);
    line(x+size,y, x+size,y+size,set);
}

void BinaryDisplay::box(int x, int y, int size_x, int size_y, bool set) {
    line(x,y, x+size_x,y,set);
    line(x,y+size_y, x+size_x,y+size_y,set);
    line(x,y, x,y+size_y,set);
    line(x+size_x,y, x+size_x,y+size_y,set);
}

