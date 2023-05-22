#include "game_construction.hpp"
class room
{
private:
    sf::Sprite door_sprite_north, door_sprite_south, door_sprite_east, door_sprite_west;
    sf::Sprite sprite;
    sf::Vector2u size;
    sf::FloatRect rectangle;
    float pixel_error_x = 0.f, pixel_error_y = 0.f;
    std::vector<std::unique_ptr<misc>>objects;
    std::vector<std::unique_ptr<enemy>>enemies;
    std::vector<std::unique_ptr<item>>items;

    bool door_directions[4] = { false,false,false,false };

    sf::Texture background;
    bool level_cleared = false, is_dropped_heart = false;
    sf::Texture heart;
    sf::Sprite dropped_heart;
public:
    room() = default;
    room(const std::string& background_location, const std::string& directions, const float pixel_error_x_, const float pixel_error_y_)
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
    inline sf::FloatRect get_heart() const
    {
        if (is_dropped_heart) return dropped_heart.getGlobalBounds();
        return sf::FloatRect(0.0f, 0.0f, 0.0f, 0.0f);
    }
    void setheart(bool value_);
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
    void get_into_room(const resource_holder& rh);
    void display_background(sf::RenderTarget* target);
    void create_enemy(sf::FloatRect redzone, const resource_holder& rh);
    //get enemies
    const std::vector<enemy*>& getEnemies();
    ///copy constructor
    room(const room& other) :
        door_sprite_north(other.door_sprite_north),
        door_sprite_south(other.door_sprite_south),
        door_sprite_east(other.door_sprite_east),
        door_sprite_west(other.door_sprite_west),
        sprite(other.sprite), size(other.size),
        rectangle(other.rectangle),
        pixel_error_x(other.pixel_error_x),
        pixel_error_y(other.pixel_error_y),
        door_directions{ other.door_directions[0],other.door_directions[1],other.door_directions[2],other.door_directions[3] },
        background(other.background),
        level_cleared(other.level_cleared),
        is_dropped_heart(other.is_dropped_heart),
        heart(other.heart),
        dropped_heart(other.dropped_heart)
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
        this->is_dropped_heart = room_to_copy.is_dropped_heart;
        this->heart = room_to_copy.heart;
        this->dropped_heart = room_to_copy.dropped_heart;
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
