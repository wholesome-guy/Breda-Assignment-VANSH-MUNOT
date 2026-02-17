#pragma once

#include "EntityBase.h"
#include "Weapon.h"
#include <cmath>

class EnemySpawner;

class Player: public EntityBase
{
public:
	//constructor
	Player();

	//core loops
	void update(float deltatime) override;
	void render(sf::RenderTarget& target) override;

	void player_Health(float _Damage);


	//getters
	sf::Vector2f get_Position();
	sf::FloatRect get_GlobalBounds();

	float get_Cooldown(int i);
	bool get_CoolDown_Bool(int i);

	bool get_can_Damage();
	float get_Health();
	int get_Ammo();

	bool get_Can_Interact_Square();


	//setters
	void set_Can_Interact_Sqaure(bool);


private:

	sf::Texture player_Texture;
	sf::Sprite player_Sprite;

	sf::RenderWindow* game_Window;

	float sprite_Offset_X;
	float sprite_Offset_Y;
	float screen_Width;
	float screen_Height;

	float player_Speed;
	sf::Vector2f player_Velocity;
	float max_player_Health;
	float current_player_Health;

	float invincibility_Time;
	float invincibility_Timer;
	bool can_Damage =true;

	Weapon* current_Weapon = nullptr;
	Weapon* _Sword = nullptr;
	Weapon* _Rifle = nullptr;
	Weapon* _RPG = nullptr;

	EnemySpawner* _EnemySpawner;
	bool can_Attack = true;
	bool is_weapon_Cooldown = false;

	int current_weapon_Ammo = 0;
	float current_weapon_Cooldown;
	float weapon_cooldown_Timer = 0;

	float weapon_Transfrom_Time;
	float weapon_Transform_Timer;
	bool is_weapon_Transforming = false;

	bool can_Interact_Square;

	//functions
	void init_playerSprite();
	void init_Variables();
	void init_Weapons();


	void player_Movement(float deltatime);
	void weapon_Movement();

	void wall_Collision();
	void enemy_Collision(float deltatime);

	void player_Attack();
	void Attack();

	void player_Invincibility(float deltatime);

	void weapon_Cooldown(float deltatime);
	void transform_Weapon();
	void weapon_Transformation_Cooldown(float deltatime);
	void weapon_Assigner(Weapon* weapon);
};

