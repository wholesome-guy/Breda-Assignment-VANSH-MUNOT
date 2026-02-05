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

	void set_Sword(Weapon* weapon);
	void set_Rifle(Weapon* weapon);
	void set_RPG(Weapon* weapon);



private:

	sf::Texture player_Texture_m;
	sf::Sprite player_Sprite_m;

	sf::RenderWindow* game_Window_m;
	float player_Speed_m;

	Weapon* current_Weapon_m = nullptr;
	Weapon* _sword_m = nullptr;
	Weapon* _rifle_m = nullptr;
	Weapon* _RPG_m = nullptr;

	bool can_Attack_m = true;
	bool is_weapon_Cooldown_m = false;

	int current_weapon_Ammo_m;
	float current_weapon_Cooldown_m;
	float weapon_cooldown_Timer_m =0;

	void init_playerSprite();
	void init_Variables();
	void player_Movement();
	void player_Attack();
	void weapon_Cooldown(float deltatime);
	void transform_Weapon();

};

