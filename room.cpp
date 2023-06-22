#include "room.h"
void room::setheart(bool value_)
{
    if (value_ == true)
    {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<int> dist(0, 2);
        auto is_dropped = dist(mt);
        if (is_dropped == 1)
        {
            is_dropped_heart = true;
            if (!heart.loadFromFile("assets/full_hp.png"))
            {
                throw FileError("dropped heart png failed");
            }
            dropped_heart.setTexture(heart);
            sf::Vector2f pos;
            pos.x = this->rectangle.left + this->rectangle.width / 2;
            pos.y = this->rectangle.top + this->rectangle.height / 2;
            dropped_heart.setPosition(pos);
        }
    }
    else is_dropped_heart = value_;
}
void room::get_into_room(const resource_holder& rh)
{
    size = background.getSize();
    sprite.setTexture(background);
    sprite.setOrigin(0.f, 0.f);
    door_sprite_north.setTexture(rh.door_texture_north);
    door_sprite_south.setTexture(rh.door_texture_south);
    door_sprite_east.setTexture(rh.door_texture_east);
    door_sprite_west.setTexture(rh.door_texture_west);
    this->rectangle = sprite.getLocalBounds();
    this->rectangle.left += this->pixel_error_x;
    this->rectangle.top += this->pixel_error_y;
    this->rectangle.width -= 2 * this->pixel_error_x;
    this->rectangle.height -= 2 * this->pixel_error_y;
    door_sprite_north.setOrigin(0.f, 0.f);
    door_sprite_north.setPosition(this->rectangle.left + rectangle.width / 2 - (float)rh.door_texture_north.getSize().x / 2, pixel_error_y - (float)rh.door_texture_north.getSize().y);
    door_sprite_south.setOrigin(0.f, 0.f);
    door_sprite_south.setPosition(this->rectangle.left + rectangle.width / 2 - (float)rh.door_texture_south.getSize().x / 2, this->rectangle.top + this->rectangle.height - 10);
    door_sprite_east.setOrigin(0.f, 0.f);
    door_sprite_east.setPosition(this->rectangle.left + this->rectangle.width, this->rectangle.top + this->rectangle.height / 2 - (float)rh.door_texture_east.getSize().y / 2);
    door_sprite_west.setOrigin(0.f, 0.f);
    door_sprite_west.setPosition(this->rectangle.left - (float)rh.door_texture_west.getSize().x, this->rectangle.top + this->rectangle.height / 2 - (float)rh.door_texture_west.getSize().y / 2);
   // std::cout << rectangle.top << ' ' << rectangle.left << ' ' << rectangle.width << ' ' << rectangle.height << '\n';
}
void room::display_background(sf::RenderTarget* target)
{
    if (sprite.getLocalBounds().width == 0) throw TextureError("background");
    if (door_directions[0] == true && door_sprite_north.getLocalBounds().width == 0) throw TextureError("door-north");
    if (door_directions[1] == true && door_sprite_south.getLocalBounds().width == 0) throw TextureError("door-south");
    if (door_directions[2] == true && door_sprite_east.getLocalBounds().width == 0) throw TextureError("door-east");
    if (door_directions[3] == true && door_sprite_west.getLocalBounds().width == 0) throw TextureError("door-west");
    target->draw(sprite);
    if (door_directions[0]) target->draw(door_sprite_north);
    if (door_directions[1]) target->draw(door_sprite_south);
    if (door_directions[2]) target->draw(door_sprite_east);
    if (door_directions[3]) target->draw(door_sprite_west);
    if (is_dropped_heart)
    {
        target->draw(dropped_heart);
    }
}
void room::create_enemy(sf::FloatRect redzone, const resource_holder & rh)
{
    this->enemies.clear();
    sf::FloatRect help = this->getRectangle();
    int xx = help.left, xx2 = xx + help.width;
    int yy = help.top, yy2 = yy + help.height;

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(xx, xx2);
    std::uniform_int_distribution<int> dist2(yy, yy2);
    ///type of enemies
    /// fat guy with projectiles
    /// small spiders, speedy, charge randomly
    /// boss
    /// bats, fly around
    std::uniform_int_distribution<int> dist3(0, 2);
    int type = dist3(mt);
    int nrenemy1, nrenemy2;
    if (type == 0) nrenemy1 = 2, nrenemy2 = 4;
    else if (type == 1) nrenemy1 = 6, nrenemy2 = 12;
    else nrenemy1 = 4, nrenemy2 = 8;


    std::uniform_int_distribution<int>dist4(nrenemy1, nrenemy2);
    int number_of_enemies = dist4(mt);
    if (this->getBackgroundRectangle().width > 2000) number_of_enemies *= 2;
    for (int z = 1; z <= number_of_enemies; z++)
    {
        while (1)
        {
            int x = dist(mt);
            int y = dist2(mt);
            sf::Vector2f tempvec(x, y);
            sf::FloatRect help2(x, y, 150, 150);
            if (collision::collisionsprites(help2, redzone) == false)
            {
                if (type == 0)
                {
                    std::unique_ptr<enemy> temp(std::make_unique<fatguy>(enemy_factory::fatguy_(tempvec, rh)));
                    this->enemies.push_back(std::move(temp));
                }
                else if (type == 1)
                {
                    std::unique_ptr<enemy> temp(std::make_unique<spider>(enemy_factory::spider_(tempvec, rh)));
                    this->enemies.push_back(std::move(temp));
                }
                else
                {
                    std::unique_ptr<enemy> temp(std::make_unique<bat>(enemy_factory::bat_(tempvec, rh)));
                    this->enemies.push_back(std::move(temp));
                }
                break;
            }
        }
    }
}
//get enemies
const std::vector<enemy*>& room::getEnemies()
{
    static std::vector<enemy*> temp;
    temp.clear();
    while (this->enemies.size() > 0)
    {
        enemy* ptr = this->enemies[0].get();
        temp.push_back(dynamic_cast<enemy*>(ptr->clone()));
        this->enemies.erase(this->enemies.begin());
    }
    this->enemies.clear();
    return temp;
}