#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
class animation
{
    private:
        int current_frame;
        std::vector<sf::Texture> textures;
        std::vector<sf::Sprite> sprites;
        std::chrono::high_resolution_clock::time_point last = std::chrono::high_resolution_clock::now();
    public:
        animation(int nr_of_frames, const std::string &name)
        {
            sf::Texture empty_text;
            sf::Sprite empty_sprite;
            textures.assign(nr_of_frames, empty_text);
            sprites.assign(nr_of_frames, empty_sprite);
            current_frame = 0;
            std::string base_path = "assets/" + name + "/" + name, formed_path;
            for (int i = 1 ; i<= nr_of_frames ; i++)
            {
                formed_path = base_path + std::to_string(i) + ".png";
                if (!textures[i-1].loadFromFile(formed_path)) {
                    throw FileError("loading texture of class animation failed");
                }
                sprites[i-1].setTexture(textures[i - 1]);
                sprites[i - 1].setOrigin(0, 0);
            }
        }
        void draw(sf::RenderTarget * target, float x, float y)
        {
            auto now = std::chrono::high_resolution_clock::now();
            if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last).count() > 100)
            {
                current_frame++;
                if (current_frame == (int) sprites.size())
                {
                    current_frame = 0;
                }
                last = now;
            }
            sprites[current_frame].setPosition(x, y);
            target->draw(sprites[current_frame]);
        }
        sf::FloatRect getRect() const
        {
            return sprites[current_frame].getGlobalBounds();
        }
};
