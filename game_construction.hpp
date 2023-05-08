#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<cmath>
#include<random>
#include<chrono>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include "projectile.h"
class effect
{
    protected:
        std::string effect_name;
        std::vector<std::pair<std::string,std::string>>modifiers;
};
/*
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
    public:
        weapon() = default;
};
*/
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
        //weapon weapon_held;
        effect effect_applied_on_entity;
    public:
        entity(int hp_ = 100)
        {
            MAXHP = hp_;
            HP = MAXHP;
        }
        virtual ~entity() = default;
        virtual void getdamage(const int damagetaken) = 0;
        virtual const sf::FloatRect getrect() const = 0;
        virtual int getHP() const = 0;
        virtual entity* clone() const = 0;
        
};
class enemy : public entity
{
    private:
        //bool phase2 = false;
        sf::Sprite model;
        std::string enemy_name;
        float speed;
        int sign1,sign2;
        std::chrono::high_resolution_clock::time_point last = std::chrono::high_resolution_clock::now();
    public:
        enemy(const std::string &name, const sf::Vector2f position, int maxhp, const resource_holder &rh, int sign11 = 1, int sign22 = 1) : entity(maxhp)
        {
            enemy_name = name;
            if(enemy_name == "bat")
            {
                this->speed = 1.5f;
                this->model.setTexture(rh.bat_texture);
                this->model.setOrigin(this->model.getGlobalBounds().width/2, this->model.getGlobalBounds().height/2);
                this->model.setPosition(position);
            }
            else if(enemy_name == "spider")
            {
                this->speed = 6.0f;
                this->model.setTexture(rh.spider_texture);
                this->model.setOrigin(this->model.getGlobalBounds().width/2, this->model.getGlobalBounds().height/2);
                this->model.setPosition(position);
            }
            else if(enemy_name == "fat_guy")
            {
                this->speed = 0.4f;
                this->model.setTexture(rh.fat_guy);
                this->model.setOrigin(this->model.getGlobalBounds().width/2, this->model.getGlobalBounds().height/2);
                this->model.setPosition(position);
            }
            sign1 = sign11;
            sign2 = sign22;
        };
        void movement_update(const sf::Vector2f player_pos, std::vector<projectile> &enemy_projectiles,sf::FloatRect roomwall,resource_holder &rh)
        {
            if(enemy_name == "fat_guy")
            {
                //move slowly towards player
                sf::Vector2f direction = player_pos - this->model.getPosition();
                float length = float(sqrt(pow(direction.x,2) + pow(direction.y,2)));
                direction.x /= length;
                direction.y /= length;
                this->model.move(direction * this->speed);
                //shoot projectile every 3 seconds
                std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
                if(std::chrono::duration_cast<std::chrono::seconds>(now - last).count() >= 3)
                {
                    last = now;
                    enemy_projectiles.push_back(projectile("enemy", direction, 24.0f, player_pos, this->model.getPosition(), rh));
                }
            }
            else if(enemy_name == "spider")
            {
                // sprint towards player (mach type of speed)
                sf::Vector2f direction = player_pos - this->model.getPosition();
                float length = sqrt(pow(direction.x,2) + pow(direction.y,2));
                direction.x /= length;
                direction.y /= length;
                this->model.move(direction * this->speed);
            }
            else if(enemy_name == "bat")
            {
                this->model.move(this->speed*10*sign1, this->speed*10*sign2);
                //collision with wall
               // std::cout<<this->model.getGlobalBounds().left<<" "<<this->model.getGlobalBounds().top<<" "<< this->model.getGlobalBounds().width<<" "<<this->model.getGlobalBounds().height<<std::endl;
                if (this->model.getGlobalBounds().left <= roomwall.left) sign1=1;
                if (this->model.getGlobalBounds().left + this->model.getGlobalBounds().width >= roomwall.left + roomwall.width) sign1=-1;
                if (this->model.getGlobalBounds().top <= roomwall.top) sign2=1;
                if (this->model.getGlobalBounds().top + this->model.getGlobalBounds().height >= roomwall.top + roomwall.height) sign2=-1;
            }
        }
        ~enemy() = default; 
        void getdamage (const int damagetaken) override
        {
            this->HP -= damagetaken;
        }
        int getHP() const override
        {
            return this->HP;
        }
        const sf::FloatRect getrect() const override
        {
            return this->model.getGlobalBounds();
        }
        void render(sf::RenderTarget * target) const
        {
            target->draw(this->model);
        }
        entity* clone() const override { return new enemy(*this);}
};
class misc
{
    protected:
        sf::Sprite sprite_misc;
};

