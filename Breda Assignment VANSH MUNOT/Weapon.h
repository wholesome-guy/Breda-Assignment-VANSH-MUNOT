#pragma once
#include <SFML/Graphics.hpp>

class Weapon
{
public:

	Weapon():weapon_Sprite(weapon_Texture) {};
	virtual ~Weapon() = default;

	virtual void update(float deltatime) = 0;
	virtual void render(sf::RenderTarget& target) = 0;

	int _Ammo;
	float cooldown_Timer;

	virtual void Attack() =0;
	virtual void weapon_Position(sf::Vector2f player_Position) = 0;
	virtual void weapon_Rotate(sf::RenderWindow& gameWindow) = 0;


protected:

	float _Damage;
	float _Range;

	sf::Texture weapon_Texture;
	sf::Sprite weapon_Sprite;


};

