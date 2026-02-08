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
    float spawn_Timer_m = 0;
    float spawn_Interval_m;
    int max_Enemies_m;
    int current_Enemy_Count_m;

    // Spawn area boundaries
    sf::Vector2f spawn_Area_Min_m;
    sf::Vector2f spawn_Area_Max_m;

    // Container for spawned enemies
    std::vector<std::unique_ptr<Enemy>> _Enemies_m;

    void init_Variables();

    void spawn_Enemy();
    sf::Vector2f get_Random_Spawn_Position();
    //void cleanup_Dead_Enemies();
};