class collision
{
    public:
        static bool collisionsprites(sf::FloatRect rect1, sf::FloatRect rect2)
        {
           // std::cout << rect1.top << ' ' << rect1.height << ' ' << rect1.left << ' ' << rect1.width << std::endl;
           // std::cout << rect2.top << ' ' << rect2.height << ' ' << rect2.left << ' ' << rect2.width << std::endl;
            if (rect1.intersects(rect2))
            {
                return true;
            }
            return false;
        }
};



class room
{
    private:
        sf::Sprite door_sprite_north, door_sprite_south, door_sprite_east, door_sprite_west;
        sf::Sprite sprite;
        sf::Vector2u size;
        sf::FloatRect rectangle;
        float pixel_error_x=0.f, pixel_error_y=0.f;
        std::vector<std::unique_ptr<misc>>objects;
        std::vector<std::unique_ptr<enemy>>enemies;
        std::vector<std::unique_ptr<item>>items;
        
        bool door_directions[4]={false,false,false,false};

        sf::Texture background;
        bool level_cleared = false;
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
                throw FileError("background loading failed");
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
        inline sf::FloatRect get_north() const
        {
            return door_sprite_north.getGlobalBounds();
        }
        inline sf::FloatRect get_south() const
        {
            return door_sprite_south.getGlobalBounds();
        }
        inline sf::FloatRect get_east() const
        {
            return door_sprite_east.getGlobalBounds();
        }
        inline sf::FloatRect get_west() const
        {
            return door_sprite_west.getGlobalBounds();
        }
        inline bool getdoor(int poz) const
        {
            return this->door_directions[poz];
        }
        inline bool get_level_clear() const
        {
            return this->level_cleared;
        }
        void set_door(int poz)
        {
            this->door_directions[poz] = true;
        }
        void set_level_clear(bool value)
        {
            this->level_cleared = value;
        }
        void get_into_room(const resource_holder &rh)
        {
            size = background.getSize();
            sprite.setTexture(background);
            sprite.setOrigin(0.f,0.f);
            door_sprite_north.setTexture(rh.door_texture_north);
            door_sprite_south.setTexture(rh.door_texture_south);
            door_sprite_east.setTexture(rh.door_texture_east);
            door_sprite_west.setTexture(rh.door_texture_west);
            this->rectangle = sprite.getLocalBounds();
            this->rectangle.left += this->pixel_error_x;
            this->rectangle.top += this->pixel_error_y;
            this->rectangle.width -= 2*this->pixel_error_x;
            this->rectangle.height -= 2*this->pixel_error_y;
            door_sprite_north.setOrigin(0.f,0.f);
            door_sprite_north.setPosition(this->rectangle.left+rectangle.width/2-(float)rh.door_texture_north.getSize().x/2, pixel_error_y-(float)rh.door_texture_north.getSize().y);
            door_sprite_south.setOrigin(0.f,0.f);
            door_sprite_south.setPosition(this->rectangle.left+rectangle.width/2-(float)rh.door_texture_south.getSize().x/2, this->rectangle.top+this->rectangle.height - 10 );
            door_sprite_east.setOrigin(0.f,0.f);
            door_sprite_east.setPosition(this->rectangle.left+this->rectangle.width, this->rectangle.top + this->rectangle.height / 2 - (float)rh.door_texture_east.getSize().y / 2);
            door_sprite_west.setOrigin(0.f,0.f);
            door_sprite_west.setPosition(this->rectangle.left-(float)rh.door_texture_west.getSize().x, this->rectangle.top + this->rectangle.height / 2 - (float)rh.door_texture_west.getSize().y / 2);
            std::cout << rectangle.top << ' ' << rectangle.left << ' ' << rectangle.width << ' ' << rectangle.height << '\n';
        }
        void display_background(sf::RenderTarget* target)
        {
            if (sprite.getLocalBounds().width == 0) throw TextureError("background");
            if (door_directions[0] == true && door_sprite_north.getLocalBounds().width == 0) throw TextureError("door-north");
            if (door_directions[1] == true && door_sprite_south.getLocalBounds().width == 0) throw TextureError("door-south");
            if (door_directions[2] == true && door_sprite_east.getLocalBounds().width == 0) throw TextureError("door-east");
            if (door_directions[3] == true && door_sprite_west.getLocalBounds().width == 0) throw TextureError("door-west");
            target->draw(sprite);
            if(door_directions[0]) target->draw(door_sprite_north);
            if(door_directions[1]) target->draw(door_sprite_south);
            if(door_directions[2]) target->draw(door_sprite_east);
            if(door_directions[3]) target->draw(door_sprite_west);

        }
        void create_enemy(sf::FloatRect redzone, const resource_holder &rh)
        {
            this->enemies.clear();
            sf::FloatRect help = this->getRectangle();
            int xx = help.left, xx2 = xx + help.width;
            int yy = help.top, yy2 = yy = help.height;

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
            int nrenemy1 , nrenemy2;
            if(type==0) nrenemy1 = 2, nrenemy2 = 4;
                else if(type==1) nrenemy1 = 6, nrenemy2 = 12;
                    else nrenemy1 = 4, nrenemy2 = 8;


            std::uniform_int_distribution<int>dist4(nrenemy1,nrenemy2);
            int number_of_enemies = dist4(mt);
            if (this->getBackgroundRectangle().width > 2000) number_of_enemies *= 2;
            for(int z = 1; z<=  number_of_enemies; z++)
            {
                while(1)
                {
                    int x = dist(mt);
                    int y = dist2(mt);
                    sf::Vector2f tempvec(x,y);
                    sf::FloatRect help2(x, y, 150, 150);
                    if (collision::collisionsprites(help2, redzone) == false)
                    {
                        if(type==0)
                        {
                            std::unique_ptr<enemy> temp(new enemy("fat_guy", tempvec, 150, rh));
                            this->enemies.push_back(std::move(temp));
                        }
                        else if(type==1)
                        {
                            std::unique_ptr<enemy> temp(new enemy("spider", tempvec, 60, rh));
                            this->enemies.push_back(std::move(temp));
                        }
                        else
                        {
                            std::uniform_int_distribution<int>dist5(0,1);
                            int sign1 = dist5(mt), sign2 = dist5(mt);
                            if(sign1 == 0) sign1--;
                            if(sign2 == 0) sign2--;
                            std::unique_ptr<enemy> temp(new enemy("bat", tempvec, 20, rh, sign1, sign2));
                            this->enemies.push_back(std::move(temp));
                        }
                        break;
                    }
                }
            }
        }
        //get enemies
        const std::vector<enemy>& getEnemies()
        {
            static std::vector<enemy> temp;
            temp.clear();
            while(this->enemies.size()>0)
            {
                enemy* ptr = this->enemies[0].get();
                temp.push_back(*ptr);
                this->enemies.erase(this->enemies.begin());
            }
            this->enemies.clear();
            return temp;
        }
        ///copy constructor
        room(const room& other):
            door_sprite_north(other.door_sprite_north), 
            door_sprite_south(other.door_sprite_south), 
            door_sprite_east(other.door_sprite_east), 
            door_sprite_west(other.door_sprite_west), 
            sprite(other.sprite), size(other.size), 
            rectangle(other.rectangle), 
            pixel_error_x(other.pixel_error_x), 
            pixel_error_y(other.pixel_error_y),
            door_directions{other.door_directions[0],other.door_directions[1],other.door_directions[2],other.door_directions[3]}, 
            background(other.background), 
            level_cleared(other.level_cleared)
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
            this->pixel_error_x = room_to_copy.pixel_error_x;
            this->pixel_error_y = room_to_copy.pixel_error_y;
            this->rectangle = room_to_copy.rectangle;
            this->size = room_to_copy.size;
            this->sprite = room_to_copy.sprite;
            this->door_sprite_north = room_to_copy.door_sprite_north;
            this->door_sprite_south = room_to_copy.door_sprite_south;
            this->door_sprite_east = room_to_copy.door_sprite_east;
            this->door_sprite_west = room_to_copy.door_sprite_west;
            this->level_cleared = room_to_copy.level_cleared;
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
