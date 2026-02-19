#include "EnemySpawner.h"
#include "GameEngine.h"
#include "Player.h"
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

	_GameEngine = GameEngine::get_Instance();

	if (!spawn.loadFromFile("Assets/Sound/Spawn_MP3.mp3"))
		std::cout << "Failed to load Heal\n";
}
void EnemySpawner::update(float deltatime)
{
	if (!is_MiniGame_Active)
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

			erase_Enemy(i);
		}
	}
	for (int i=0;i<_Squares.size();i++)
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

void EnemySpawner::on_Event(const Event& event)
{
	if (auto* data = dynamic_cast<const minigame_Active_State*>(&event))
	{
		is_MiniGame_Active = data->active;
	}
	else if (auto* data = dynamic_cast<const game_Difficulty*>(&event))
	{
		spawn_Timer = data->spawn_Time;
		max_Enemies = data->max_Enemies;
		damage_Multiplier = data->damage_Multiplier;
		health_Multiplier = data->Health_Multiplier;

	}
}

void EnemySpawner::spawn_Enemy()
{
	Player* player = GameEngine::get_Instance()->get_Player();

	auto new_Enemy = std::make_unique<Enemy>(damage_Multiplier, health_Multiplier, get_Random_Spawn_Position());

	//adding observer, player and enemyspawner are observers of enemy
	new_Enemy->add_Observer(player);
	new_Enemy->add_Observer(_GameEngine);

	//player's observer is enemy
	player->add_Observer(new_Enemy.get());
	_GameEngine->add_Observer(new_Enemy.get());

	_Enemies.push_back(std::move(new_Enemy));

	current_Enemy_Count++;

}
void EnemySpawner::update_Enemy(int i, float deltatime)
{
	_Enemies[i]->update(deltatime);
}

void EnemySpawner::erase_Enemy(int i)
{
	if (_Enemies[i]->get_Dead_Bool())
	{
		spawn_Square(i);

		_GameEngine->remove_Observer(_Enemies[i].get());

		Player* player = GameEngine::get_Instance()->get_Player();
		player->remove_Observer(_Enemies[i].get());

		_Enemies.erase(_Enemies.begin() + i);
		current_Enemy_Count--;
		kill_Enemy_Count++;

		//ui update
		kill_count.kill_Count = kill_Enemy_Count;
		notify_Observers(kill_count);

		//heal player on kill Event
		player_Health_Event._Change = -5.f;
		notify_Observers(player_Health_Event);

		//on kill reduce terraforming time
		notify_Observers(reduce_Terraforming_Event);
	}
}
sf::Vector2f EnemySpawner::get_Random_Spawn_Position()
{

	//uniform real distribution= random float values betweentwo values
	std::uniform_real_distribution<float> distance_X(spawn_Area_Min.x, spawn_Area_Max.x);
	std::uniform_real_distribution<float> distance_Y(spawn_Area_Min.y, spawn_Area_Max.y);

	return { distance_X(random_Generator),distance_Y(random_Generator) };

}

void EnemySpawner::spawn_Square(int i)
{
	std::uniform_int_distribution<int> random(0, 100);

	if (random(random_Generator) < 10)//make it 30 later
	{
		auto new_Shape = std::make_unique<Shape>(_Enemies[i]->get_Position());
		
		//player ui and enemyspawner are observer of shape
		new_Shape->add_Observer(GameEngine::get_Instance()->get_UI());
		new_Shape->add_Observer(this);
		//game engine is observer of shape so shape can invoke mini game
		new_Shape->add_Observer(_GameEngine);

		//shape is oberserve of gameengine
		_GameEngine->add_Observer(new_Shape.get());


		sfx_Event.buffer = &spawn;
		sfx_Event.volume = 100;
		sfx_Event.pitch = 1;
		sfx_Event.randomise_pitch = false;
		notify_Observers(sfx_Event);

		particle_System.position = _Enemies[i]->get_Position();
		particle_System.count = 100;
		particle_System.colour = sf::Color::Color(255, 101, 134);
		particle_System.speed = 2000;
		particle_System.lifetime = 5.f;
		notify_Observers(particle_System);


		_Squares.push_back(std::move(new_Shape));

	}
}
void EnemySpawner::erase_Square(int i)
{
	if (_Squares[i]->get_Despawn())
	{
		//clean up observer list
		_GameEngine->remove_Observer(_Squares[i].get());
		_Squares.erase(_Squares.begin() + i);				
	}	
}
std::vector<std::unique_ptr<Enemy>>& EnemySpawner::get_Enemies()
{
	return _Enemies;
}


