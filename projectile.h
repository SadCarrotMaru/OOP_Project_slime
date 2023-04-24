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
        resource_holder()
        {
            if (!projectile_texture.loadFromFile("assets/projectile.png"))
            {
                throw FileError("loading projectile failed");
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