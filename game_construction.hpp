#pragma once
#include<iostream>
#include<vector>
#include<string>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
class effect
{
    protected:
        std::string effect_name;
        std::vector<std::pair<std::string,std::string>>modifiers;
};
class projectile
{
    protected:
        std::vector<std::pair<int,int>>coordinates_affected;
        std::string type_of_projectile;
        std::string direction;
        effect effect_on_projectile;
};
class ability
{
    protected:
        std::string ability_name;
        projectile area_affected_by_ability;
};
class weapon
{
    protected:
        std::string weapon_name;
        ability weapon_ability;
        projectile area_affected_by_weapon;
};
class item
{
    protected:
        std::string item_name;
        std::string class_affected;
        std::vector<effect>effects;
};
class entity
{
    protected: 
        int HP, MAXHP;
        weapon weapon_held;
        effect effect_applied_on_entity;
    public:
        entity()
        {
            MAXHP = 100;
            HP = MAXHP;
        }
};
class enemy : protected entity
{
    private:
        //bool phase2 = false;
        std::string enemy_name;
};
class misc
{
    protected:
        sf::Sprite sprite_misc;
};

class room
{
    private:
        sf::Texture door_texture;
        sf::Sprite door_sprite_north, door_sprite_south, door_sprite_east, door_sprite_west;
        sf::Sprite sprite;
        sf::Vector2u size;
        sf::FloatRect rectangle;
        float pixel_error_x=0.f, pixel_error_y=0.f;

        std::vector<misc>objects;
        std::vector<enemy>enemies;
        std::vector<item>items;
        bool door_directions[4]={false,false,false,false};
        // bool final_boss_level;
        sf::Texture background;
    public:
        room() = default;
        room(const std::string& background_location,const std::string &directions,const float pixel_error_x_,const float pixel_error_y_)
        {
            pixel_error_x = pixel_error_x_;
            pixel_error_y = pixel_error_y_;
            if (directions[0] == '1') door_directions[0] = true;
            if (directions[1] == '1') door_directions[1] = true;
            if (directions[2] == '1') door_directions[2] = true;
            if (directions[3] == '1') door_directions[3] = true;
            if (!background.loadFromFile(background_location))
            {
                std::cout << "loading failed";
                /// throw error
                return;
            }
            if(!this->door_texture.loadFromFile("assets/door.png"))
            {
                std::cout << "loading failed";
                /// throw error
                return;
            }

        }
        ~room()
        {
            enemies.clear();
            objects.clear();
            items.clear();
        }
        inline sf::FloatRect getRectangle() const
        {
            return rectangle;
        }
        inline sf::FloatRect getBackgroundRectangle() const
        {
            return sprite.getLocalBounds();
        }
        void get_into_room()
        {
            size = background.getSize();
            sprite.setTexture(background);
            sprite.setOrigin(0.f,0.f);
            door_sprite_north.setTexture(door_texture);
            door_sprite_south.setTexture(door_texture);
            door_sprite_east.setTexture(door_texture);
            door_sprite_west.setTexture(door_texture);
            this->rectangle = sprite.getLocalBounds();
            this->rectangle.left += this->pixel_error_x;
            this->rectangle.top += this->pixel_error_y;
            this->rectangle.width -= 2*this->pixel_error_x;
            this->rectangle.height -= 2*this->pixel_error_y;
            door_sprite_north.setOrigin(0.f,0.f);
            door_sprite_north.setPosition(this->rectangle.left+rectangle.width/2-door_texture.getSize().x/2, pixel_error_y-door_texture.getSize().y);
            door_sprite_south.setOrigin(0.f,0.f);
            door_sprite_south.setPosition(this->rectangle.left+rectangle.width/2,this->rectangle.top+this->rectangle.height-pixel_error_y);
            door_sprite_east.setOrigin(0.f,0.f);
            door_sprite_east.setPosition(this->rectangle.left+this->rectangle.width-pixel_error_x,this->rectangle.top+this->rectangle.height/2);
            door_sprite_west.setOrigin(0.f,0.f);
            door_sprite_west.setPosition(this->pixel_error_x,this->rectangle.top+this->rectangle.height/2);
            std::cout << rectangle.top << ' ' << rectangle.left << ' ' << rectangle.width << ' ' << rectangle.height << '\n';
        }
        void display_background(sf::RenderTarget* target)
        {
            target->draw(sprite);
            if(door_directions[0]) target->draw(door_sprite_north);
            if(door_directions[1]) target->draw(door_sprite_south);
            if(door_directions[2]) target->draw(door_sprite_east);
            if(door_directions[3]) target->draw(door_sprite_west);

        }
        ///copy constructor
        room(const room& other): door_texture(other.door_texture), door_sprite_north(other.door_sprite_north), door_sprite_south(other.door_sprite_south), door_sprite_east(other.door_sprite_east), door_sprite_west(other.door_sprite_west), sprite(other.sprite), size(other.size), rectangle(other.rectangle), pixel_error_x(other.pixel_error_x), pixel_error_y(other.pixel_error_y), objects(other.objects), enemies(other.enemies), items(other.items), door_directions{other.door_directions[0],other.door_directions[1],other.door_directions[2],other.door_directions[3]}, background(other.background)
        {
            /// copy constructor called
        }
        //copy constructor overloading =
        room& operator=(const room& room_to_copy)
        {
            this->background = room_to_copy.background;
            this->door_directions[0] = room_to_copy.door_directions[0];
            this->door_directions[1] = room_to_copy.door_directions[1];
            this->door_directions[2] = room_to_copy.door_directions[2];
            this->door_directions[3] = room_to_copy.door_directions[3];
            this->enemies = room_to_copy.enemies;
            this->items = room_to_copy.items;
            this->objects = room_to_copy.objects;
            this->pixel_error_x = room_to_copy.pixel_error_x;
            this->pixel_error_y = room_to_copy.pixel_error_y;
            this->rectangle = room_to_copy.rectangle;
            this->size = room_to_copy.size;
            this->sprite = room_to_copy.sprite;
            this->door_texture = room_to_copy.door_texture;
            this->door_sprite_north = room_to_copy.door_sprite_north;
            this->door_sprite_south = room_to_copy.door_sprite_south;
            this->door_sprite_east = room_to_copy.door_sprite_east;
            this->door_sprite_west = room_to_copy.door_sprite_west;
            return *this;
        }
        ///overload << operator
        friend std::ostream& operator<<(std::ostream& os, const room& room_to_display)
        {
            os << "room: " << room_to_display.rectangle.left << ' ' << room_to_display.rectangle.top << ' ' << room_to_display.rectangle.width << ' ' << room_to_display.rectangle.height << ' ' << room_to_display.door_directions[0] << ' ' << room_to_display.door_directions[1] << ' ' << room_to_display.door_directions[2] << ' ' << room_to_display.door_directions[3] << '\n';
            return os;
        }
        //void add_enemy(enemy enemy_to_add) { enemies.push_back(enemy_to_add); }
        //void add_object(misc object) { objects.push_back(object); }
        //void add_item(item item_to_add) { items.push_back(item_to_add); }
};
