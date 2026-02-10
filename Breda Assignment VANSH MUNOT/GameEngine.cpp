#include "GameEngine.h"
#include "Player.h"


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
	_Window_Size = { 640,360 };
	game_Window = new sf::RenderWindow(sf::VideoMode(_Window_Size), "Game");
	game_Window->setFramerateLimit(60);
}

void GameEngine::init_Entities()
{
	//make player
	auto player = std::make_unique<Player>();
	//pass for reference raw pointer
	_Player = player.get();
	//transfer ownership by using move? smart pointer owns the object, transfer of ownership = move
	Entities.push_back(std::move(player));


	auto enemy_list = std::make_unique<EnemySpawner>();

	_EnemySpawner = enemy_list.get();
	
	Entities.push_back(std::move(enemy_list));

}
std::vector<std::unique_ptr<Enemy>>& GameEngine::get_Enemies()
{
	return _EnemySpawner->get_Enemies();
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
