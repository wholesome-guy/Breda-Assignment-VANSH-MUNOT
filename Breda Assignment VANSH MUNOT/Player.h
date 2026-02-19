#pragma once
#include "EventSystem.h"
#include "EntityBase.h"
#include "Weapon.h"
#include <cmath>
#include <random>

class EnemySpawner;

struct character_Values 
{
	float character_Speed;
	int character_Terraforming_Factor;
	float max_character_Health;
	sf::Texture character_Texture;
	int character_Index;
};

class Player: public EntityBase,public Observer,public Subject
{
public:
	//constructor
	Player();

	//core loops
	void update(float deltatime) override;
	void render(sf::RenderTarget& target) override;

	void player_Health(float _Damage);
	bool get_can_Damage();

	void on_Event(const Event& event) override;

	//getters
	sf::Vector2f get_Position();
	sf::FloatRect get_GlobalBounds();

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
	int terraforming_Factor;
	float max_player_Health;
	float current_player_Health;

	character_Values Penguin;
	character_Values Crocodile;
	character_Values Peacock;
	int current_Character;
	int last_Character = 0;

	float invincibility_Time;
	float invincibility_Timer;
	bool can_Damage =true;

	Weapon* current_Weapon = nullptr;
	Weapon* _Sword = nullptr;
	Weapon* _Rifle = nullptr;
	Weapon* _RPG = nullptr;
	Weapon* _Mace = nullptr;
	Weapon* _WarAxe = nullptr;
	Weapon* _LMG = nullptr;
	Weapon* _Pistol = nullptr;
	Weapon* _Grenade_Launcher = nullptr;
	Weapon* _Shotgun = nullptr;
	int last_Weapon = 0;

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

	//randomiser
	std::mt19937 rng;
	std::random_device rd ;

	//events
	player_Ammo_Event ammo_Event;
	player_Health_Event health_Event;
	weapon_Reload_Cooldown reload_Weapon;
	weapon_Transform_Cooldown transform_Weapon_Event;
	weapon_State weapon_State_Event;
	player_terraforming_Factor_Event terraforming_Factor_Event;
	game_Difficulty game_Difficulty_Event;
	transformaion_Event transform_Event;

	//functions
	void init_playerSprite();
	void init_Variables();
	void init_Weapons();
	void init_Character();


	void player_Movement(float deltatime);
	void weapon_Movement();

	void wall_Collision();
	void enemy_Collision(float deltatime);

	void player_Attack();
	void Attack();

	void player_Invincibility(float deltatime);

	void weapon_Cooldown(float deltatime);
	void transform_Weapon(bool);
	void weapon_Transformation_Cooldown(float deltatime);
	void weapon_Assigner(Weapon* weapon);

	void character_Transform();
	void character_Assigner(character_Values,sf::Vector2f);
};

