#include "game_construction.hpp"
class GUI
{
	protected:
        //sf::RectangleShape hp_bar;
        sf::Texture hp_full_heart, hp_half_heart, hp_zero_heart;
        std::vector<sf::Sprite>hearts;
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
            for (int i = 0; i < 10; i++)
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
        void update_GUI(int current_hp, sf::RenderTarget * target)
        {
            for (int i = 0; i < 10; i++)
            {
                if (current_hp >= 10)
                {
                    hearts[i].setTexture(hp_full_heart);
                    current_hp -= 10;
                }
                else if (current_hp == 5)
                {
                    hearts[i].setTexture(hp_half_heart);
                    current_hp -= 5;
                }
                else
                {
                    hearts[i].setTexture(hp_zero_heart);
                }
                target->draw(hearts[i]);
            }
            //hp_bar.setSize(sf::Vector2f(current_hp, 50));
            //target->draw(hp_bar);
        }
};