#ifndef GAME_INITIALISATION
#define GAME_INITIALISATION

#ifndef LIBRARIES_INCLUDED
#define LIBRARIES_INCLUDED
#include<iostream>
#include<vector>
#include<string>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
using namespace std;
#endif
class effect
{
    protected:
        string effect_name;
        vector<pair<string,string>>modifiers;   
};
class projectile
{
    protected:
        vector<pair<int,int>>coordinates_affected;
        string type_of_projectile;
        string direction;
        effect effect_on_projectile;
};
class ability
{
    protected:
        string ability_name;
        projectile area_affected_by_ability;
};
class weapon
{
    protected:
        string weapon_name;
        ability weapon_ability;
        projectile area_affected_by_weapon;
};
class item
{
    protected:
        string item_name;
        string class_affected;
        vector<effect>effects;
};
class entity
{
    protected: 
        int HP, MAXHP;
        weapon weapon_held;
        effect effect_applied_on_entity;
        float x,y;
    public:
        entity()
        {
            x = y = 0.0;
            MAXHP = 100;
            HP = MAXHP;
        }
};
class enemy : entity
{
    private:
        bool phase2 = false;
        string enemy_name;
};
class misc
{
    protected:
        float x, y;
        sf::Sprite sprite_misc;
};
class room
{
    private:
        sf::Sprite sprite;
        sf::Vector2u size;

        vector<misc>objects;
        vector<enemy>enemies;
        vector<item>items;
        bool door_directions[5] ;
        bool final_boss_level;
        sf::Texture background;
    public:
        room()
        {
            final_boss_level = false;
        }
        room(string background_location, string directions)
        {
            if (directions[0] == '1') door_directions[0] = true;
            else door_directions[0] = false;
            if (directions[1] == '1') door_directions[1] = true;
            else door_directions[1] = false;
            if (directions[2] == '1') door_directions[2] = true;
            else door_directions[2] = false;
            if (directions[3] == '1') door_directions[3] = true;
            else door_directions[3] = false;
            if (!background.loadFromFile(background_location))
            {
                cout << "loading failed";
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
        sf::FloatRect getRectangle()
        {
            return sprite.getLocalBounds();
        }
        void get_into_room()
        {
            size = background.getSize();
            sprite.setTexture(background);
            sprite.setOrigin(0.f,0.f);
        }
        void display_background(sf::RenderTarget* target)
        {
            target->draw(sprite);
        }
        void add_enemy(enemy enemy_to_add) { enemies.push_back(enemy_to_add); }
        void add_object(misc object) { objects.push_back(object); }
        void add_item(item item_to_add) { items.push_back(item_to_add); }
};

#endif