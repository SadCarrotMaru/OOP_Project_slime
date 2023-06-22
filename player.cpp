#include "player.h"

void Player::getdamage(const int damagetaken)
{
	std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
	if (std::chrono::duration_cast<std::chrono::seconds>(now - last).count() >= 0.3)
	{
		this->HP -= damagetaken;
		last = std::chrono::high_resolution_clock::now();
		if (this->HP > this->MAXHP)
			this->HP = this->MAXHP;
	}
}
void Player::updateTime()
{
	last = std::chrono::high_resolution_clock::now();
}
void Player::initVariables()
{
	this->movementSpeed = 10.f;
	//this->MANA = 30;
}

void Player::initShape()
{
    if (!player_model.loadFromFile("assets/player.png"))
    {
		throw FileError("loading player failed");
    }
    this->player_model.setSmooth(true);
    this->model.setTexture(player_model);
}

Player::Player(float x, float y) : entity()
{
	this->model.setPosition(x, y);
	this->initVariables();
	this->initShape();
}

void Player::updateModel_progress(int state)
{
	if (state == 1)
	{
		if (!player_model.loadFromFile("assets/player1.png"))
		{
			throw FileError("loading player failed");
		}
	}
	else if (state == 2)
	{
		if (!player_model.loadFromFile("assets/player2.png"))
		{
			throw FileError("loading player failed");
		}
	}
	this->player_model.setSmooth(true);
	this->model.setTexture(player_model);
}

//Accessors

const int & Player::getHp() const
{
	return this->HP;
}

/*
const int & Player::getHpMax() const
{
	return this->MAXHP;
}

const sf::Vector2f& Player::getPos() const
{
	return this->model.getPosition();
}

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
*/
void Player::setPosition(float x, float y)
{
    this->model.setPosition(x, y);
}
const sf::Vector2f Player::getModelCoord() const
{
	sf::Vector2f temp = this->model.getPosition();
	temp.x += this->model.getGlobalBounds().width / 2;
	temp.y += this->model.getGlobalBounds().height / 2;
	return temp;
}
int Player::check_doors(room* currentroom)
{
	if(collision::collisionsprites(this->model.getGlobalBounds(),currentroom->get_north()) && currentroom->getdoor(0) == true)
		return 0;
	if (collision::collisionsprites(this->model.getGlobalBounds(), currentroom->get_south()) && currentroom->getdoor(1) == true)
		return 1;
	if (collision::collisionsprites(this->model.getGlobalBounds(), currentroom->get_east()) && currentroom->getdoor(2) == true)
		return 2;
	if (collision::collisionsprites(this->model.getGlobalBounds(), currentroom->get_west()) && currentroom->getdoor(3) == true)
		return 3;
	return 4;

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

void Player::updateMapBoundsCollision(const sf::FloatRect rect)
{
	//Left
	if (this->model.getGlobalBounds().left <= rect.left)
		this->model.setPosition(rect.left, this->model.getGlobalBounds().top);
	//Right
	if (this->model.getGlobalBounds().left + this->model.getGlobalBounds().width >= rect.left+rect.width)
		this->model.setPosition(rect.left+rect.width-this->model.getGlobalBounds().width, this->model.getGlobalBounds().top);
	//Top
	if (this->model.getGlobalBounds().top <= rect.top)
		this->model.setPosition(this->model.getGlobalBounds().left, rect.top);
	//Bottom
	if (this->model.getGlobalBounds().top + this->model.getGlobalBounds().height >= rect.top+rect.height)
		this->model.setPosition(this->model.getGlobalBounds().left, rect.top+rect.height- this->model.getGlobalBounds().height);
}

int Player::update(const sf::FloatRect rect, room* currentroom)
{
	this->updateInput();
	//Doors
	int temp = this->check_doors(currentroom);
	//Window bounds collision
	this->updateMapBoundsCollision(rect);
	if (temp != 4)
		return temp;
	return 4;
}

void Player::render(sf::RenderTarget * target) const
{
	target->draw(this->model);
}

const sf::FloatRect Player::getrect() const
{
	return this->model.getGlobalBounds();
}
int Player::getHP() const
{
    return this->HP;
}