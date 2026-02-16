#pragma once
#include "EntityBase.h"
#include "Enemy.h"
#include <vector>
#include <memory>
class EnemySpawner:public EntityBase
{
public:
    EnemySpawner();

    // Destructor
    ~EnemySpawner() override = default;

    void update(float deltatime) override;
    void render(sf::RenderTarget& target) override;

    // Getters
    std::vector<std::unique_ptr<Enemy>>& get_Enemies();
    int get_Kill_Count();

private:
    // Spawner properties
    float spawn_Timer = 0;
    float spawn_Interval;
    int max_Enemies;
    int current_Enemy_Count;
    int killed_Enemy_Count;

    // Spawn area boundaries
    sf::Vector2f spawn_Area_Min;
    sf::Vector2f spawn_Area_Max;

    float screen_Width;
    float screen_Height;

    // Container for spawned enemies
    std::vector<std::unique_ptr<Enemy>> _Enemies;

    //functions
    void init_Variables();

    void spawn_Enemy();
    void update_Enemy(int i,float deltatime);
    void erase_Enemy(int i);
    sf::Vector2f get_Random_Spawn_Position();
    
};

