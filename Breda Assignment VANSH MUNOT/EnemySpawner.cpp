#include "EnemySpawner.h"
#include "GameEngine.h"

#include "Shape.h"

EnemySpawner::EnemySpawner():random_Generator(seed())
{
	init_Variables();
}
void EnemySpawner::init_Variables()
{
	spawn_Interval = 2.f;
	max_Enemies = 10;
	spawn_Timer = 0;

	sf::Vector2u window_Size = GameEngine::get_Instance()->get_Window()->getSize();

	spawn_Area_Min = { -1.0f * static_cast<float>(window_Size.x) ,-1.0f * static_cast<float>(window_Size.y) };
	spawn_Area_Max = { 2.f * static_cast<float>(window_Size.x) ,2.f * static_cast<float>(window_Size.y) };
}
void EnemySpawner::update(float deltatime)
{
	if (!GameEngine::get_Instance()->get_Mini_Game_Active())
	{
		spawn_Timer += deltatime;

		if (spawn_Timer >= spawn_Interval && current_Enemy_Count < max_Enemies)
		{
			spawn_Enemy();
			spawn_Timer = 0.0f; // Reset timer
		}

		for (int i = 0; i < _Enemies.size(); i++)
		{
			update_Enemy(i, deltatime);

			if (knockback)
			{
				_Enemies[i]->enemy_KnockBack();
				knockback = false;
			}

			erase_Enemy(i);
		}

		
	}
	else
	{
		knockback = true;
	}
	
	for (int i = 0; i < _Squares.size(); i++)
	{
		_Squares[i]->update(deltatime);

		 erase_Square(i);
	}
}
void EnemySpawner::render(sf::RenderTarget& target)
{
	for (auto& e : _Enemies)
	{
		e->render(target);
	}
	for (auto& s : _Squares)
	{
		s->render(target);
	}
}
void EnemySpawner::spawn_Enemy()
{
	auto new_Enemy = std::make_unique<Enemy>();

	new_Enemy->set_Position(get_Random_Spawn_Position());

	_Enemies.push_back(std::move(new_Enemy));

	current_Enemy_Count++;

}
void EnemySpawner::update_Enemy(int i,float deltatime)
{
	_Enemies[i]->update(deltatime);
}

void EnemySpawner::erase_Enemy(int i)
{
	if (_Enemies[i]->get_Dead_Bool())
	{
		spawn_Square(i);

		_Enemies.erase(_Enemies.begin() + i);
		current_Enemy_Count--;
		killed_Enemy_Count++;
	}
}
sf::Vector2f EnemySpawner::get_Random_Spawn_Position()
{

	//uniform real distribution= random float values betweentwo values
	std::uniform_real_distribution<float> distance_X(spawn_Area_Min.x, spawn_Area_Max.x);
	std::uniform_real_distribution<float> distance_Y(spawn_Area_Min.y, spawn_Area_Max.y);

	return { distance_X(random_Generator),distance_Y(random_Generator) };

}
sf::Color EnemySpawner::get_Square_Color()
{
	sf::Color colour;

	std::uniform_int_distribution<int> random(0, 3);

	switch (random(random_Generator))
	{
	case 0: colour = sf::Color::Red;
		break;
	case 1: colour = sf::Color::Green;
		break;
	case 2: colour = sf::Color::Blue;
		break;
	case 3: colour = sf::Color::Yellow;
		break;
	}
	return colour;

}
void EnemySpawner::spawn_Square(int i)
{
	std::uniform_int_distribution<int> random(0, 100);

	if (random(random_Generator) < 100)//make it 30 later
	{
		_Squares.push_back(std::make_unique<Shape>(_Enemies[i]->get_Position(), get_Square_Color()));
	}
}
void EnemySpawner::erase_Square(int i)
{
	if (_Squares[i]->get_Despawn())
	{
		_Squares.erase(_Squares.begin() + i);
	}
}
std::vector<std::unique_ptr<Enemy>>& EnemySpawner::get_Enemies()
{
	return _Enemies;
}
int EnemySpawner::get_Kill_Count()
{
	return killed_Enemy_Count;
}

