#include "game.h"

Game::Game()
{
	this->possible_rooms.clear();
    player.setPosition(400,470);
	///init window
    this->videoMode = sf::VideoMode(900, 900);
    this->window = new sf::RenderWindow(this->videoMode, "Unknown Project", sf::Style::Close | sf::Style::Titlebar);
    this->window->setFramerateLimit(60);
    ///final init
    this->endGame = false;
}

Game::~Game()
{
	delete this->window;
}

bool Game::running() const
{
	return this->window->isOpen();
}
void Game::pollEvents()
{
	while (this->window->pollEvent(this->sfmlEvent))
	{
		switch (this->sfmlEvent.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			//this->player.update(this->window);
			if (this->sfmlEvent.key.code == sf::Keyboard::X) std::cout << this->player;
			break;
        case::sf::Event::MouseButtonPressed:
            if (this->sfmlEvent.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
                auto mousePosF = (*window).mapPixelToCoords(mousePos);
               // sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
                sf::Vector2f playerPos = this->player.getModelCoord();
                sf::Vector2f direction = mousePosF - playerPos;
                auto length = float(sqrt((direction.x * direction.x) + (direction.y * direction.y)));
                direction.x /= length;
                direction.y /= length;
                projectiles.push_back(projectile("simple_projectile", direction, 12.0f, mousePosF, playerPos, rh));
            }
            break;
		default:
			break;
		}
	}
}

void Game::updatePlayer()
{
	int temp = this->player.update(this->current_room.getRectangle(), this->window, &(this->current_room));
    std::cout << temp;
	if (this->player.getHp() <= 0)
		this->endGame = true;
}

void Game::setView()
{
	sf::Vector2f center_map;
	center_map.x = (this->current_room.getBackgroundRectangle().left + this->current_room.getBackgroundRectangle().width) / 2;
	center_map.y = (this->current_room.getBackgroundRectangle().top + this->current_room.getBackgroundRectangle().height) / 2;
	
    if (this->current_room.getBackgroundRectangle().width > 1620)
    {
        this->view.setCenter(this->player.getModelCoord());
        this->window->setView(this->view);
    }
    else
    {
        this->view.setCenter(center_map);
        this->view.zoom(1.6f);
        this->window->setView(this->view);
        this->view.zoom((float)(1 / 1.6f));
    }
 }

void Game::update()
{
    this->pollEvents();
    if (!this->endGame)
    {
        this->updatePlayer();
    }
    if(this->endGame)
    {
        this->window->close();
    }
}
void Game::render()
{
	this->window->clear();
	this->updatePlayer();
	this->setView();
	this->current_room.display_background(this->window);
    if (!this->projectiles.empty())
    {
        for (int i = 0; i < this->projectiles.size(); i++)
        {
            auto projectile_ = &(this->projectiles[i]);
            projectile_->update();
            if (projectile_->check(current_room.getRectangle()))
            {
                this->projectiles.erase(this->projectiles.begin()+i);
                continue;
            }
            projectile_->render(this->window);
        }
    }
	this->player.render(this->window);
	this->window->display();
}


//void Game::add_projectile(const projectile& projectile_) {
//    this->projectiles.push_back(projectile_);
//}

void Game::fill_dungeon(int x, int y)
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(0, 1);
    if (!(x == 3 && y == 3))
    {
        auto for_room = dist(mt);
        this->roomlayout[x][y] = this->possible_rooms[for_room];
    }
    int dx[] = { -1,1,0,0 };
    int dy[] = { 0,0,1,-1 };
    int cnt = 0;
    for (int i = 0; i < 4; i++)
    {
        auto val = dist(mt); // Integer
        std::cout << val << ' ';
        if (val == 1)
        {
            cnt++;
            if (!(x == 0 || x == 6 || y == 0 || y == 6))
            {
                this->roomlayout[x][y].set_door(i);
                int pair;
                if (i == 0) pair = 1;
                else if (i == 1) pair = 0;
                else if (i == 2) pair = 3;
                else pair = 2;
                if (this->roomlayout[x + dx[i]][y + dy[i]].getdoor(pair) == false)
                {
                    this->roomlayout[x + dx[i]][y + dy[i]].set_door(pair);
                    fill_dungeon(x + dx[i], y + dy[i]);
                }
            }
        }
    }
    if (x == 3 && y == 3 && cnt == 0)
    {
        int i = 0;
        this->roomlayout[x][y].set_door(i);
        int pair = 1;
        if (this->roomlayout[x + dx[i]][y + dy[i]].getdoor(pair) == false)
        {
            this->roomlayout[x + dx[i]][y + dy[i]].set_door(pair);
            fill_dungeon(x + dx[i], y + dy[i]);
        }
    }
}


void Game::generate_dungeon()
{
    this->roomlayout[3][3]=this->possible_rooms[0];
    this->fill_dungeon(3, 3);
    this->current_room = this->roomlayout[3][3];
    this->current_room.get_into_room();
}

void Game::create_rooms()
{
	room room1("assets/background1.png", "0000", 180, 240);
	this->possible_rooms.push_back(room1);
	room room2("assets/background2.png", "0000", 180, 240);
	this->possible_rooms.push_back(room2);
    this->generate_dungeon();
}






Game::Game (const Game& other)
{
    this->videoMode = other.videoMode;
    this->view = other.view;
    this->endGame = other.endGame;
	this->window = new sf::RenderWindow;
    this->window = other.window;
    this->possible_rooms = other.possible_rooms;
    this->current_room = other.current_room;
    this->sfmlEvent = other.sfmlEvent;
    this->player = other.player;
    this->font = other.font;
    this->guiText = other.guiText;
    this->endGameText = other.endGameText;
}
Game& Game::operator=(const Game& other)
{
    if (this != &other)
    {
        this->videoMode = other.videoMode;
        this->view = other.view;
        this->endGame = other.endGame;
		this->window = new sf::RenderWindow;
        this->window = other.window;
        this->possible_rooms = other.possible_rooms;
        this->current_room = other.current_room;
        this->sfmlEvent = other.sfmlEvent;
        this->player = other.player;
        this->font = other.font;
        this->guiText = other.guiText;
        this->endGameText = other.endGameText;
    }
    return *this;
}

// << operator
std::ostream& operator<<(std::ostream& out, const Game& game)
{
    out << "current_room: \n";
    out << game.current_room;
    out << "player: \n";
    return out;
}