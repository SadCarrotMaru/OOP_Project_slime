#include "game.h"

Game::Game()
{
	this->possible_rooms.clear();
    player.setPosition(400,470);
	///init window
    this->videoMode = sf::VideoMode(900, 900);
    this->window = new sf::RenderWindow(this->videoMode, "Unknown Project", sf::Style::Close | sf::Style::Titlebar);
    this->window->setFramerateLimit(60);
    ///final init
    this->endGame = false;
}

Game::~Game()
{
	delete this->window;
}

bool Game::running() const
{
	return this->window->isOpen();
}
void Game::pollEvents()
{
	while (this->window->pollEvent(this->sfmlEvent))
	{
		switch (this->sfmlEvent.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			//this->player.update(this->window);
			if (this->sfmlEvent.key.code == sf::Keyboard::X) std::cout << this->player;
			break;
		/*case sf::Event::KeyReleased:
			this->player.update(this->window);
			break;*/
		default:
			break;
		}
	}
}

void Game::updatePlayer()
{
	this->player.update(this->current_room.getRectangle());

	if (this->player.getHp() <= 0)
		this->endGame = true;
}

void Game::setView()
{
	sf::Vector2f center_map;
	center_map.x = (this->current_room.getBackgroundRectangle().left + this->current_room.getBackgroundRectangle().width) / 2;
	center_map.y = (this->current_room.getBackgroundRectangle().top + this->current_room.getBackgroundRectangle().height) / 2;
	/*
	this->view.setCenter(this->player.getPos());
	this->window->setView(this->view);
	*/
	this->view.setCenter(center_map);
	this->view.zoom(2.0f);
	this->window->setView(this->view);
	this->view.zoom(0.5f);
	}

void Game::update()
{
    this->pollEvents();
    if (!this->endGame)
    {
        this->updatePlayer();
    }
    if(this->endGame)
    {
        this->window->close();
    }
}
void Game::render()
{
	this->window->clear();
	this->updatePlayer();
	this->setView();
	this->current_room.display_background(this->window);
	this->player.render(this->window);
	this->window->display();
}

void Game::create_rooms()
{
	room room1("assets/background1.png", "1111", 140, 240);
	this->possible_rooms.push_back(room1);
	room room2("assets/background2.png", "0000", 140, 240);
	this->possible_rooms.push_back(room2);
    this->current_room = this->possible_rooms[0];
    this->current_room.get_into_room();
}

Game::Game (const Game& other)
{
    this->videoMode = other.videoMode;
    this->view = other.view;
    this->endGame = other.endGame;
	this->window = new sf::RenderWindow;
    this->window = other.window;
    this->possible_rooms = other.possible_rooms;
    this->current_room = other.current_room;
    this->sfmlEvent = other.sfmlEvent;
    this->player = other.player;
    this->font = other.font;
    this->guiText = other.guiText;
    this->endGameText = other.endGameText;
}
Game& Game::operator=(const Game& other)
{
    if (this != &other)
    {
        this->videoMode = other.videoMode;
        this->view = other.view;
        this->endGame = other.endGame;
		this->window = new sf::RenderWindow;
        this->window = other.window;
        this->possible_rooms = other.possible_rooms;
        this->current_room = other.current_room;
        this->sfmlEvent = other.sfmlEvent;
        this->player = other.player;
        this->font = other.font;
        this->guiText = other.guiText;
        this->endGameText = other.endGameText;
    }
    return *this;
}