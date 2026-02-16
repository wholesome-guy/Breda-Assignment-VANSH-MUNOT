#pragma once
#include "Projectile.h"

class Missile:public Projectile
{
public:
	Missile(sf::Vector2f position, sf::Angle rotation, sf::Vector2f direction, float _Damage, float _Range);

	void update(float deltatime) override;

	void render(sf::RenderTarget& target) override;

	bool shouldDespawn() const override;

private:

	sf::Texture explosion_Texture;
	sf::Sprite explosion_Sprite;

	bool end_Explosion = false;
	bool can_Damage = true;
	bool is_Moving = true;
	bool is_Exploding = false;

	float explosion_Radius;
	float explosion_Timer;
	float explosion_Time = 3;
	
	//Function

	void init_Sprite(sf::Vector2f position, sf::Angle rotation) override;
	void init_Variables(float _Damage, float _Range) override;
	void collision() override;


	void explosion();



};

