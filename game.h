#pragma once

#include<iostream>
#include<ctime>
#include<vector>
#include<sstream>

#include "player.h"
#include "game_construction.hpp"

class Game
{
private:
	sf::VideoMode videoMode;
	sf::View view;
	bool endGame;
	sf::RenderWindow* window;
	std::vector<room>possible_rooms;
    std::vector<projectile>projectiles;
    room current_room;
	resource_holder rh;
    //room layout
    room roomlayout[7][7];
	
	sf::Event sfmlEvent{};

	Player player;

	sf::Font font;
	sf::Text guiText;
	sf::Text endGameText;

public:
	//Constructors and Destructors
	Game();
	~Game();
	///Accessors

	//Functions
    bool running() const;
	void pollEvents();
    void update();
    void add_projectile(const projectile& projectile_);

	void updatePlayer();
	void setView();

	void render();

	void fill_dungeon(int x, int y);
	void create_rooms();
    void generate_dungeon();

    ///copy constructor
    Game(const Game& other);
    ///copy assignment operator
    Game& operator=(const Game& other);
    friend std::ostream& operator<<(std::ostream& out, const Game& game);
};