#include "game.h"
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
            if (this->sfmlEvent.key.code == sf::Keyboard::M)
            {
                this->display_map();
            }
            if (this->sfmlEvent.key.code == sf::Keyboard::T)
            {
                if(this->charges_ability>0)
                {
                    this->ability_.setScale(1, 1);
                    this->ability_.setOrigin(this->ability_.getLocalBounds().width / 2, this->ability_.getLocalBounds().height / 2);
                    this->ability_.setPosition(this->player.getModelCoord());
                    this->ability_launched = true;
                    this->charges_ability--;
                }
            }
            if (this->sfmlEvent.key.code == sf::Keyboard::Q)
            {
                std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
                if (std::chrono::duration_cast<std::chrono::seconds>(now - last2).count() >= 2)
                {
                    last2 = now;
                    sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
                    auto mousePosF = (*window).mapPixelToCoords(mousePos);
                    sf::Vector2f playerPos = this->player.getModelCoord();
                    sf::Vector2f direction = mousePosF - playerPos;
                    auto length = float(sqrt((direction.x * direction.x) + (direction.y * direction.y)));
                    direction.x /= length;
                    direction.y /= length;
                    float rotation = std::atan2(direction.y, direction.x) * 180 / std::acos(-1.0);
                    ally_projectiles.push_back(projectile("ability_sword", direction, 25.0f, playerPos, rh, rotation));
                }
            }
            if (this->sfmlEvent.key.code == sf::Keyboard::X) std::cout << this->player;
            if (this->sfmlEvent.key.code == sf::Keyboard::Z) std::cout << this->dungeons_left;
            if (this->sfmlEvent.key.code == sf::Keyboard::G) this->dungeons_left = 1;
            if (this->sfmlEvent.key.code == sf::Keyboard::F)
            {
                std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
                if (std::chrono::duration_cast<std::chrono::seconds>(now - last).count() >= 5)
                {
                    last = now;
                    this->player.updateTime();
                }
            }
            break;
        case::sf::Event::MouseButtonPressed:
            if (this->sfmlEvent.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
                auto mousePosF = (*window).mapPixelToCoords(mousePos);
                sf::Vector2f playerPos = this->player.getModelCoord();
                sf::Vector2f direction = mousePosF - playerPos;
                auto length = float(sqrt((direction.x * direction.x) + (direction.y * direction.y)));
                direction.x /= length;
                direction.y /= length;
                ally_projectiles.push_back(projectile("allied", direction, 15.0f, playerPos, rh));
            }
            break;
        default:
            break;
        }
    }
}

