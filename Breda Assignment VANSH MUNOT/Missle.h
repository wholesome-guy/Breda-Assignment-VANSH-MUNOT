#pragma once
#include "Projectile.h"

class Missle :public Projectile
{
public:
	Missle(sf::Vector2f position, sf::Angle rotation, sf::Vector2f direction, float _Damage,float _Range);

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

};

