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
		return game_Window; 
	}

	Player* get_Player()
	{
		return _Player;
	}

	sf::Vector2u get_Window_Size()
	{
		return _Window_Size;
	}

	std::vector<std::unique_ptr<Enemy>>& get_Enemies();
	

private:
	
	//static instance
	static GameEngine* instance;

	sf::Clock clock;
	sf::RenderWindow* game_Window;
	sf::Vector2u _Window_Size;
	// a kind of array that stores smart pointer to enitity base classes 
	// thus to all entity derived classes
	std::vector<std::unique_ptr<EntityBase>> Entities;

	Player* _Player;
	EnemySpawner* _EnemySpawner;
	//basic functions
	void update(float deltatime);
	void render();

	// intialises Entities
	void init_Entities();

	//poll = check events of the window
	void poll_Event();
	//intialise window
	void init_gameWindow();

	//FPS 
	float updateInterval = 0.5f; // How often to update the FPS display

	float accum = 0; // FPS accumulated over the interval
	int frames = 0; // Frames drawn over the interval
	float timeleft = updateInterval;

	void FPS_Counter(float deltatime);

	sf::Text FPS_Text;
	sf::Font game_Font;

	void init_Text();

};

