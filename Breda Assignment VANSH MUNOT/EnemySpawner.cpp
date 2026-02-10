#include "EnemySpawner.h"
#include <random>

EnemySpawner::EnemySpawner()
{
	init_Variables();
}
void EnemySpawner::init_Variables()
{
	spawn_Interval = 2.f;
	max_Enemies = 10;
	spawn_Timer = 0;
	spawn_Area_Min = { -640,-360 };
	spawn_Area_Max = { 1280,720 };
}
void EnemySpawner::update(float deltatime)
{
	spawn_Timer += deltatime;

	if (spawn_Timer >= spawn_Interval && current_Enemy_Count < max_Enemies)
	{
		spawn_Enemy();
		spawn_Timer = 0.0f; // Reset timer
	}

	// Update all enemies
	for (auto& e : _Enemies)
	{
		e->update(deltatime);
	}
}
void EnemySpawner::render(sf::RenderTarget& target)
{
	for (auto& e : _Enemies)
	{
		e->render(target);
	}
}
void EnemySpawner::spawn_Enemy()
{
	auto new_Enemy = std::make_unique<Enemy>();

	new_Enemy->set_Position(get_Random_Spawn_Position());

	_Enemies.push_back(std::move(new_Enemy));

	current_Enemy_Count++;

}
sf::Vector2f EnemySpawner::get_Random_Spawn_Position()
{
	//get seed
	std::random_device rd;
	//intialise  m twister
	std::mt19937 gen(rd());

	//uniform real distribution= random float values betweentwo values
	std::uniform_real_distribution<float> distance_X(spawn_Area_Min.x, spawn_Area_Max.x);
	std::uniform_real_distribution<float> distance_Y(spawn_Area_Min.y, spawn_Area_Max.y);

	return { distance_X(gen),distance_Y(gen) };

}
std::vector<std::unique_ptr<Enemy>>& EnemySpawner::get_Enemies()
{
return _Enemies;
}
