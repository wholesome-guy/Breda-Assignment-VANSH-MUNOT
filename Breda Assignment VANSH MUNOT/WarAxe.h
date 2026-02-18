#pragma once
#include "Weapon.h"
#include "EnemySpawner.h"
class WarAxe: public Weapon
{
public:
	WarAxe();

	void update(float deltatime) override;
	void render(sf::RenderTarget& target) override;

	void weapon_Position(sf::Vector2f player_Position) override;
	void weapon_Rotate(sf::RenderWindow& gameWindow) override;
	void weapon_Scale(sf::Vector2f _Scale) override;

	void Attack() override;

private:

	void init_Variables();
	void init_Sprite();


	bool is_Swinging;
	float swing_Timer;
	float swing_Duration;
	float swing_Start_Angle;
	float swing_End_Angle;

	EnemySpawner* _EnemySpawner;

	void attack_Animation_Intialiser();
	void attack_Animation_Update(float deltatime);
	void attack_Enemy_Collision();
};

