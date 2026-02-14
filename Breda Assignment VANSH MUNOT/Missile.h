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

	void init_Sprite(sf::Vector2f position, sf::Angle rotation) override;
	void init_Variables(float _Damage, float _Range) override;
	void collision() override;
	void explosion();

	bool is_Moving = true;
	float explosion_Radius;

	sf::Texture explosion_Texture;
	sf::Sprite explosion_Sprite;

	bool is_Exploding = false;
	float explosion_Timer;
	float explosion_Time = 3;

	bool end_Explosion = false;

	bool can_Damage = true;

};

