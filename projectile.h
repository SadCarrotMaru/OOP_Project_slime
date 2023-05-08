#pragma once

#include<iostream>
#include<vector>
#include<string>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
class Error : public std::exception {
public:
    explicit Error(const char* message) : message(message) {}
    virtual const char* what() const throw() {
        return message.c_str();
    }
protected:
    std::string message;
};

class TextureError : public Error {
public:
    explicit TextureError(const char* message) : Error(message) {}
};

class FileError : public Error {
public:
    explicit FileError(const char* message) : Error(message) {}
};
class resource_holder {
    public:
        sf::Texture projectile_texture;
        sf::Texture enemy_projectile_texture;
        sf::Texture bat_texture;
        sf::Texture spider_texture;
        sf::Texture fat_guy;
        std::vector<sf::Texture> boss_textures;
        sf::Texture door_texture_north, door_texture_east, door_texture_south, door_texture_west;
        //boss
        resource_holder()
        {
            if (!projectile_texture.loadFromFile("assets/projectile.png"))
            {
                throw FileError("loading projectile failed");
            }
            if (!enemy_projectile_texture.loadFromFile("assets/enemy_projectile.png"))
            {
                throw FileError("loading enemy projectile failed");
            }
            if (!bat_texture.loadFromFile("assets/bat.png"))
            {
                throw FileError("loading bat failed");
            }
            if (!spider_texture.loadFromFile("assets/spider.png"))
            {
                throw FileError("loading spider failed");
            }
            if (!fat_guy.loadFromFile("assets/fat_guy.png"))
            {
                throw FileError("loading fat guy failed");
            }
            for (int i = 1; i <= 8; i++)
            {
                sf::Texture temp;
                if (!temp.loadFromFile("assets/boss/boss" + std::to_string(i) + ".png"))
                {
                    throw FileError("loading boss frame failed");
                }
                boss_textures.push_back(temp);
            }
            if(!this->door_texture_north.loadFromFile("assets/door_north.png"))
            {
                throw FileError("door loading failed - N");
            }
            if (!this->door_texture_east.loadFromFile("assets/door_east.png"))
            {
                throw FileError("door loading failed - E");
            }
            if (!this->door_texture_south.loadFromFile("assets/door_south.png"))
            {
                throw FileError("door loading failed - S");
            }
            if (!this->door_texture_west.loadFromFile("assets/door_west.png"))
            {
                throw FileError("door loading failed - W");
            }
        }

};
class projectile
{
protected:
    sf::Sprite projectile_sprite;
    std::string type_of_projectile;
    sf::Vector2f direction;
    sf::Vector2f destination;
    float projectile_speed;
public:
    projectile(const std::string& type_of_projectile_, const sf::Vector2f& direction_, float projectile_speed_, const sf::Vector2f& destination_, sf::Vector2f playerpos_, resource_holder &rh);
    void update();
    void render(sf::RenderTarget* target);
    bool check(const sf::FloatRect rect);
    sf::FloatRect get_projectile() const;
    projectile(const projectile& other): projectile_sprite(other.projectile_sprite), type_of_projectile(other.type_of_projectile), direction(other.direction), destination(other.destination), projectile_speed(other.projectile_speed) {

    }
    projectile& operator=(const projectile& other)
    {
        this->projectile_sprite = other.projectile_sprite;
        this->type_of_projectile = other.type_of_projectile;
        this->direction = other.direction;
        this->destination = other.destination;
        this->projectile_speed = other.projectile_speed;
        return *this;
    }
};