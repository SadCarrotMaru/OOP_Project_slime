#pragma once
#include "game_construction.h"
class Player : entity
{
private:
	sf::Sprite model;
	item item_help;

	float movementSpeed;
	int MANA;

	void initVariables();
	void initShape();

public:
	Player(float x = 0.f, float y = 0.f);
	virtual ~Player();

	///Accessors
	const sf::Sprite& getShape() const;
	const int& getHp() const;
	const int& getHpMax() const;

	///Functions
	void takeDamage(const int damage);
	void gainHealth(const int health);

	///Updates
	void updateInput();
	void updateWindowBoundsCollision(const sf::RenderTarget* target);
	void update(const sf::RenderTarget* target);
	void render(sf::RenderTarget* target);
};