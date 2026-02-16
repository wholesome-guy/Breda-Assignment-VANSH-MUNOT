#pragma once

#include "Weapon.h"
#include "Missile.h"
class RPG : public Weapon
{
public:

	RPG();

	void update(float deltatime) override;
	void render(sf::RenderTarget& target) override;

	void weapon_Position(sf::Vector2f player_Position) override;
	void weapon_Rotate(sf::RenderWindow& gameWindow) override;
	void weapon_Scale(sf::Vector2f _Scale) override;
	//void weapon_Colour(sf::Color _Colour) override;


	void Attack() override;

private:

	void init_Variables();
	void init_Sprite();

	//missiles array
	std::vector<std::unique_ptr<Missile>> Missiles;


};

