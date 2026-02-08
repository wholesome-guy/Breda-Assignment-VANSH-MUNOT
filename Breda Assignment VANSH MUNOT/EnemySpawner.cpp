#include "EnemySpawner.h"
#include <random>

EnemySpawner::EnemySpawner()
{
	init_Variables();
}
void EnemySpawner::init_Variables()
{
	spawn_Interval_m = 2.f;
	max_Enemies_m = 10;
	spawn_Timer_m = 0;
	spawn_Area_Min_m = { 0,0 };
	spawn_Area_Max_m = { 960,540 };
}
void EnemySpawner::update(float deltatime)
{
	spawn_Timer_m += deltatime;

	if (spawn_Timer_m >= spawn_Interval_m && current_Enemy_Count_m < max_Enemies_m)
	{
		spawn_Enemy();
		spawn_Timer_m = 0.0f; // Reset timer
	}

	// Update all enemies
	for (auto& e : _Enemies_m)
	{
		e->update(deltatime);
	}
}
void EnemySpawner::render(sf::RenderTarget& target)
{
	for (auto& e : _Enemies_m)
	{
		e->render(target);
	}
}
void EnemySpawner::spawn_Enemy()
{
	auto new_Enemy = std::make_unique<Enemy>();

	new_Enemy->set_Position(get_Random_Spawn_Position());

	_Enemies_m.push_back(std::move(new_Enemy));

	current_Enemy_Count_m++;

}
sf::Vector2f EnemySpawner::get_Random_Spawn_Position()
{
	//get seed
	std::random_device rd;
	//intialise  m twister
	std::mt19937 gen(rd());

	//uniform real distribution= random float values betweentwo values
	std::uniform_real_distribution<float> distance_X(spawn_Area_Min_m.x, spawn_Area_Max_m.x);
	std::uniform_real_distribution<float> distance_Y(spawn_Area_Min_m.y, spawn_Area_Max_m.y);

	return { distance_X(gen),distance_Y(gen) };

}
std::vector<std::unique_ptr<Enemy>>& EnemySpawner::get_Enemies()
{
	return _Enemies_m;
}
