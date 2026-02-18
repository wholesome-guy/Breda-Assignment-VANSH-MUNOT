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
    sf::Sprite      sprite;
    Tile(sf::Texture& texture, sf::Vector2f position)
        : type(TileType::Tile_0), sprite(texture)
    {
        sprite.setTexture(texture,true);
        sprite.setOrigin({ static_cast<float>(texture.getSize().x / 2),static_cast<float>(texture.getSize().y / 2) });
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

private:
    int tile_Size = 16;
    int _Columns = 672 / tile_Size; // 40
    int _Rows = 392 / tile_Size; // 22
    int total_Tiles = _Columns * _Rows;     // 880

     sf::Texture  tile_0_Texture;
     sf::Texture  tile_1_Texture;

    std::vector<Tile>   _Tiles;

    float terraform_Timer = 0.f;
    float terraform_Time = 10.f;
    float min_Terraform_Time = 1.f;
    float kill_Reduction = 0.1f;
    float terraforming_Factor =1;

    int tile_1_Count = 0;
    bool  is_Won = false;

    std::mt19937 rng
    { std::random_device{}()};
    std::uniform_int_distribution<int>  random_Tile{ 0, total_Tiles - 1 };

    //events 
    terraforming_Percentage_Event terraforming_Percentage;

    void load_Textures();
    void init_Tiles();
    void change_Random_Tiles(int count);
    void trigger_Terraform(int terraforming_Factor);
    void on_Kill();
    bool get_Is_Won();
};

