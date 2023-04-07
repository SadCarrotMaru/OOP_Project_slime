//#include <SFML/Audio.hpp>
//#include <SFML/Graphics.hpp>
//#include <chrono>
#include <thread>
#include "player.h"
#include "game.h"

#ifdef __linux__
#include <X11/Xlib.h>
#endif

class SomeClass {
public:
    explicit SomeClass(int) {}
};

SomeClass *getC() {
    return new SomeClass{2};
}

int main() {
    #ifdef __linux__
    XInitThreads();
    #endif

    SomeClass *c = getC();
    std::cout << c << "\n";
    delete c;
   
    Game main_game;
    main_game.create_rooms();
    while (main_game.running())
    {
        main_game.pollEvents();
        main_game.render();
    }
    return 0;
}
