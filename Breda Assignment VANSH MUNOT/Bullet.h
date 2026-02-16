#pragma once

#include "Projectile.h"

class Bullet:public Projectile
{
public:

	Bullet(sf::Vector2f position, sf::Angle rotation, sf::Vector2f direction,float _Damage, float _Range);

	void update(float deltatime) override;

	void render(sf::RenderTarget& target) override;

	bool shouldDespawn() const override;
	

private:

	void init_Sprite(sf::Vector2f position, sf::Angle rotation) override;
	void init_Variables(float _Damage, float _Range) override;
	void collision() override;

};

