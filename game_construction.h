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
};
class enemy : entity
{
    protected:
        bool phase2 = false;
        string enemy_name;
};
#endif