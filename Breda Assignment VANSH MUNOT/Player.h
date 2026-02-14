#pragma once

#include "EntityBase.h"
#include "Weapon.h"
#include <cmath>

class Player: public EntityBase
{
public:
	//constructor
	Player();

	void update(float deltatime) override;
	void render(sf::RenderTarget& target) override;

	sf::Vector2f get_Position();

	float get_Cooldown(int i);
	bool get_CoolDown_Bool(int i);

	float get_Health();
	int get_Ammo();
	


private:

	sf::Texture player_Texture;
	sf::Sprite player_Sprite;

	sf::RenderWindow* game_Window;
	float player_Speed;
	float max_player_Health;
	float player_Health;

	float invincibility_Time;
	float invincibility_Timer;
	bool can_Damage =true;

	Weapon* current_Weapon = nullptr;
	Weapon* _Sword = nullptr;
	Weapon* _Rifle = nullptr;
	Weapon* _RPG = nullptr;

	bool can_Attack = true;
	bool is_weapon_Cooldown = false;

	int current_weapon_Ammo = 0;
	float current_weapon_Cooldown;
	float weapon_cooldown_Timer = 0;

	float weapon_Transfrom_Time;
	float weapon_Transform_Timer;
	bool is_weapon_Transforming = false;

	void init_playerSprite();
	void init_Variables();
	void player_Movement(float deltatime);
	void wall_Collision();
	void enemy_Collision(float deltatime);

	void player_Attack();

	void init_Weapons();
	void weapon_Movement();
	void weapon_Cooldown(float deltatime);
	void transform_Weapon();
	void weapon_Transformation_Cooldown(float deltatime);
	void weapon_Assigner(Weapon* weapon);


	float sprite_Offset_X;
	float sprite_Offset_Y;
	float screen_Width;
	float screen_Height;

};

