//
// Created by Holger on 18.03.2018.
//

#include "GameApp.h"
#include "FrameBuffer.h"
#include "sleep.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>

GameApp::GameApp(LedController2& led, RangeController& range, bool& quit_game)
:   _quit_game(quit_game)
,   _led(led)
,   _range(range) {

}

void GameApp::play_loop() {

    FrameBuffer frame;

    BinaryDisplay bin(frame);
    TextDisplay text(frame);

    text.flip(true); // my display is build upside down, so i need to flip the text display

    while(!_quit_game){

        frame.clear();

        //score up front, because text display will also write the "0"s
        show_score(text);
        show_field(bin);
        show_goals(bin);

        update_show_ball(bin);

        update_controller(bin, 1);
        update_controller(bin, 2);

        frame.display(_led);

        sleep_ms((int)_delay);

#ifdef __WIN32__
        //for screen debugging
        sleep_ms(500);
#endif
    }
}

void GameApp::show_score(TextDisplay& text) {
    if(_score_counter > 0) {
        std::stringstream ss;
        ss << " " << std::setfill(' ') << std::setw(2) << _score_player_1 << " " << _score_player_2 << " ";
        text.write(1, ss.str());
        --_score_counter;
    } else {
        if(_score_player_1 >= 10 || _score_player_2 >= 10) {
            _score_player_1 = _score_player_2 = 0;
            _score_counter = 50;
        }
    }
}

void GameApp::show_field(BinaryDisplay& bin) {
    bin.line(0,0,55,0);
    bin.line(0,31,55,31);

    bin.dotted_line(27,0,27,31, true);
    bin.dotted_line(28,0,28,31, false);
}

void GameApp::show_goals(BinaryDisplay& bin) {
    if(_position_ball_x >= 0) {
        bin.dotted_line(0, 0, 0, 31, true);
    }
    if(_position_ball_x <= 55) {
        bin.dotted_line(55,0,55,31, false);
    }
}

void GameApp::update_show_ball(BinaryDisplay& bin) {

    const float reduce_delay = 0.5;

    _position_ball_x += _velocity_ball_x;
    _position_ball_y += _velocity_ball_y;

    //check for upper and lower border
    if(_position_ball_y < 1 || _position_ball_y > 29 ) {
        _velocity_ball_y *= -1;
        _position_ball_y += _velocity_ball_y;
        _delay -= reduce_delay;
    }

    const int racket_height = 5;

    //check for player racket
    if(_velocity_ball_x <= 0) {
        //ball going left
        if (    (_position_ball_x > 1 && _position_ball_x < 3) &&
                _position_ball_y > _position_player_1 && _position_ball_y < (_position_player_1 + racket_height)) {
            _velocity_ball_x *= -1;
            _position_ball_x += _velocity_ball_x;
            _delay -= reduce_delay;
        }
    } else {
        //ball going right
        if (    (_position_ball_x > 51 && _position_ball_x < 53) &&
                _position_ball_y > _position_player_2 && _position_ball_y < (_position_player_2 + racket_height)) {
            _velocity_ball_x *= -1;
            _position_ball_x += _velocity_ball_x;
            _delay -= reduce_delay;
        }
    }

    //check for goals
    if(_position_ball_x < -10) {
        _velocity_ball_x *= -1;
        _position_ball_x += _velocity_ball_x;
        _delay = 20;
        ++_score_player_1;
        _score_counter = 70;
        kickoff(2);
        std::cout << "Player 1 scored a goal! -> " << _score_player_1 << " : " << _score_player_2 << std::endl;
    }

    if(_position_ball_x > 66) {
        _velocity_ball_x *= -1;
        _position_ball_x += _velocity_ball_x;
        _delay = 20;
        ++_score_player_2;
        _score_counter = 70;
        kickoff(1);
        std::cout << "Player 2 scored a goal! -> " << _score_player_1 << " : " << _score_player_2 << std::endl;
    }

    bin.box(std::round(_position_ball_x), std::round(_position_ball_y),1);
}

void GameApp::kickoff(int player) {
    if(player == 1){
        _position_ball_x = 52;
        _position_ball_y = _position_player_1+3;
        _velocity_ball_x = -0.5;
        _velocity_ball_y = 0.3;
    } else {
        _position_ball_x = 3;
        _position_ball_y = _position_player_2+3;
        _velocity_ball_x = 0.5;
        _velocity_ball_y = 0.3;
    }
}

void GameApp::update_controller(BinaryDisplay& bin, int player){

    float position = 1.0f;

    int position_mm = (player == 1) ? _range.lastDistance() : _range.lastDistance2();
    if(position_mm < 150.0f) {
        position = 1;
    } else if(position_mm > 300.0f) {
        position = 25.0f;
    } else {
        position = (((float)position_mm - 150.0f)/150.0f)*25.0f;
    }

    if (player == 1) {
        _position_player_1 = 0.8 * _position_player_1 + 0.2 * position;
        //_position_player_1 = (int)std::round(position);
        bin.box(2,(int)_position_player_1, 1, 5);
    } else {
        _position_player_2 = 0.8 * _position_player_2 + 0.2 * position;
        //_position_player_2 = (int)std::round(position);
        bin.box(52,(int)_position_player_2, 1, 5);
    }

}