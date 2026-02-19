#include "TileMap.h"
#include <iostream>

TileMap::TileMap()
{
    load_Textures();
    init_Tiles();
}

void TileMap::load_Textures()
{
    tile_0_Texture = sf::Texture(sf::Image("C:/Users/vansh/CPP Games/Breda Assignment/Source/Repository/Breda Assignment VANSH MUNOT/Assets/Player/Tile_0_PNG.png"));
    tile_1_Texture = sf::Texture(sf::Image("C:/Users/vansh/CPP Games/Breda Assignment/Source/Repository/Breda Assignment VANSH MUNOT/Assets/Player/Tile_1_PNG.png"));
}

void TileMap::init_Tiles()
{
    //just like printing a square of character, rows cols
    _Tiles.reserve(total_Tiles);
    for (int row = 0; row < _Rows; row++)
    {
        for (int col = 0; col < _Columns; col++)
        {
            //emplace creats the object inside the vector, while push back needs a pointer to be moved into the vector
            _Tiles.emplace_back(tile_0_Texture,sf::Vector2f{ static_cast<float>(col * tile_Size), static_cast<float>(row * tile_Size) });
        }
    }
}

void TileMap::update(float deltatime)
{
    if (is_Won ||is_MiniGame_Active) 
    { 
        return; 
    }

    terraform_Timer += deltatime;
    if (terraform_Timer >= terraform_Time)
    {
        terraform_Timer = 0.f;
        trigger_Terraform(terraforming_Factor);

    }

}

void TileMap::trigger_Terraform(int terraforming_Factor)
{
    if (is_Won) 
    {
        return;
    } 

    change_Random_Tiles(terraforming_Factor);

    //UI Event for percentage
    terraforming_Percentage.percent = static_cast<int>((static_cast<float>(tile_1_Count) / static_cast<float>(total_Tiles)) * 100.f);
    notify_Observers(terraforming_Percentage);

    if (tile_1_Count >= total_Tiles)
    {
        is_Won = true;

        game_Over event;
        event.state = 1;
        notify_Observers(event);
    }
}

void TileMap::change_Random_Tiles(int count)
{
    //changed to keep track of tiles changed out of count(the parameter)
    //attempts to avoid a infinte loop
    int changed = 0;
    int attempts = 0;

    while (changed < count && attempts < total_Tiles)
    {
        int index = random_Tile(rng);
        //checks if tile is tile 0
        if (_Tiles[index].type == TileType::Tile_0)
        {
            _Tiles[index].type = TileType::Tile_1;
            _Tiles[index].sprite.setTexture(tile_1_Texture,true);
            tile_1_Count++;
            changed++;
        }
        attempts++;
    }
}

void TileMap::on_Kill()
{
    //reduces terraforming when enmy dies
    terraform_Time -= kill_Reduction;

    if (terraform_Time < min_Terraform_Time)
    {
        terraform_Time = min_Terraform_Time;
    }
}

void TileMap::render(sf::RenderTarget& target)
{
    for (auto& tile : _Tiles)
    {
        target.draw(tile.sprite);
    }
}

void TileMap::on_Event(const Event& event)
{
    if (auto* data = dynamic_cast<const player_terraforming_Factor_Event*>(&event))
    {
        terraforming_Factor = data->Terraforming_Factor;
    }
    else if (dynamic_cast<const kill_reduction_Terraforming_Time_Event*>(&event))
    {
        on_Kill();
    }
}

void TileMap::set_MiniGame_Active(bool state)
{
    is_MiniGame_Active = state;
}
