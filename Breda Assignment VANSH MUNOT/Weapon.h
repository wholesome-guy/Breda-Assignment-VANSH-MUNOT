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
	float weapon_RotationAngle;


	virtual void Attack() =0;
	virtual void weapon_Position(sf::Vector2f player_Position) = 0;
	virtual void weapon_Rotate(sf::RenderWindow& gameWindow) = 0;
	virtual void weapon_Scale(sf::Vector2f _Scale)=0;
	virtual void weapon_Colour(sf::Color colour)
	{
		weapon_Sprite.setColor(colour);
	}
protected:

	float _Damage;
	float _Range;

	float Radians_To_Degrees = 180 / 3.14f;

	sf::Texture weapon_Texture;
	sf::Sprite weapon_Sprite;

};

