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

private:
    // Spawner properties
    float spawn_Timer = 0;
    float spawn_Interval;
    int max_Enemies;
    int current_Enemy_Count;

    // Spawn area boundaries
    sf::Vector2f spawn_Area_Min;
    sf::Vector2f spawn_Area_Max;

    float screen_Width;
    float screen_Height;
    float spawn_Margin;
    // Container for spawned enemies
    std::vector<std::unique_ptr<Enemy>> _Enemies;

    void init_Variables();

    void spawn_Enemy();
    sf::Vector2f get_Random_Spawn_Position();
    //void cleanup_Dead_Enemies();
};

