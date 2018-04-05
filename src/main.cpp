
#include "PiGpio.h"
#include "LedController2.h"
#include "RangeController.h"
#include "GameApp.h"

static bool _quit_app = false;

#include <iostream>
#include <signal.h>

void sig_handler(int  s){
    std::cout << std::endl << "Ctrl-C signal caught! " << std::endl << "Quiting the game..." << std::endl;
    _quit_app = true;
}

int main() {

    std::cout << "Starting the game..." << std::endl;

    signal (SIGINT, sig_handler);
    PiGpio gpio;
    LedController2 led(gpio.get_handle(), 28);
    RangeController range(gpio.get_handle());

    GameApp app(led, range, _quit_app);
    app.play_loop();

    std::cout << "Bye!" << std::endl;

    return 0;
}
