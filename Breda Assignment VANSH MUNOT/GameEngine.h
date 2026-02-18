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
#include "TranformationMiniGame.h"
#include "EventSystem.h"
class PlayerUI;
class Observer;
class GameEngine:public Observer, public Subject
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


	EnemySpawner* get_EnemySpawner()
	{
		return _EnemySpawner;
	}

	Observer* get_UI()
	{
		return _PlayerUI;
	}

	void on_Event(const Event& event) override;

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
	Observer* _PlayerUI;

	TranformationMiniGame mini_Game;
	//events
	minigame_Active_State minigame_Active_Event;
	bool minigame_Completed_Handled = false;
	bool pending_MiniGame_Start = false;
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

