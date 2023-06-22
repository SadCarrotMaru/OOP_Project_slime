#include "game_construction.hpp"
class GUI
{
	protected:
        //sf::RectangleShape hp_bar;
        sf::Texture hp_full_heart, hp_half_heart, hp_zero_heart;
        sf::Texture shield;
        sf::Texture sword;
        sf::Texture magic_ability;
        std::vector<sf::Sprite>hearts;
        sf::Sprite shield_;
        sf::Sprite sword_;
        sf::Sprite magic_ability_;
        const int hp_hearts = 10, heart_value = 10;
        public:
        GUI()
        {
            if (!hp_full_heart.loadFromFile("assets/full_hp.png"))
            {
                throw FileError("loading hp_full_heart failed");
            }
            if (!hp_half_heart.loadFromFile("assets/half_hp.png"))
            {
                throw FileError("loading hp_half_heart failed");
            }
            if (!hp_zero_heart.loadFromFile("assets/zero_hp.png"))
            {
                throw FileError("loading hp_zero_heart failed");
            }
            if (!shield.loadFromFile("assets/defend.png"))
            {
                throw FileError("loading ability logo failed");
            }
            if (!sword.loadFromFile("assets/sword_ability.png"))
            {
                throw FileError("loading ability logo failed");
            }
            if (!magic_ability.loadFromFile("assets/magic_power.png"))
            {
                throw FileError("loading ability logo failed");
            }
            shield_.setTexture(shield);
            shield_.setPosition(850,850);
            sword_.setTexture(sword);
            sword_.setPosition(800, 850);
            magic_ability_.setTexture(magic_ability);
            magic_ability_.setPosition(750, 850);
            for (int i = 0; i < hp_hearts; i++)
            {
                sf::Sprite temp;
                temp.setTexture(hp_full_heart);
                temp.setOrigin(0,0);
                temp.setPosition(i * 34 , 0);
                hearts.push_back(temp);
            }
            /*
            hp_bar.setSize(sf::Vector2f(100, 50));
            hp_bar.setOrigin(0, 0);
            hp_bar.setOutlineColor(sf::Color::Black);
            hp_bar.setOutlineThickness(3);
            hp_bar.setFillColor(sf::Color::Red);
            hp_bar.setPosition(0, 0);
            */
        }
        void update_GUI(int current_hp, sf::RenderTarget * target, double time, double time2, int charges_)
        {
            for (int i = 0; i < hp_hearts; i++)
            {
                if (current_hp >= heart_value)
                {
                    hearts[i].setTexture(hp_full_heart);
                    current_hp -= heart_value;
                }
                else if (current_hp == heart_value / 2)
                {
                    hearts[i].setTexture(hp_half_heart);
                    current_hp -= heart_value / 2;
                }
                else
                {
                    hearts[i].setTexture(hp_zero_heart);
                }
                target->draw(hearts[i]);
            }
            double hue = 255 / 5 * time;
            if (hue > 255) hue = 255;
            shield_.setColor(sf::Color(255, 255, 255, hue));
            
            hue = 255 / 2 * time2;
            if (hue > 255) hue = 255;
            sword_.setColor(sf::Color(255, 255, 255, hue));
            
            if (charges_ > 0) target->draw(magic_ability_);
            target->draw(shield_);
            target->draw(sword_);

            //hp_bar.setSize(sf::Vector2f(current_hp, 50));
            //target->draw(hp_bar);
        }
};