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


private:

	sf::Texture player_Texture;
	sf::Sprite player_Sprite;

	sf::RenderWindow* game_Window;
	float player_Speed;
	float max_player_Health;
	float player_Health;

	Weapon* current_Weapon = nullptr;
	Weapon* _Sword = nullptr;
	Weapon* _Rifle = nullptr;
	Weapon* _RPG = nullptr;

	bool can_Attack = true;
	bool is_weapon_Cooldown = false;

	int current_weapon_Ammo;
	float current_weapon_Cooldown;
	float weapon_cooldown_Timer =0;

	void init_playerSprite();
	void init_UI();
	void init_Variables();
	void player_Movement(float deltatime);
	void wall_Collision();
	void enemy_Collision();

	void player_Attack();

	void init_Weapons();
	void weapon_Movement();
	void weapon_Cooldown(float deltatime);
	void transform_Weapon();
	void weapon_Assigner(Weapon* weapon);


	float sprite_Offset_X;
	float sprite_Offset_Y;
	float screen_Width;
	float screen_Height;

	//UI
	sf::Font game_Font;
	sf::Text ammo_Text;
	sf::Text health_Text;

	sf::Texture ammo_Texture;
	sf::Sprite ammo_Sprite;
	sf::Texture health_Texture;
	sf::Sprite health_Sprite;
	void render_UI(sf::RenderTarget& target);
	void update_UI();

};

