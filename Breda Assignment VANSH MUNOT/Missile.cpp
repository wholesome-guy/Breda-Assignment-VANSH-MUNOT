#include "Missile.h"
#include "GameEngine.h"

Missile::Missile(sf::Vector2f position, sf::Angle rotation, sf::Vector2f direction, float _Damage, float _Range) : Projectile(),
explosion_Sprite(explosion_Texture)
{
	init_Sprite(position, rotation);
	init_Variables(_Damage, _Range);

	//Finding velocity
	float magnitude = sqrtf((direction.x * direction.x) + (direction.y * direction.y));

	projectile_Velocity = { (direction.x / magnitude) * _Speed, (direction.y / magnitude) * _Speed };


}

void Missile::init_Variables(float damage, float range)
{
	//properties
	_Speed = 200.f;
	_Damage = damage;
	despawn_Time = range;
	despawn_Timer = 0.f;

	should_Despawn = false;
	is_Moving = true;
	//explosion
	explosion_Radius = 200;
	explosion_Timer = 0;
	explosion_Time = 2;

	_EnemySpawner = GameEngine::get_Instance()->get_EnemySpawner();

}

void Missile::init_Sprite(sf::Vector2f position, sf::Angle rotation)
{
	projectile_Texture = sf::Texture(sf::Image("C:/Users/vansh/CPP Games/Breda Assignment/Source/Repository/Breda Assignment VANSH MUNOT/Assets/Player/Missile_PNG.png"));
	projectile_Sprite.setTexture(projectile_Texture,true);
	projectile_Sprite.setScale({ 1,1 });
	projectile_Sprite.setOrigin({ static_cast<float>(projectile_Texture.getSize().x / 2),static_cast<float>(projectile_Texture.getSize().y / 2) });


	projectile_Sprite.setPosition(position);
	projectile_Sprite.setRotation(rotation);


	explosion_Texture = sf::Texture(sf::Image("C:/Users/vansh/CPP Games/Breda Assignment/Source/Repository/Breda Assignment VANSH MUNOT/Assets/Player/Explosion_PNG.png"));
	explosion_Sprite.setTexture(explosion_Texture,true);
	explosion_Sprite.setScale({ 5,5 });
	explosion_Sprite.setOrigin({ static_cast<float>(explosion_Texture.getSize().x / 2),static_cast<float>(explosion_Texture.getSize().y / 2)});

}
void Missile::update(float deltatime)
{
	//moving function
	if (is_Moving)
	{
		projectile_Sprite.move(projectile_Velocity * deltatime);
	}

	//despwan
	if (despawn_Timer < despawn_Time)
	{
		despawn_Timer += deltatime;
	}
	else
	{
		//explode on despawn if no collision occurs
		explosion();
	}

	collision();

	//explosion timer
	if (is_Exploding)
	{
		explosion_Timer += deltatime;
		if (explosion_Timer > explosion_Time)
		{
			end_Explosion = true;
		}
	}

}
void Missile::render(sf::RenderTarget& target)
{
	target.draw(projectile_Sprite);

	if (is_Exploding)
	{
		target.draw(explosion_Sprite);
	}
}

bool Missile::shouldDespawn() const
{
	return should_Despawn;
}

void Missile::collision()
{
	auto& enemies = _EnemySpawner->get_Enemies();

	for (auto& e : enemies)
	{
		if (projectile_Sprite.getGlobalBounds().findIntersection(e->get_GlobalBounds()))
		{
			explosion();
		}
	}

}

void Missile::explosion()
{
	if (can_Damage)
	{
		is_Moving = false;

		sf::Vector2f missile_Position = projectile_Sprite.getPosition();

		explosion_Sprite.setPosition(missile_Position);
		is_Exploding = true;

		auto& enemies = _EnemySpawner->get_Enemies();

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
		can_Damage = false;

		
	}
	if (end_Explosion)
	{
		should_Despawn = true;
		is_Exploding = false;
	}

}
