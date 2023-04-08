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
    room current_room;
	//room level_layout[5][5];
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

	void updatePlayer();
	void setView();

	void render();

	void create_rooms();

    ///copy constructor
    Game(const Game& other);
    ///copy assignment operator
    Game& operator=(const Game& other);
    friend std::ostream& operator<<(std::ostream& out, const Game& game);
};