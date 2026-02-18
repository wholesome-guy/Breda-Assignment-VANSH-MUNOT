#include "GameEngine.h"
#include "Player.h"
#include "PlayerUI.h"

//singleton method 
//a pointer of instance is set to null
GameEngine* GameEngine::instance = nullptr;

//constructor -- unity start method
GameEngine::GameEngine() 
{
	//instance is assigned
	instance = this;
	init_gameWindow();
	init_Entities();
}
//destructor -- unity on destroy method
GameEngine::~GameEngine()
{
	delete game_Window;
	//clean instance
	instance = nullptr;
}

//intialise the game window
void GameEngine::init_gameWindow()
{
	_Window_Size = { 1280,720 };
	game_Window = new sf::RenderWindow(sf::VideoMode(_Window_Size), "Game");
	//game_Window->setFramerateLimit(60);
}


void GameEngine::init_Entities()
{
	//make enemyspawner
	auto enemy_list = std::make_unique<EnemySpawner>();
	//raw pointer
	_EnemySpawner = enemy_list.get();

	//make player
	auto player = std::make_unique<Player>();
	//pass for reference raw pointer
	_Player = player.get();

	auto player_UI_sptr = std::make_unique<PlayerUI>();

	//Player Ui is the observer for Player
	_Player->add_Observer(player_UI_sptr.get());

	//transfer ownership by using move? smart pointer owns the object, transfer of ownership = move
	Entities.push_back(std::move(player));

	Entities.push_back(std::move(enemy_list));

	Entities.push_back(std::move(player_UI_sptr));

	//making player the observe of enemy_spawner
	_EnemySpawner->add_Observer(_Player);
}

void GameEngine::run()
{
	// update and render loop when game is running
	while (game_Window->isOpen())
	{
		poll_Event();

		//deltatime unity deltatime, time between previous and current frame
		float deltatime = clock.restart().asSeconds();
		//loops
		update(deltatime);
		render();
	}
}


//method to check(poll) events of the game window/ application
void GameEngine::poll_Event()
{
	while (auto event = game_Window->pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{
			game_Window->close();
		}
	}
}

//unity update
void GameEngine::update(float deltatime)
{
	for (auto& e : Entities)
	{
		e->update(deltatime);
	}	
}

//render objects here
void GameEngine::render()
{
	game_Window->clear();

	for (auto& e : Entities)
	{
		e->render(*game_Window);
	}

	game_Window->display();
}

bool GameEngine::get_Mini_Game_Active()
{
	return minigame_Active;
}
void GameEngine::set_Mini_Game_Active(bool active)
{
	minigame_Active = active;
}

