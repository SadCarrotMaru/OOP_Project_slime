#include "player.h"

void Player::initVariables()
{
	this->movementSpeed = 5.f;
	this->MAXHP = 100;
	this->HP = MAXHP;
}

void Player::initShape()
{
	sf::Texture player_model;
    if (!player_model.loadFromFile("assets/player.png"))
    {
        cout<<"Could not load player model";
		return; /// end the whole thing
    }
    this->model.setTexture(player_model);
}

Player::Player(float x, float y)
{
	this->model.setPosition(x, y);
	this->initVariables();
	this->initShape();
}

Player::~Player()
{

}

//Accessors
/*
const sf::RectangleShape & Player::getShape() const
{
	return this->shape;
}
*/

const int & Player::getHp() const
{
	return this->HP;
}

const int & Player::getHpMax() const
{
	return this->MAXHP;
}

//Functions
void Player::takeDamage(const int damage)
{
	if (this->HP > 0) this->HP -= damage;
	if (this->HP < 0) this->HP = 0;
}

void Player::gainHealth(const int health)
{
	if (this->HP < this->MAXHP)	this->HP += health;
    if (this->HP > this->MAXHP) this->HP = this->MAXHP;
}

void Player::updateInput()
{
	//Keyboard input (WASD || arrows)
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		this->model.move(-this->movementSpeed, 0.f);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)|| sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		this->model.move(this->movementSpeed, 0.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)|| sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		this->model.move(0.f, -this->movementSpeed);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)|| sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		this->model.move(0.f, this->movementSpeed);
	}
}

void Player::updateWindowBoundsCollision(const sf::RenderTarget * target)
{
	//Left
	if (this->model.getGlobalBounds().left <= 0.f)
		this->model.setPosition(0.f, this->model.getGlobalBounds().top);
	//Right
	if (this->model.getGlobalBounds().left + this->model.getGlobalBounds().width >= target->getSize().x)
		this->model.setPosition(target->getSize().x - this->model.getGlobalBounds().width, this->model.getGlobalBounds().top);
	//Top
	if (this->model.getGlobalBounds().top <= 0.f)
		this->model.setPosition(this->model.getGlobalBounds().left, 0.f);
	//Bottom
	if (this->model.getGlobalBounds().top + this->model.getGlobalBounds().height >= target->getSize().y)
		this->model.setPosition(this->model.getGlobalBounds().left, target->getSize().y - this->model.getGlobalBounds().height);
}

void Player::update(const sf::RenderTarget* target)
{
	this->updateInput();

	//Window bounds collision
	this->updateWindowBoundsCollision(target);
}

void Player::render(sf::RenderTarget * target)
{
	target->draw(this->model);
}