#include "game.h"

Game::Game()
{
	this->possible_rooms.clear();
	this->initWindow();
}

void Game::initVariables()
{

}

void Game::create_possible_rooms()
{

}

void Game::initWindow()
{
	this->videoMode = sf::VideoMode(900, 900);
	this->window = new sf::RenderWindow(this->videoMode, "Unknown Project", sf::Style::Close | sf::Style::Titlebar);
	this->window->setFramerateLimit(60);
}

Game::~Game()
{
	delete this->window;
}

const bool& Game::getEndGame() const
{
	return this->endGame;
}
const sf::VideoMode& Game::getVideoMode() const
{
	return this->videoMode;
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
			if (this->sfmlEvent.key.code == sf::Keyboard::Z) this->possible_rooms[1].get_into_room();
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
	this->player.update(this->window, this->possible_rooms[1].getRectangle());

	if (this->player.getHp() <= 0)
		this->endGame = true;
}

void Game::setView()
{
	sf::Vector2f center_map;
	center_map.x = (this->possible_rooms[1].getRectangle().left + this->possible_rooms[1].getRectangle().width) / 2;
	center_map.y = (this->possible_rooms[1].getRectangle().top + this->possible_rooms[1].getRectangle().height) / 2;
	//this->view.setCenter(this->player.getPos());
	this->view.setCenter(center_map);
	this->view.zoom(2.0f);
	this->window->setView(this->view);
	this->view.zoom(0.5f);
}

void Game::update()
{
	this->pollEvents();
}

void Game::render()
{
	this->window->clear();
	this->updatePlayer();
	this->setView();
	this->possible_rooms[1].display_background(this->window);
	this->player.render(this->window);
	this->window->display();
}

void Game::create_rooms()
{
	room room1("A://OOP_Project_slime//assets//background1.png", "0000");
	this->possible_rooms.push_back(room1);
	room room2("A://OOP_Project_slime//assets//background1.png", "0000");
	this->possible_rooms.push_back(room2);
}