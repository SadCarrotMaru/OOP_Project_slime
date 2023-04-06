#pragma once

#include<iostream>
#include<ctime>
#include<vector>
#include<sstream>

#include "player.h"
#include "game_construction.h"

class Game
{
private:
	sf::VideoMode videoMode;
	sf::View view;
	bool endGame;
	sf::RenderWindow* window;
	vector<room>possible_rooms;
	room level_layout[5][5];
	sf::Event sfmlEvent;

	Player player;

	int points;

	sf::Font font;
	sf::Text guiText;
	sf::Text endGameText;

	float spawnTimerMax;
	float spawnTimer;
	int maxSwagBalls;

	void initVariables();
	void initWindow();

public:
	//Constructors and Destructors
	Game();
	~Game();
	///Accessors
	const sf::VideoMode& getVideoMode() const;
	const bool& getEndGame() const;

	//Modifiers
	void create_possible_rooms();
	//Functions
    bool running() const;
	void pollEvents();

	void updatePlayer();
	void update();
	void setView();

	void renderGui(sf::RenderTarget* target);
	void render();

	void create_rooms();
};