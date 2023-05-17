#include "projectile.h"

projectile::projectile(const std::string &type_of_projectile_, const sf::Vector2f &direction_, float projectile_speed_, sf::Vector2f startpos_, resource_holder &rh) : type_of_projectile (type_of_projectile_), direction(direction_)
{
    projectile_speed = projectile_speed_;
    if(type_of_projectile_ == "allied") this->projectile_sprite.setTexture(rh.projectile_texture);
        else this->projectile_sprite.setTexture(rh.enemy_projectile_texture);
    if(type_of_projectile_ == "boss_projectile") this->projectile_sprite.scale(2.0f,2.0f);
    this->projectile_sprite.setPosition(startpos_);
    this->projectile_sprite.setOrigin(25.0f, 25.0f);
}
void projectile::update()
{
    this->projectile_sprite.move(this->direction * this->projectile_speed);
}
bool projectile::check(const sf::FloatRect rect)
{
    if (this->projectile_sprite.getGlobalBounds().left <= rect.left) return true;
    if (this->projectile_sprite.getGlobalBounds().left + this->projectile_sprite.getGlobalBounds().width >= rect.left + rect.width) return true;
    if (this->projectile_sprite.getGlobalBounds().top <= rect.top) return true;
    if (this->projectile_sprite.getGlobalBounds().top + this->projectile_sprite.getGlobalBounds().height >= rect.top + rect.height) return true;
    return false;
}
void projectile::render(sf::RenderTarget* target)
{
    target->draw(this->projectile_sprite);
}
sf::FloatRect projectile::get_projectile() const
{
    return this->projectile_sprite.getGlobalBounds();
}