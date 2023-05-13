#include<iostream>
#include<ctime>
#include<vector>
#include<sstream>
#include <thread>
#include <chrono>

#include "player.h"
#include "game_construction.hpp"
#include "player_gui.hpp"
#include "animation.hpp"

class Game
{
private:
    Game() : boss(8, "boss")
    {
        this->dungeons_left = 0;
        if (!music.openFromFile("assets/8bitmusic.mp3"))
        {
            throw FileError("music loading error");
        }
        music.setLoop(true);
        music.play();
        xr = yr = 3;
        for (int i = 1; i <= 6; i++)
        {
            for (int j = 1; j <= 6; j++)
            {
                this->visited[i][j] = false;
            }
        }
        this->possible_rooms.clear();
        player.setPosition(400, 470);
        this->entities.push_back(&player);
        ///init window
        this->videoMode = sf::VideoMode(900, 900);
        this->window = new sf::RenderWindow(this->videoMode, "Unknown Project", sf::Style::Close | sf::Style::Titlebar);
        this->window->setFramerateLimit(60);
        ///final init
        this->endGame = false;
    }
    GUI GUI_;
    animation boss;
    static sf::Music music;
    sf::VideoMode videoMode;
    sf::View view;
    bool endGame;
    sf::RenderWindow* window;
    std::vector<room>possible_rooms;
    std::vector<projectile>ally_projectiles;
    std::vector<projectile>enemy_projectiles;
    room current_room;
    resource_holder rh;
    //room layout
    room roomlayout[7][7];
    bool visited[7][7];
    int xr, yr;
    sf::Event sfmlEvent{};
    std::vector<entity*>entities;
    Player player;
    int dungeons_left;

    sf::Font font;
    sf::Text guiText;
    sf::Text endGameText;

    //methods
    void update();

    void updatePlayer();
    void setView();
    void checkcolliders();
    void updateProjectiles();

    void fill_dungeon(int x, int y);
    void generate_dungeon();
    void handle_enemy();
    void render_enemy();
public:
    //Constructors and Destructors
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    static Game& start_game()
    {
        static Game game_;
        return game_;
    }
    ~Game()
    {
        delete this->window;
    }
    ///Accessors

    //Functions
    bool running() const; //
    void pollEvents(); //
    //void add_projectile(const projectile& projectile_);



    void render(); //
    void create_rooms(); //

    friend std::ostream& operator<<(std::ostream& out, const Game& game);

};