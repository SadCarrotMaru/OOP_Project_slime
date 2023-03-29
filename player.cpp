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
    if (!texture.loadFromFile("assets/player.png"))
    {
        cout<<"Could not load player model";
        return 0;
    }
    this->model.setTexture(player_model);
}

Player::Player(float x, float y)
{
	this->shape.setPosition(x, y);
	this->initVariables();
	this->initShape();
}

Player::~Player()
{

}

//Accessors
const sf::RectangleShape & Player::getShape() const
{
	return this->shape;
}

const int & Player::getHp() const
{
	return this->hp;
}

const int & Player::getHpMax() const
{
	return this->hpMax;
}

//Functions
void Player::takeDamage(const int damage)
{
	if (this->hp > 0) this->hp -= damage;
	if (this->hp < 0) this->hp = 0;
}

void Player::gainHealth(const int health)
{
	if (this->hp < this->hpMax)	this->hp += health;
    if (this->hp > this->hpMax) this->hp = this->hpMax;
}

void Player::updateInput()
{
	//Keyboard input (WASD || arrows)
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		this->shape.move(-this->movementSpeed, 0.f);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)|| sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		this->shape.move(this->movementSpeed, 0.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)|| sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		this->shape.move(0.f, -this->movementSpeed);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)|| sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		this->shape.move(0.f, this->movementSpeed);
	}
}

void Player::updateWindowBoundsCollision(const sf::RenderTarget * target)
{
	//Left
	if (this->shape.getGlobalBounds().left <= 0.f)
		this->shape.setPosition(0.f, this->shape.getGlobalBounds().top);
	//Right
	if (this->shape.getGlobalBounds().left + this->shape.getGlobalBounds().width >= target->getSize().x)
		this->shape.setPosition(target->getSize().x - this->shape.getGlobalBounds().width, this->shape.getGlobalBounds().top);
	//Top
	if (this->shape.getGlobalBounds().top <= 0.f)
		this->shape.setPosition(this->shape.getGlobalBounds().left, 0.f);
	//Bottom
	if (this->shape.getGlobalBounds().top + this->shape.getGlobalBounds().height >= target->getSize().y)
		this->shape.setPosition(this->shape.getGlobalBounds().left, target->getSize().y - this->shape.getGlobalBounds().height);
}

void Player::update(const sf::RenderTarget* target)
{
	this->updateInput();

	//Window bounds collision
	this->updateWindowBoundsCollision(target);
}

void Player::render(sf::RenderTarget * target)
{
	target->draw(this->shape);
}