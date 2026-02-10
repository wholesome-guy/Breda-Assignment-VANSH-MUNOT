#pragma once
#include "EntityBase.h"
#include "Player.h"

class Enemy
{

public:

	Enemy();
	~Enemy() = default;

	void update(float deltatime);
	void render(sf::RenderTarget& target);

	void take_Damage(float _Damage);

	sf::Vector2f get_Position();
	sf::FloatRect get_GlobalBounds();
	void set_Position(sf::Vector2f position);
	void set_Color(sf::Color colour);
private:

	void init_Variables();
	void init_Sprite();

	void enemy_Movement(float deltatime);

	float enemy_Speed;

	sf::Texture enemy_Texture;
	sf::Sprite enemy_Sprite;

	Player* _Player;

};

