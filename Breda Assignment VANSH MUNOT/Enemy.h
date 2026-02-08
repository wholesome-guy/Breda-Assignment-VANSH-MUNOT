#pragma once
#include "EntityBase.h"
#include "Player.h"

class Enemy:public EntityBase
{
public:

	Enemy();

	void update(float deltatime) override;
	void render(sf::RenderTarget& target) override;

	void take_Damage(float _Damage);

	sf::Vector2f get_Position();
	void set_Position(sf::Vector2f position);
private:

	void init_Variables();
	void init_Sprite();

	void enemy_Movement(float deltatime);

	float enemy_Speed_m;

	sf::Texture enemy_Texture_m;
	sf::Sprite enemy_Sprite_m;

	Player* _Player_m;

};

