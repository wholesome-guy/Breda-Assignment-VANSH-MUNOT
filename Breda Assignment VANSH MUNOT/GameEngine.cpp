#include "GameEngine.h"

#include "Player.h"
#include "Sword.h"
#include "Rifle.h"
#include "RPG.h"
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
	delete game_Window_m;
	//clean instance
	instance = nullptr;
}

//intialise the game window
void GameEngine::init_gameWindow()
{
	game_Window_m = new sf::RenderWindow(sf::VideoMode({ 960,540 }), "Game");
	game_Window_m->setFramerateLimit(60);
}

void GameEngine::init_Entities()
{
	//created a sword and set the smart pointer
	auto _Sword = std::make_unique<Sword>();
	Sword* sword_Ptr = _Sword.get();   
	//assign a raw pointer with the smartpointer,because GameEngine.cpp owns the sword. 
	// the player can only refer to it.
	//smart pointer = ownership(cant be copied), raw pointer = no ownership, just uses the address copyable

	auto _Rifle = std::make_unique<Rifle>();
	Rifle* rifle_Ptr = _Rifle.get();

	auto _RPG = std::make_unique<RPG>();
	RPG* rpg_Ptr = _RPG.get();

	//makes player
	auto _Player = std::make_unique<Player>();

	//set sword
	_Player->set_Sword(sword_Ptr);
	_Player->set_Rifle(rifle_Ptr);
	_Player->set_RPG(rpg_Ptr);

	//move is used to transfer ownership from smart pointer to entity list
	Entities.push_back(std::move(_Player));
	Entities.push_back(std::move(_Sword));
	Entities.push_back(std::move(_Rifle));
	Entities.push_back(std::move(_RPG));

}

void GameEngine::run()
{
	// update and render loop when game is running
	while (game_Window_m->isOpen())
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
	while (auto event = game_Window_m->pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{
			game_Window_m->close();
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
	game_Window_m->clear();

	for (auto& e : Entities)
	{
		e->render(*game_Window_m);
	}

	game_Window_m->display();
}
