#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<cmath>
#include<memory>
#include<random>
#include<chrono>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>  
#include "projectile.h"
#include "animation.hpp"
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
        explicit entity(int hp_ = 100)
        {
            MAXHP = hp_;
            HP = MAXHP;
        }
    public:
        virtual ~entity() = default;
        virtual void getdamage(const int damagetaken) = 0;
        virtual const sf::FloatRect getrect() const = 0;
        virtual int getHP() const = 0;
        virtual entity* clone() const = 0;
        
};
class enemy : public entity
{
    protected:
        //bool phase2 = false;
        sf::Sprite model;
        std::string enemy_name;
        float speed;
        int sign1,sign2;
        std::chrono::high_resolution_clock::time_point last = std::chrono::high_resolution_clock::now();
    public:

        explicit enemy(const int maxhp, const std::string& name) : entity(maxhp), enemy_name(name) { this->speed = 0.0f; this->sign1 = this->sign2 = 0; };
        virtual void movement_update([[maybe_unused]] const sf::Vector2f player_pos, [[maybe_unused]] std::vector<projectile>& enemy_projectiles, [[maybe_unused]]sf::FloatRect roomwall, [[maybe_unused]]resource_holder& rh) { std::cout << "test"; };
        virtual ~enemy() = default; 
        void getdamage (const int damagetaken) override
        {
            this->HP -= damagetaken;
        }
        int getHP() const override
        {
            return this->HP;
        }
        virtual const sf::FloatRect getrect() const override
        {
            return this->model.getGlobalBounds();
        }
        virtual void render(sf::RenderTarget * target)
        {
            target->draw(this->model);
        }
        virtual  entity* clone() const override { return new enemy(*this); }
};
class fatguy : public enemy
{
public:
    explicit fatguy(const std::string& name, const sf::Vector2f position, const int maxhp, const resource_holder& rh, int sign11 = 1, int sign22 = 1) : enemy(maxhp, name)
    {
        this->speed = 0.4f;
        this->model.setTexture(rh.fat_guy);
        this->model.setOrigin(this->model.getGlobalBounds().width / 2, this->model.getGlobalBounds().height / 2);
        this->model.setPosition(position);
        sign1 = sign11;
        sign2 = sign22;
    }
    void movement_update(const sf::Vector2f player_pos, std::vector<projectile>& enemy_projectiles, [[maybe_unused]]sf::FloatRect roomwall, resource_holder& rh) override
        {
            //move slowly towards player
            sf::Vector2f direction = player_pos - this->model.getPosition();
            float length = float(sqrt(pow(direction.x,2) + pow(direction.y,2)));
            direction.x /= length;
            direction.y /= length;
            this->model.move(direction * this->speed);
            //shoot projectile every 1.5 seconds
            std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
            if(std::chrono::duration_cast<std::chrono::seconds>(now - last).count() >= 1.5)
            {
                last = now;
                enemy_projectiles.push_back(projectile("enemy", direction, 24.0f, this->model.getPosition(), rh));
            }
        }
        virtual const sf::FloatRect getrect() const override
        {
            return this->model.getGlobalBounds();
        }
        virtual void render(sf::RenderTarget* target) override
        {
            target->draw(this->model);
        }
        entity* clone() const override  { return new fatguy(*this);}
};
class bat : public enemy
{
public:
    explicit bat(const std::string& name, const sf::Vector2f position, const int maxhp, const resource_holder& rh, int sign11 = 1, int sign22 = 1) : enemy(maxhp, name)
    {
        this->speed = 1.5f;
        this->model.setTexture(rh.bat_texture);
        this->model.setOrigin(this->model.getGlobalBounds().width / 2, this->model.getGlobalBounds().height / 2);
        this->model.setPosition(position);
        sign1 = sign11;
        sign2 = sign22;
    }
    void movement_update([[maybe_unused]] const sf::Vector2f player_pos, [[maybe_unused]] std::vector<projectile>& enemy_projectiles, sf::FloatRect roomwall, [[maybe_unused]] resource_holder& rh) override
        {
            this->model.move(this->speed*10*sign1, this->speed*10*sign2);
            //collision with wall
           // std::cout<<this->model.getGlobalBounds().left<<" "<<this->model.getGlobalBounds().top<<" "<< this->model.getGlobalBounds().width<<" "<<this->model.getGlobalBounds().height<<std::endl;
            if (this->model.getGlobalBounds().left <= roomwall.left) sign1=1;
            if (this->model.getGlobalBounds().left + this->model.getGlobalBounds().width >= roomwall.left + roomwall.width) sign1=-1;
            if (this->model.getGlobalBounds().top <= roomwall.top) sign2=1;
            if (this->model.getGlobalBounds().top + this->model.getGlobalBounds().height >= roomwall.top + roomwall.height) sign2=-1;
        }
        virtual const sf::FloatRect getrect() const override
        {
            return this->model.getGlobalBounds();
        }
        virtual void render(sf::RenderTarget* target) override
        {
            target->draw(this->model);
        }
        entity* clone() const override  { return new bat(*this);}
};
class spider : public enemy
{
public:
    spider(const std::string& name, const sf::Vector2f position, const int maxhp, const resource_holder& rh, int sign11 = 1, int sign22 = 1) : enemy(maxhp, name)
    {
        this->speed = 6.0f;
        this->model.setTexture(rh.spider_texture);
        this->model.setOrigin(this->model.getGlobalBounds().width / 2, this->model.getGlobalBounds().height / 2);
        this->model.setPosition(position);
        sign1 = sign11;
        sign2 = sign22;
    }
    void movement_update(const sf::Vector2f player_pos, [[maybe_unused]] std::vector<projectile>& enemy_projectiles, [[maybe_unused]]sf::FloatRect roomwall, [[maybe_unused]]resource_holder& rh) override
        {
            // sprint towards player (mach type of speed)
            sf::Vector2f direction = player_pos - this->model.getPosition();
            float length = sqrt(pow(direction.x, 2) + pow(direction.y, 2));
            direction.x /= length;
            direction.y /= length;
            this->model.move(direction * this->speed);
        }
        virtual const sf::FloatRect getrect() const override
        {
            return this->model.getGlobalBounds();
        }
        virtual void render(sf::RenderTarget* target) override
        {
            target->draw(this->model);
        }
        entity* clone() const override  { return new spider(*this);}
};
class enemy_factory
{
public:
    static spider spider_(sf::Vector2f pos, const resource_holder &rh)  {return spider("spider", pos, 60, rh); }
    static fatguy fatguy_(sf::Vector2f pos, const resource_holder &rh) { return fatguy("fat_guy", pos, 150, rh); }
    static bat bat_(sf::Vector2f pos, const resource_holder &rh)
    {

        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<int>dist5(0, 1);
        int sign1 = dist5(mt), sign2 = dist5(mt);
        if (sign1 == 0) sign1--;
        if (sign2 == 0) sign2--;
        return bat("bat", pos, 20, rh, sign1, sign2);
    }
};
class boss : public enemy
{
    protected:
        int attack_;
        animation sprite_;
        sf::Vector2f boss_pos;
        std::vector<std::shared_ptr<enemy>>buffer_minions;
        sf::Font font;
        sf::Text text;
    public:
        explicit boss(sf::Vector2f pos) : enemy(3500, "final_boss"), sprite_(8, "boss"), boss_pos(pos)
        {
            attack_ = 0;

            if ( !font.loadFromFile( "assets/FontFile.ttf" ) )
            {
                throw FileError("font file error");
            }
            text.setFont( font );

            // in pixels, not points!
            text.setCharacterSize( 256 );

            text.setFillColor( sf::Color::Red );

            text.setStyle( sf::Text::Style::Bold );

            text.setOutlineColor( sf::Color(255,0,0,10));
            text.setOutlineThickness( 10 );

        }
        void movement_update([[maybe_unused]] const sf::Vector2f player_pos, [[maybe_unused]] std::vector<projectile>& enemy_projectiles, [[maybe_unused]]sf::FloatRect roomwall, resource_holder& rh) override
        {
            
            std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
            switch (attack_)
            {
                case 2:
                if (std::chrono::duration_cast<std::chrono::seconds>(now - last).count() >= 3)
                {
                    static int attack_projectile_status = 0;
                    //shoot projectiles in a semicircle towards the down part of the map
                    int nr_projectiles = 20;
                    double angle = 0, angleincrement = 3.14159265359 / nr_projectiles;
                    // shoot projectiles in a semicircle towards the down part of the map
                    if (attack_projectile_status == 0)
                    {
                        angle = 0; attack_projectile_status = 1;
                    }
                    else if (std::chrono::duration_cast<std::chrono::seconds>(now - last).count() >= 5)
                    {
                        angle = 0.3;
                        attack_projectile_status = 0;
                        last = now;
                        attack_ = 0;
                    }
                    else break;
                    for (int i = 0; i < nr_projectiles; i++)
                    {
                        std::cout << "!";
                        sf::Vector2f direction = sf::Vector2f(cos(angle), sin(angle));
                        enemy_projectiles.push_back(projectile("boss_projectile", direction, 10.0f, sf::Vector2f(boss_pos.x + 400, boss_pos.y + 900), rh));
                        angle += angleincrement;
                    }
                    
                }
                break;
                case 1:
                    if (std::chrono::duration_cast<std::chrono::seconds>(now - last).count() >= 3)
                    {
                        //summon low-tier enemies (bats, spiders, fatguys)
                        last = now;
                        std::random_device rd;
                        std::mt19937 mt(rd());
                        std::uniform_int_distribution<int>ene(0, 1);
                        auto type_ = ene(mt);
                        switch(type_)
                        {
                            case 0:
                            {
                                //fatguys
                                std::shared_ptr<enemy> temp;
                                temp = std::shared_ptr<enemy>(std::make_shared<fatguy>(enemy_factory::fatguy_(sf::Vector2f(boss_pos.x - 1000, boss_pos.y + 600), rh)));
                                this->buffer_minions.push_back(temp);
                                temp = std::shared_ptr<enemy>(std::make_shared<fatguy>(enemy_factory::fatguy_(sf::Vector2f(boss_pos.x - 800, boss_pos.y + 600), rh)));
                                this->buffer_minions.push_back(temp);
                                temp = std::shared_ptr<enemy>(std::make_shared<fatguy>(enemy_factory::fatguy_(sf::Vector2f(boss_pos.x + 800, boss_pos.y + 600), rh)));
                                this->buffer_minions.push_back(temp);
                                temp = std::shared_ptr<enemy>(std::make_shared<fatguy>(enemy_factory::fatguy_(sf::Vector2f(boss_pos.x + 1000, boss_pos.y + 600), rh)));
                                this->buffer_minions.push_back(temp);
                                break;
                            }
                            case 1: 
                                // template uiesc idk, got no idea
                            {
                                //bats
                                
                                std::shared_ptr<enemy> temp;
                                temp = std::shared_ptr<enemy>(std::make_shared<bat>(enemy_factory::bat_(sf::Vector2f(boss_pos.x - 1000, boss_pos.y + 600), rh)));
                                this->buffer_minions.push_back(temp);
                                temp = std::shared_ptr<enemy>(std::make_shared<bat>(enemy_factory::bat_(sf::Vector2f(boss_pos.x - 1000, boss_pos.y + 800), rh)));
                                this->buffer_minions.push_back(temp);
                                temp = std::shared_ptr<enemy>(std::make_shared<bat>(enemy_factory::bat_(sf::Vector2f(boss_pos.x - 800, boss_pos.y + 600), rh)));
                                this->buffer_minions.push_back(temp);
                                temp = std::shared_ptr<enemy>(std::make_shared<bat>(enemy_factory::bat_(sf::Vector2f(boss_pos.x - 800, boss_pos.y + 800), rh)));
                                this->buffer_minions.push_back(temp);
                                temp = std::shared_ptr<enemy>(std::make_shared<bat>(enemy_factory::bat_(sf::Vector2f(boss_pos.x + 800, boss_pos.y + 600), rh)));
                                this->buffer_minions.push_back(temp);
                                temp = std::shared_ptr<enemy>(std::make_shared<bat>(enemy_factory::bat_(sf::Vector2f(boss_pos.x + 800, boss_pos.y + 800), rh)));
                                this->buffer_minions.push_back(temp);
                                temp = std::shared_ptr<enemy>(std::make_shared<bat>(enemy_factory::bat_(sf::Vector2f(boss_pos.x + 1000, boss_pos.y + 600), rh)));
                                this->buffer_minions.push_back(temp);
                                temp = std::shared_ptr<enemy>(std::make_shared<bat>(enemy_factory::bat_(sf::Vector2f(boss_pos.x + 1000, boss_pos.y + 800), rh)));
                                this->buffer_minions.push_back(temp);
                                break;
                            } // oke, caut si muzica pt el, ca am pt joc, dar pt el n-am gasit inca ceva fitting
                        }
                        std::cout << "Enemies spawned";
                        attack_++;
                    }
                    break;
                case 0:
                    
                    if (std::chrono::duration_cast<std::chrono::seconds>(now - last).count() >= 2)
                    {
                        static int status_attack;
                        if (std::chrono::duration_cast<std::chrono::seconds>(now - last).count() <= 3)
                            text.setString("3");
                        else if (std::chrono::duration_cast<std::chrono::seconds>(now - last).count() <= 4)
                            text.setString("2");
                        else if (std::chrono::duration_cast<std::chrono::seconds>(now - last).count() <= 5)
                            text.setString("1"), status_attack = 1;
                        else 
                        {
                            if (status_attack)
                            {
                                text.setString("0");
                                status_attack = 0;
                            }
                            else
                            {
                                text.setString("-1");
                                attack_++;
                                last = now;
                            }
                        }
                    }

            }
        }
        sf::Text* get_text() 
        {
            return &text;
        }
        
        const std::vector<enemy*>& getBuffer()
        {
            static std::vector<enemy*> tempBuffer;
            tempBuffer.clear();
            while (this->buffer_minions.size() > 0)
            {
                enemy* ptr = this->buffer_minions[0].get();
                tempBuffer.push_back(dynamic_cast<enemy*>(ptr->clone()));
                this->buffer_minions.erase(this->buffer_minions.begin());
            }
            this->buffer_minions.clear();
            return tempBuffer;
        }
        virtual const sf::FloatRect getrect() const override
        {
            return this->sprite_.getRect();
        }
        virtual void render(sf::RenderTarget* target) override
        {
            sprite_.draw(target, boss_pos.x, boss_pos.y);
        }
        entity* clone() const override { return new boss(*this); }
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

