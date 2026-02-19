#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include "EntityBase.h"
#include "EventSystem.h"

enum class TileType { Tile_0, Tile_1 };

struct Tile
{
    TileType        type = TileType::Tile_0;
    sf::Texture texture;
    sf::Sprite      sprite;
    Tile(sf::Texture tex, sf::Vector2f position)
        : type(TileType::Tile_0), texture(std::move(tex)), sprite(this->texture)
    {
        sprite.setTexture(this->texture, true);
        sprite.setOrigin({ static_cast<float>(this->texture.getSize().x / 2), static_cast<float>(this->texture.getSize().y / 2) });
        sprite.setPosition(position);
    }
};
class TileMap: public EntityBase,public Observer, public Subject
{
public:
    TileMap();
    void update(float deltatime) override;
    void render(sf::RenderTarget& target) override;
    
    void on_Event(const Event& event) override;

    void set_MiniGame_Active(bool);

private:
    int tile_Size = 16;
    int _Columns = 672 / tile_Size; 
    int _Rows = 392 / tile_Size; 
    int total_Tiles = _Columns * _Rows; 

    int tileSet_0_Columns = 2;
    int tileSet_0_Rows = 4;
    int total_Tiles_0 = tileSet_0_Columns * tileSet_0_Rows;

    int tileSet_1_Columns = 2;
    int tileSet_1_Rows = 4;
    int total_Tiles_1 = tileSet_1_Columns * tileSet_1_Rows;

    std::vector<sf::Texture>  tile_0_Texture;
    std::vector<sf::Texture>  tile_1_Texture;

    std::vector<Tile>   _Tiles;

    float terraform_Timer = 0.f;
    float terraform_Time = 10.f;
    float min_Terraform_Time = 1.f;
    float kill_Reduction = 0.1f;
    float terraforming_Factor =1;

    int tile_1_Count = 0;
    bool  is_Won = false;
    bool is_MiniGame_Active = false;

    //ramdomiser
    std::mt19937 rng
    { std::random_device{}()};

    //events 
    terraforming_Percentage_Event terraforming_Percentage;

    void load_Textures();
    void init_Tiles();
    sf::Texture random_Tile_Generator(int tileset);
    void change_Random_Tiles(int count);
    void trigger_Terraform(int terraforming_Factor);
    void on_Kill();
};

