//
// Created by Holger on 18.03.2018.
//

#ifndef LEDBAR_GAMEAPP_H
#define LEDBAR_GAMEAPP_H

#include "LedController2.h"
#include "RangeController.h"
#include "TextDisplay.h"
#include "BinaryDisplay.h"

class GameApp {
public:
    GameApp(LedController2& led, RangeController& range, bool& quit_game);

    void play_loop();

private:
    void show_score(TextDisplay& text);
    void show_field(BinaryDisplay &bin);
    void show_goals(BinaryDisplay &bin);
    void update_show_ball(BinaryDisplay &bin);
    void update_controller(BinaryDisplay &bin, int player);
    void kickoff(int player);

    bool& _quit_game;
    int _delay = 20;
    int _score_counter = 30;
    int _score_player_1 = 0;
    int _score_player_2 = 0;

    float _position_ball_x = 52;
    float _position_ball_y = (32/2)-1;

    float _velocity_ball_x = -0.5;
    float _velocity_ball_y = 0.3;

    float _position_player_1 = (32/2)-1;
    float _position_player_2 = (32/2)-1;

    LedController2& _led;
    RangeController& _range;
};


#endif //LEDBAR_GAMEAPP_H
