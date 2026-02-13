#pragma once
#include <SFML/Graphics.hpp>

class Projectile
{
public:

	Projectile() :projectile_Sprite(projectile_Texture) {};
	virtual ~Projectile() = default;

	virtual void update(float deltatime) = 0;

	virtual void render(sf::RenderTarget& target) = 0;

	virtual bool shouldDespawn() const = 0;

protected:
	sf::Texture projectile_Texture;
	sf::Sprite projectile_Sprite;

	float _Speed;
	float despawn_Time;
	float despawn_Timer;
	float _Damage;

	sf::Vector2f projectile_Velocity;

	bool should_Despawn;

	virtual void init_Sprite(sf::Vector2f position, sf::Angle rotation) = 0;
	virtual void init_Variables(float _Damage, float _Range) = 0;

	virtual void collision() =0;
 
};

