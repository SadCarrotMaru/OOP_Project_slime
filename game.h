#include<iostream>
#include<ctime>
#include<vector>
#include<sstream>
#include <thread>
#include <chrono>

#include "player.h"
#include "singleton.h"
#include "player_gui.hpp"
#include "animation.hpp"

class Game : public Singleton<Game>
{
private:
    Game() 
    {
        this->dungeons_left = 0;
        if (!music.openFromFile("assets/8bitmusic.mp3"))
        {
            throw FileError("music loading error");
        }
        if (!trap_door.loadFromFile("assets/trap_door.png"))
        {
            throw FileError("trap door loading error");
        }
        if (!ability.loadFromFile("assets/ability.png"))
        {
            throw FileError("ability loading error");
        }
        this->ability.setSmooth(true);
        ability_.setTexture(ability);
        trap_door_.setTexture(trap_door);
        trap_door_.setOrigin(trap_door_.getGlobalBounds().width / 2, trap_door_.getGlobalBounds().height / 2);
        trap_door_.setPosition(805, 680);
        music.setLoop(true);
        music.setVolume(50);
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
    sf::Music music;
    boss* boss_;
    sf::VideoMode videoMode;
    sf::View view;
    bool endGame;
    sf::RenderWindow* window;
    std::vector<room>possible_rooms;
    std::vector<projectile>ally_projectiles;
    std::vector<projectile>enemy_projectiles;
    sf::RectangleShape boss_hp;
    room current_room;
    resource_holder rh;
    //room layout
    room roomlayout[7][7];
    bool visited[7][7];
    int xr, yr;
    int charges_ability = 0 ;
    int small_help = 0;
    sf::Event sfmlEvent{};
    std::vector<entity*>entities;
    Player player;
    int dungeons_left;
    int init_dungeons;
    sf::Texture trap_door;
    sf::Sprite trap_door_;

    sf::Font font;
    sf::Text guiText;
    sf::Text endGameText;

    std::chrono::high_resolution_clock::time_point last = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::time_point last2 = std::chrono::high_resolution_clock::now();
    bool ability_launched = false;
    sf::Texture ability;
    sf::Sprite ability_;

    //methods
    void update();
    void display_map();

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
        for (auto ptr : this->entities)
            delete ptr;    // aparent nu pica de la asta, doar pica in general, nu am idee de ce :(
        delete this->boss_;
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