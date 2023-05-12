#pragma once

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
    //void add_projectile(const projectile& projectile_);

	void updatePlayer();
	void setView();

	void render();
	void checkcolliders();
	void updateProjectiles();

	void fill_dungeon(int x, int y);
	void create_rooms();
    void generate_dungeon();
    void handle_enemy();
    void render_enemy();

    friend std::ostream& operator<<(std::ostream& out, const Game& game);

};