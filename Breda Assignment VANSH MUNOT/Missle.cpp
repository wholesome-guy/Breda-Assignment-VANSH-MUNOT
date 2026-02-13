#include "Missle.h"

#include "GameEngine.h"
Missle::Missle(sf::Vector2f position, sf::Angle rotation, sf::Vector2f direction, float _Damage, float _Range) : Projectile()
{
	init_Sprite(position, rotation);
	init_Variables(_Damage, _Range);

	float magnitude = sqrtf((direction.x * direction.x) + (direction.y * direction.y));

	projectile_Velocity = { (direction.x / magnitude) * _Speed, (direction.y / magnitude) * _Speed };


}

void Missle::init_Variables(float damage,float range)
{
	_Speed = 200.f;
	_Damage = damage;
	despawn_Time = range;
	despawn_Timer = 0.f;
	should_Despawn = false;
	is_Moving = true;
	explosion_Radius = 200;
}

void Missle::init_Sprite(sf::Vector2f position, sf::Angle rotation)
{
	projectile_Texture = sf::Texture(sf::Image("C:/Users/vansh/CPP Games/Breda Assignment/Source/Repository/Breda Assignment VANSH MUNOT/Assets/Player/Sword_PNG.png"));
	projectile_Sprite = sf::Sprite(projectile_Texture);
	projectile_Sprite.setTexture(projectile_Texture);
	projectile_Sprite.setScale({ 1,1 });
	projectile_Sprite.setOrigin({ static_cast<float>(projectile_Texture.getSize().x / 2),static_cast<float>(projectile_Texture.getSize().y / 2) });


	projectile_Sprite.setPosition(position);
	projectile_Sprite.setRotation(rotation);
}
void Missle::update(float deltatime)
{
	if (is_Moving)
	{
		projectile_Sprite.move(projectile_Velocity * deltatime);
	}

	if (despawn_Timer < despawn_Time)
	{
		despawn_Timer += deltatime;
	}
	else
	{
		explosion();
	}

	collision();

}
void Missle::render(sf::RenderTarget& target)
{
	target.draw(projectile_Sprite);
}

bool Missle::shouldDespawn() const
{
	return should_Despawn;
}

void Missle::collision()
{
	auto& enemies = GameEngine::get_Instance()->get_Enemies();

	for (auto& e : enemies)
	{
		if (projectile_Sprite.getGlobalBounds().findIntersection(e->get_GlobalBounds()))
		{
			explosion();
		}
	}

}

void Missle::explosion()
{
	is_Moving = false;

	sf::Vector2f missile_Position = projectile_Sprite.getPosition();
	auto& enemies = GameEngine::get_Instance()->get_Enemies();

	for (auto& e : enemies)
	{
		sf::Vector2f enemy_Position = e->get_Position();

		sf::Vector2f difference = missile_Position - enemy_Position;

		float magnitude = std::sqrtf(difference.x * difference.x + difference.y * difference.y);

		if (magnitude < explosion_Radius)
		{
			e->take_Damage(_Damage);
		}

	}

	should_Despawn = true;
	std::cout << "explosion" << "\n";
}
