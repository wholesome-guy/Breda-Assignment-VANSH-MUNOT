#pragma once

#include <SFML/Graphics.hpp>
#include<SFML/Main.hpp>
#include <SFML/Window.hpp>
#include<SFML/System.hpp>
#include<SFML/Audio.hpp>
#include <iostream>
#include<vector>
#include "EntityBase.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemySpawner.h"
#include "TranformationMiniGame.h"
#include "EventSystem.h"
#include "CameraShake.h"
#include "ParticleSystem.h"
#include "SFXSystem.h"

class PlayerUI;
class Observer;
class TileMap;
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

	//xamera shake
	CameraShake shake;
	sf::View _Camera;
	sf::Vector2f base_Center;
	// a kind of array that stores smart pointer to enitity base classes 
	// thus to all entity derived classes
	std::vector<std::unique_ptr<EntityBase>> Entities;

	Player* _Player;
	EnemySpawner* _EnemySpawner;
	Observer* _PlayerUI;
	TileMap* _TileMap;
	ParticleSystem particle_System;
	SFXSystem sfx_System;

	TranformationMiniGame mini_Game;

	//events
	minigame_Active_State minigame_Active_Event;
	minigame_Win miniGame_Win_Event;

	sf::Texture cursor_Texture;
	sf::Sprite cursor_Sprite;

	sf::Texture title_Texture;
	sf::Sprite title_Sprite;

	sf::Font game_Font;
	sf::Text game_Text;
	sf::Text end_Text;

	sf::Music game_Music;

	bool minigame_Completed_Handled = false;
	bool pending_MiniGame_Start = false;

	bool is_Game_Start = false;
	bool is_Game_Over = false;
	int game_End_State;

	//basic functions
	void update(float deltatime);
	void render();
	//poll = check events of the window
	void poll_Event();

	void cursor();
	void cameraShake(float deltatime);

	// intialises Entities
	void init_Entities();
	//intialise window
	void init_gameWindow();
	void init_UI();

	void game_Text_Render();

	void miniGame_Update(float deltatime);

};