void Game::display_map()
{
    std::vector<std::vector<sf::RectangleShape>>map__;
    map__.clear();
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(50, 50));
    rectangle.setOutlineColor(sf::Color::Black);
    rectangle.setOutlineThickness(5);
    rectangle.setPosition(300, 250);
    rectangle.setFillColor(sf::Color::Black);
    this->window->setView(this->window->getDefaultView());
    for (int i = 1; i <= 6; i++)
    {
        std::vector<sf::RectangleShape>temp;
        map__.push_back(temp);
        for (int j = 1; j <= 6; j++)
        {
            if (this->visited[i][j] == true)
            {
                if (this->roomlayout[i][j].get_level_clear() == true) rectangle.setFillColor(sf::Color::Blue);
                    else rectangle.setFillColor(sf::Color::Red);
            }
            else rectangle.setFillColor(sf::Color::Black);
            if (i == 3 && j == 3) rectangle.setFillColor(sf::Color::Magenta);
            if (this->xr == i && this->yr == j) rectangle.setFillColor(sf::Color::Green);
            rectangle.setPosition(300 + (j - 1) * 50, 250 + (i - 1) * 50);
            map__[map__.size() - 1].push_back(rectangle);
            this->window->draw(map__[i - 1][j - 1]);
        }
    }
    this->window->display();
    int ok = 0;
    while (1)
    {
        while (this->window->pollEvent(this->sfmlEvent))
        {
            if (this->sfmlEvent.type == sf::Event::KeyPressed && this->sfmlEvent.key.code == sf::Keyboard::M) { ok = 1; break; }
        }
        if (ok) break;
    }
}
void Game::updatePlayer()
{
    if (this->dungeons_left <= this->init_dungeons / 2 && this->dungeons_left > 1 && small_help == 0)
        this->player.updateModel_progress(1), this->charges_ability = 2, small_help = 1;
    else if (this->dungeons_left <= 1 && small_help == 1)
        this->player.updateModel_progress(2), this->charges_ability = 2, small_help = 2;

    if (collision::collisionsprites(this->player.getrect(), this->current_room.get_heart()))
    {
        this->player.getdamage(-10);
        this->current_room.setheart(false);
    }
    int temp = this->player.update(this->current_room.getRectangle(), &(this->current_room));
    if (this->current_room.get_level_clear() == true)
    {
        this->roomlayout[xr][yr].set_level_clear(true);
        switch (temp)
        {
        case 0: {this->xr--; break; }
        case 1: {this->xr++; break; }
        case 2: {this->yr++; break; }
        case 3: {this->yr--; break; }
        }
        if (temp != 4)
        {
            //std::cout << xr << ' ' << yr << ' ';
            this->current_room = this->roomlayout[this->xr][this->yr];
            this->current_room.get_into_room(rh);
            if (this->current_room.get_level_clear() == false)
            {
                this->current_room.create_enemy(this->player.getrect(), rh);
                std::vector<enemy*> tempenemi;
                tempenemi.clear();
                tempenemi = this->current_room.getEnemies();
                for (auto i : tempenemi)
                    this->entities.push_back(i);
            }
            float xx, yy;
            if (temp == 0) xx = this->current_room.get_south().left, yy = this->current_room.get_south().top - 150;
            else if (temp == 1) xx = this->current_room.get_north().left, yy = this->current_room.get_north().top + this->current_room.get_north().height + 150;
            else if (temp == 2) xx = this->current_room.get_west().left + this->current_room.get_west().width + 150, yy = this->current_room.get_west().top;
            else xx = this->current_room.get_east().left - 150, yy = this->current_room.get_east().top;
            this->ally_projectiles.clear();
            this->enemy_projectiles.clear();
            this->player.setPosition(xx, yy);
            using namespace std::chrono;
            using namespace std::this_thread;
            sleep_for(nanoseconds(10));
        }
    }
    if(this->xr==3 && this->yr==3 && this->dungeons_left==1 && collision::collisionsprites(this->player.getrect(), this->trap_door_.getGlobalBounds()))
    {
        this->xr = 0, this->yr = 0;
        std::cout << "Maybe here?";
        this->current_room = this->possible_rooms[2];
        std::cout << " Huh?";
        this->current_room.get_into_room(rh);
       // std::cout << "It reached here";
        this->ally_projectiles.clear();
        this->enemy_projectiles.clear();
        this->player.setPosition(3000, 2000);
        if (!music.openFromFile("assets/boss.mp3"))
        {
            throw FileError("music loading error");
        }
        music.setLoop(true);
        music.setVolume(50);
        music.play();
        sf::Vector2f position_boss(2000.0f, 430.f);
        boss_ = new boss(position_boss);
        this->entities.push_back(dynamic_cast<enemy*> (boss_));
        // put music
        this->boss_hp.setPosition(2100.0f,1230.0f);
        this->boss_hp.setOrigin(0, 0);
        this->boss_hp.setFillColor(sf::Color::Red);
        this->boss_hp.setSize(sf::Vector2f(700,50));
        using namespace std::chrono;
        using namespace std::this_thread;
        sleep_for(nanoseconds(10));
    }
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
        if (!(this->xr == 0 && this->yr == 0))
        {
            this->view.setCenter(this->player.getModelCoord());
            this->view.zoom(1.3f);
            this->window->setView(this->view);
            this->view.zoom((float)(1 / 1.3f));
        }
        else
        {
            this->view.setCenter(this->player.getModelCoord());
            this->view.zoom(3.0f);
            this->window->setView(this->view);
            this->view.zoom((float)(1/3.0f));
        }
    }
    else
    {
        this->view.setCenter(center_map);
        this->view.zoom(1.6f);
        this->window->setView(this->view);
        this->view.zoom((float)(1 / 1.6f));
    }
}
void Game::updateProjectiles()
{
    if (this->ability_launched == true)
    {
        this->ability_.scale(1.05f, 1.05f);
        for (int z = 0; z < (int)this->entities.size(); z++)
        {
            auto ptr = this->entities[z];
            if (ptr == dynamic_cast<enemy*>(ptr))
            {
                sf::FloatRect drept = ptr->getrect();
                sf::Vector2f pos_inamic(drept.left + drept.width / 2, drept.top + drept.height / 2);
                double dist_de_centru = std::sqrt((this->ability_.getPosition().x - pos_inamic.x) * (this->ability_.getPosition().x - pos_inamic.x) + (this->ability_.getPosition().y - pos_inamic.y) * (this->ability_.getPosition().y - pos_inamic.y));
                if (dist_de_centru <= this->ability_.getGlobalBounds().width / 2 && dist_de_centru > this->ability_.getGlobalBounds().width / 2 - (15 * this->ability_.getGlobalBounds().width / 100)   )
                {
                    ptr->getdamage(100);
                    if (ptr->getHP() <= 0)
                    {
                        this->entities.erase(this->entities.begin() + z);
                        if (this->entities.size() == 1)
                        {
                            this->roomlayout[this->xr][this->yr].setheart(true);
                            this->current_room.setheart(true);
                            this->dungeons_left--;
                        }
                    }
                }
            }
        }
        if (this->ability_.getGlobalBounds().width > 10000) ability_launched = false;
    }
    if (!this->ally_projectiles.empty())
    {
        for (int i = 0; i < (int)this->ally_projectiles.size(); i++)
        {
            auto projectile_ = &(this->ally_projectiles[i]);
            projectile_->update();
            if (projectile_->check(current_room.getRectangle()))
            {
                this->ally_projectiles.erase(this->ally_projectiles.begin() + i);
                continue;
            }
        }
    }
    if (!this->enemy_projectiles.empty())
    {
        for (int i = 0; i < (int)this->enemy_projectiles.size(); i++)
        {
            auto projectile_ = &(this->enemy_projectiles[i]);
            projectile_->update();
            if (projectile_->check(current_room.getRectangle()))
            {
                this->enemy_projectiles.erase(this->enemy_projectiles.begin() + i);
                continue;
            }
        }
    }
}
void Game::handle_enemy()
{
    if (entities.size() == 1)
    {
        this->current_room.set_level_clear(true);

    }
    for (int i = 1; i < (int)this->entities.size(); i++)
    {
        if (enemy* en = dynamic_cast<enemy*>(this->entities[i]); en != nullptr)
            en->movement_update(this->player.getModelCoord(), this->enemy_projectiles, this->current_room.getRectangle(), this->rh);
    }
}
void Game::render_enemy()
{
    for (int i = 1; i < (int)this->entities.size(); i++)
    {
        if (enemy* en = dynamic_cast<enemy*>(this->entities[i]); en != nullptr)
            en->render(this->window);
    }
}
void Game::checkcolliders()
{
    for (int z = 0; z < (int)this->entities.size(); z++)
    {
        auto ptr = this->entities[z];
        if (ptr == dynamic_cast<enemy*>(ptr))
        {
            if (!this->ally_projectiles.empty())
            {
                for (int i = 0; i < (int)this->ally_projectiles.size(); i++)
                {
                    auto projectile_ = &(this->ally_projectiles[i]);
                    if (collision::collisionsprites(ptr->getrect(), projectile_->get_projectile()))
                    {
                        if(!(projectile_->get_type_of_projectile() == "ability_sword" && this->dungeons_left==1&&z==1)) ptr->getdamage(10);
                        if(projectile_->get_type_of_projectile() != "ability_sword") this->ally_projectiles.erase(this->ally_projectiles.begin() + i);
                        if (ptr->getHP() <= 0)
                        {
                            //delete ptr;
                            this->entities.erase(this->entities.begin() + z);
                            if (this->entities.size() == 1)
                            {
                                this->roomlayout[this->xr][this->yr].setheart(true);
                                this->current_room.setheart(true);
                                this->dungeons_left--;
                            }
                            
                        }
                        break;
                    }
                }
            }
            if (collision::collisionsprites(ptr->getrect(), this->player.getrect()))
            {
                this->player.getdamage(5);
            }
        }
        else
        {
            if (!this->enemy_projectiles.empty())
            {
                for (int i = 0; i < (int)this->enemy_projectiles.size(); i++)
                {
                    auto projectile_ = &(this->enemy_projectiles[i]);
                    if (collision::collisionsprites(ptr->getrect(), projectile_->get_projectile()))
                    {
                        ptr->getdamage(10);
                        this->enemy_projectiles.erase(this->enemy_projectiles.begin() + i);
                        if (ptr->getHP() <= 0)
                        {
                            //delete ptr;
                            this->entities.erase(this->entities.begin() + z);
                            
                            /// end the game
                        }
                        break;
                    }
                }
            }
        }
    }
}
void Game::update()
{
    this->pollEvents();
    if (!this->endGame)
    {
        this->updatePlayer();
        this->setView();
        this->updateProjectiles();
        this->handle_enemy();
        this->checkcolliders();
        if (this->xr == 0 && this->yr == 0)
        {
            this->boss_hp.setSize(sf::Vector2f(700.0f / 3500.0f * this->boss_->getHP(),50));
            if (this->boss_->getHP() == 0)
                this->window->close();
        }
    }    
    if (this->endGame)
    {
        this->window->close();
    }
}
void Game::render()
{
    this->window->clear();
    this->update();
    this->current_room.display_background(this->window);
    if (this->xr==3 && this->yr==3 && this->dungeons_left == 1) this->window->draw(this->trap_door_);
    if (!this->ally_projectiles.empty())
    {
        for (int i = 0; i < (int)this->ally_projectiles.size(); i++)
        {
            auto projectile_ = &(this->ally_projectiles[i]);
            projectile_->render(this->window);
        }
    }
    if (!this->enemy_projectiles.empty())
    {
        for (int i = 0; i < (int)this->enemy_projectiles.size(); i++)
        {
            auto projectile_ = &(this->enemy_projectiles[i]);
            projectile_->render(this->window);
        }
    }
    if (this->ability_launched == true) this->window->draw(this->ability_);
    this->render_enemy();
    this->player.render(this->window);
    if(this->xr == 0 && this->yr ==0) this->window->draw(boss_hp);
    this->window->setView(this->window->getDefaultView());
    if (this->xr == 0 && this->yr == 0)
    {
       // std::cout << this->entities.size();
        std::vector<enemy*> tempenemi;
        tempenemi.clear();
        if (entities.size() > 1)
        {
            auto reply = dynamic_cast<boss*>(this->entities[1])->get_text();
            tempenemi = dynamic_cast<boss*>(this->entities[1])->getBuffer();

            for (auto i : tempenemi)
                this->entities.push_back(i);
            //std::cout << this->entities.size();
            reply->setPosition(430, 50);
            if (reply->getString() != "0" && reply->getString() != "-1")
                this->window->draw(*reply);
            if (reply->getString() == "0")
                this->player.getdamage(15), std::cout << "AAAAA";
        }
    }
    sf::Vector2f help(200, 200);
    std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
    double time_diff = std::chrono::duration_cast<std::chrono::seconds>(now - last).count();
    double time_diff2 = std::chrono::duration_cast<std::chrono::seconds>(now - last2).count();
    this->GUI_.update_GUI(this->player.getHP(), this->window, time_diff, time_diff2, charges_ability);
    this->setView();
    this->window->display();
}


