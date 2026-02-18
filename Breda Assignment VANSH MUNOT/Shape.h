#pragma once
#include <SFML/Graphics.hpp>
#include "TranformationMiniGame.h"
#include "EventSystem.h"
class Player;
class GameEngine;
class Shape:public Subject,public Observer
{
public:

	Shape(sf::Vector2f position,sf::Color);
	virtual ~Shape();

	void render(sf::RenderTarget& target);
	void update(float deltatime);
	void on_Event(const Event& event) override;
	bool get_Despawn();

private:

	sf::Texture square_Texture;
	sf::Sprite shape_Sprite;

	bool is_MiniGame_Active = false;
	//minigame_Active_State minigame_Active_Event;

	interaction_State minigame_Interaction_State;

	GameEngine* _GameEngine;

	float despawn_Timer = 0;
	float despawn_Time = 5.f;

	bool is_Despawn = false;

	Player* _Player;
	//TranformationMiniGame mini_Game;

	void init_Sprite(sf::Vector2f position, sf::Color);
	void despawn(float deltatime);
	void collision();
};

