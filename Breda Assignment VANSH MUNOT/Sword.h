#pragma once

#include "EntityBase.h"
#include "Weapon.h"

class Sword : public Weapon,public EntityBase
{
public:

	Sword();

	void update(float deltatime) override;
	void render(sf::RenderTarget& target) override;	 

	void weapon_Position(sf::Vector2f player_Position) override;
	void weapon_Rotate(sf::RenderWindow& gameWindow) override;

	void Attack() override;

private:
	
	void init_Variables();
	void init_Sprite();
	

};

