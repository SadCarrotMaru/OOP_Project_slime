#pragma once
#include "game_construction.h"
class Player : entity
{
private:
	sf::Sprite model;
	sf::Texture player_model;
	item item_help;

	float movementSpeed;
	int MANA;

	void initVariables();
	void initShape();

public:
	Player(float x = 0.f, float y = 0.f);
	~Player();

	///Accessors
	const int& getHp() const;
	const int& getHpMax() const;
	const sf::Vector2f& getPos() const;

	///Functions
	void takeDamage(const int damage);
	void gainHealth(const int health);

	///Updates
	void updateInput();
	void updateMapBoundsCollision(const sf::RenderTarget* target, const sf::FloatRect rect);
	void update(const sf::RenderTarget* target, const sf::FloatRect rect);
	void render(sf::RenderTarget* target);

	///Output Info
	friend std::ostream& operator<<(std::ostream& os, const Player& player) {
		os << "HP:" << player.HP << ", MAXHP: " << player.MAXHP<< ", Movement speed: " << player.movementSpeed << ", Mana: " << player.MANA << '\n';
		return os;
	}
};