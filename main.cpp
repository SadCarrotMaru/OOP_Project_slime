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
    try
    {
        auto& main_game = Game::start_game();
        main_game.create_rooms();
        while (main_game.running())
        {
            main_game.pollEvents();
            try { main_game.render(); }
            catch (Error& e)
            {
                std::cout << "Game internal error occured :( " << e.what();
                break;
            }
        }
    }
    catch (Error& e)
    {
        std::cout << "Game couldn't start properly :( " << e.what();
    }
    return 0;
}