//void Game::add_projectile(const projectile& projectile_) {
//    this->projectiles.push_back(projectile_);
//}

void Game::fill_dungeon(int x, int y)
{
    this->visited[x][y] = true;
    this->dungeons_left++;
    //std::cout << "A intrat in: " << x << ' ' << y << std::endl;
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(0, 1);
    if (!(x == 3 && y == 3))
    {
        auto for_room = dist(mt);
        bool tempdoors[5];
        for (auto i = 0; i < 4; i++)
            tempdoors[i] = this->roomlayout[x][y].getdoor(i);
        this->roomlayout[x][y] = this->possible_rooms[for_room];
        for (auto i = 0; i < 4; i++)
            if (tempdoors[i] == true) this->roomlayout[x][y].set_door(i);
    }
    const int dx[] = { -1,1,0,0 };
    const int dy[] = { 0,0,1,-1 };
    int cnt = 0;
    for (int i = 0; i < 4; i++)
    {
        auto val = dist(mt); // Integer
        if (val == 1)
        {
            cnt++;
            /// case pt cand e 1 si cand e 6
            if (!(x == 1 || x == 6 || y == 1 || y == 6))
            {
                this->roomlayout[x][y].set_door(i);
                int pair;
                if (i == 0) pair = 1;
                else if (i == 1) pair = 0;
                else if (i == 2) pair = 3;
                else pair = 2;
                if (this->visited[x + dx[i]][y + dy[i]] == false)
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
    this->roomlayout[3][3] = this->possible_rooms[0];
    this->roomlayout[3][3].set_level_clear(true);
    this->fill_dungeon(3, 3);
    for (int i = 1; i <= 6; i++)
    {
        for (int j = 1; j <= 6; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                int nx = i, ny = j;
                if (k == 0) nx--;
                else if (k == 1) nx++;
                else if (k == 2) ny++;
                else ny--;
                if (this->roomlayout[i][j].getdoor(k) == false && this->visited[nx][ny] == true) this->roomlayout[i][j].set_door(k);
            }
            std::cout << '\n';
        }
    }
    this->init_dungeons = this->dungeons_left;
    this->current_room = this->roomlayout[3][3];
    this->current_room.get_into_room(rh);
    this->xr = this->yr = 3;
    /*
    for (int i = 1; i <= 6; i++)
    {
        for (int j = 1; j <= 6; j++)
        {
            std::cout << i << ' ';
            std::cout << j << ": ";
            for (int k = 0; k < 4; k++)
            {
                if (k == 0) std::cout << 'N';
                else if (k == 1) std::cout << 'S';
                else if (k == 2) std::cout << 'E';
                else if (k == 3) std::cout << 'W';
                std::cout << this->roomlayout[i][j].getdoor(k) << ' ';
            }
            std::cout << '\n';
        }
    }
    */
}

void Game::create_rooms()
{
    room room1("assets/background1.png", "0000", 180, 240);
    this->possible_rooms.push_back(room1);
    room room2("assets/background2.png", "0000", 180, 240);
    this->possible_rooms.push_back(room2);
    room room3("assets/boss_room.png", "0000", 450, 680); //450,680
    this->possible_rooms.push_back(room3);
    this->generate_dungeon();
    if (this->dungeons_left < 3)
    {
        throw UnluckyRNG("Dungeon size too small. Aborting process");
    }
}



// << operator
std::ostream& operator<<(std::ostream& out, const Game& game)
{
    out << "current_room: \n";
    out << game.current_room;
    out << "player: \n";
    return out;
}