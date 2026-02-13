#pragma once

#include "Weapon.h"
#include "Projectile.h"
#include "Bullet.h"
#include <vector>

class Rifle : public Weapon
{
public:

	Rifle();

	void update(float deltatime) override;
	void render(sf::RenderTarget& target) override;

	void weapon_Position(sf::Vector2f player_Position) override;
	void weapon_Rotate(sf::RenderWindow& gameWindow) override;
	void weapon_Scale(sf::Vector2f _Scale) override;


	void Attack() override;

private:

	void init_Variables();
	void init_Sprite();

	std::vector<std::unique_ptr<Bullet>> Bullets;

};

