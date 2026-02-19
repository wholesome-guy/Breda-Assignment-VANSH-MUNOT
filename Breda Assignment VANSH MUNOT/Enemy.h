#pragma once
#include "EntityBase.h"
#include "EventSystem.h"
#include <SFML/Audio.hpp>

//forward declaration when i only need a pointer or reference
class Player;
class EnemySpawner;

class Enemy: public Subject,public Observer
{
public:

	Enemy(float,float,sf::Vector2f);
	~Enemy() = default;

	//core loop
	void update(float deltatime);
	void render(sf::RenderTarget& target);

	void on_Event(const Event& event) override;

	void take_Damage(float _Damage);
	void enemy_KnockBack();

	//setters
	void set_Position(sf::Vector2f position);
	void set_Color(sf::Color colour);

	//getters
	float get_Damage();
	bool get_Dead_Bool();
	sf::Vector2f get_Position();
	sf::FloatRect get_GlobalBounds();


private:

	//properties
	int type;
	float enemy_Speed;
	float enemy_Health;
	float max_enemy_Health;
	float enemy_Damage;

	//float damage_Multiplier = 1;
	//float health_Multiplier = 1;

	bool Despawning = false;
	bool is_Dead = false;
	sf::Vector2f movement_Direction;
	sf::FloatRect next_X_Bounds;
	sf::FloatRect next_Y_Bounds;


	float despawn_Time;
	float despawn_Timer;
	//sprite
	sf::Texture enemy_Texture;
	sf::Sprite enemy_Sprite;

	//player reference
	Player* _Player;
	sf::Vector2f player_enemy_Direction;

	EnemySpawner* _EnemySpawner = nullptr;

	bool is_InContact_Player;
	float contact_Timer =0;
	float contact_Time;

	//flash effect
	bool start_Flash = false;
	float flash_Time;
	float flash_Timer;

	//health bars
	sf::RectangleShape health_Bar;
	sf::RectangleShape background_Bar;

	//knockback
	sf::Vector2f knockback_Velocity;
	float knockback_Force =1000;
	float knockback_Decay = 0.9f;
	bool knockback_Applied = false;

	particle_Emit_Event particle_System;
	SFX_Event sfx_Event;


	sf::SoundBuffer damage;
	sf::SoundBuffer die;


	//functions
	void init_Variables(float, float);
	void init_Sprite(sf::Vector2f);
	void random_Enemy_Type();

	void direction_Calculate();
	void collision_Data_Calculate(float deltatime);
	void enemy_Movement(float deltatime);
	void enemy_player_Collision(float deltatime);
	void enemy_enemy_Collision(float deltatime);

	void enemy_Flashing(float deltatime);

	void health_Bar_Update();
	void despawning_Enemy(float deltatime);



};

