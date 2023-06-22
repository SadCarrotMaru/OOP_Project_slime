#pragma once
#include "room.h"
class Player : public entity
{
private:
	sf::Sprite model;
	sf::Texture player_model;
	std::chrono::high_resolution_clock::time_point last = std::chrono::high_resolution_clock::now();
	//item item_help;

	float movementSpeed{};
	//int MANA;

	void initVariables();
	void initShape();

public:
	explicit Player(float x = 0.f, float y = 0.f) ;
	~Player() = default;

	///Accessors
	const int& getHp() const;
	const sf::Vector2f getModelCoord() const;
	//const int& getHpMax() const;
	//const sf::Vector2f& getPos() const;

	///Functions
	//void takeDamage(const int damage);
	//void gainHealth(const int health);
	void getdamage(const int damagetaken) override;

	///Updates
	void setPosition(float x, float y);
	void updateTime();
	void updateInput();
	void updateModel_progress(int state);
	void updateMapBoundsCollision(sf::FloatRect rect);
	int update(sf::FloatRect rect, room* currentroom);
	void render(sf::RenderTarget* target) const;
	const sf::FloatRect getrect() const override;
    int getHP() const override;

	int check_doors(room* currentroom);
	entity* clone() const override { return new Player(*this); }

	///Output Info
	friend std::ostream& operator<<(std::ostream& os, const Player& player) {
		os << "Coordinates" << player.model.getPosition().x <<' ' <<player.model.getPosition().y << ' ' <<  "HP:" << player.HP << ", MAXHP: " << player.MAXHP << ", Movement speed: " << player.movementSpeed << '\n';
		return os;
	}
};