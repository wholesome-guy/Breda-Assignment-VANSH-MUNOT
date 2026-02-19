#pragma once
#include "EntityBase.h"
#include "Enemy.h"
#include <vector>
#include <memory>
#include <random>
#include "EventSystem.h"
class Shape;
class GameEngine;
class Player;
class EnemySpawner:public EntityBase,public Subject,public Observer
{
public:
    EnemySpawner();

    // Destructor
    ~EnemySpawner() override = default;

    void update(float deltatime) override;
    void render(sf::RenderTarget& target) override;
    void on_Event(const Event& event) override;

    // Getters
    std::vector<std::unique_ptr<Enemy>>& get_Enemies();

private:
    // Spawner properties
    float spawn_Timer = 0;
    float spawn_Interval;
    int max_Enemies;
    int current_Enemy_Count;
    int kill_Enemy_Count;

    float damage_Multiplier=1;
    float health_Multiplier=1;

    bool is_MiniGame_Active = false;
    // Spawn area boundaries
    sf::Vector2f spawn_Area_Min;
    sf::Vector2f spawn_Area_Max;

    float screen_Width;
    float screen_Height;

    //randomiser
    std::random_device seed;
    std::mt19937 random_Generator;

    // Container for spawned enemies
    std::vector<std::unique_ptr<Enemy>> _Enemies;
    std::vector<std::unique_ptr<Shape>> _Squares;

    //events
    kill_Count_Event kill_count;
    player_Health_Change player_Health_Event;
    kill_reduction_Terraforming_Time_Event reduce_Terraforming_Event;

    GameEngine* _GameEngine;
    //functions
    void init_Variables();

    void spawn_Enemy();
    void update_Enemy(int i, float deltatime);
    void erase_Enemy(int i);
    sf::Vector2f get_Random_Spawn_Position();
    void spawn_Square(int i);
    void erase_Square(int i);
    
};

