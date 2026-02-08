#pragma once

#include <SFML/Graphics.hpp>
#include<SFML/Main.hpp>
#include <SFML/Window.hpp>
#include<SFML/System.hpp>
#include <iostream>
#include<vector>
#include "EntityBase.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemySpawner.h"

class GameEngine
{
public:

	//constructor
	GameEngine();

	//destructor
	virtual ~GameEngine();
	// literally runs the game
	void run();

	// Singleton access
	static GameEngine* get_Instance() 
	{ 
		return instance; 
	}

	// Get the game window
	sf::RenderWindow* get_Window() 
	{ 
		return game_Window_m; 
	}

	Player* get_Player()
	{
		return _Player_m;
	}

	std::vector<std::unique_ptr<Enemy>>& get_Enemies();
	

private:
	
	//static instance
	static GameEngine* instance;

	sf::Clock clock;
	sf::RenderWindow* game_Window_m;
	// a kind of array that stores smart pointer to enitity base classes 
	// thus to all entity derived classes
	std::vector<std::unique_ptr<EntityBase>> Entities;

	Player* _Player_m;
	EnemySpawner* _EnemySpawner_m;
	//basic functions
	void update(float deltatime);
	void render();

	// intialises Entities
	void init_Entities();

	//poll = check events of the window
	void poll_Event();
	//intialise window
	void init_gameWindow();

};

