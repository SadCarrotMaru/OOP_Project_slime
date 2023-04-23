#include "projectile.h"

projectile::projectile(std::string type_of_projectile_, sf::Vector2f direction_, float projectile_speed_, sf::Vector2f destination_, sf::Vector2f playerpos_, resource_holder &rh)
{
    type_of_projectile = type_of_projectile_;
    direction = direction_;
    projectile_speed = projectile_speed_;
    destination = destination_;
    this->projectile_sprite.setTexture(rh.projectile_texture);
    this->projectile_sprite.setPosition(playerpos_);
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