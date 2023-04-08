//#include <SFML/Audio.hpp>
//#include <SFML/Graphics.hpp>
//#include <chrono>
#include <thread>
#include "game.h"

#ifdef __linux__
#include <X11/Xlib.h>
#endif

int main() {
    #ifdef __linux__
    XInitThreads();
    #endif
   
    Game main_game;
    main_game.create_rooms();
    while (main_game.running())
    {
        main_game.pollEvents();
        main_game.render();
    }
    return 0;
}
