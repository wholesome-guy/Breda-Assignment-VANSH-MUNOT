#pragma once
#include "EntityBase.h"

//forward declaration when i only need a pointer or reference
class Player;

class Enemy
{

public:

	Enemy();
	~Enemy() = default;

	//core loop
	void update(float deltatime);
	void render(sf::RenderTarget& target);

	void take_Damage(float _Damage);

	//setters
	void set_Position(sf::Vector2f position);
	void set_Color(sf::Color colour);

	//getters
	bool get_Dead_Bool();
	float get_Damage();
	sf::Vector2f get_Position();
	sf::FloatRect get_GlobalBounds();

private:

	//properties
	int type;
	float enemy_Speed;
	float enemy_Health;
	float max_enemy_Health;
	float enemy_Damage;
	bool is_Dead = false;

	//sprite
	sf::Texture enemy_Texture;
	sf::Sprite enemy_Sprite;

	//player reference
	Player* _Player;
	sf::Vector2f player_enemy_Direction;

	//flash effect
	bool start_Flash = false;
	float flash_Time;
	float flash_Timer;

	//health bars
	sf::RectangleShape health_Bar;
	sf::RectangleShape background_Bar;

	//knockback
	sf::Vector2f knockback_Velocity;
	float knockback_Force =10000;
	float knockback_Decay = 0.9f;

	//functions
	void init_Variables();
	void init_Sprite();
	void random_Enemy_Type();

	void direction_Calculate();
	void enemy_Movement(float deltatime);

	void enemy_Flashing(float deltatime);

	void health_Bar_Update();



};

