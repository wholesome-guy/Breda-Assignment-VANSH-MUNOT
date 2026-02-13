#include "Bullet.h"
#include "GameEngine.h"
Bullet::Bullet(sf::Vector2f position, sf::Angle rotation, sf::Vector2f direction,float _Damage) : Projectile()
{
	init_Sprite(position, rotation);
	init_Variables(_Damage);

	float magnitude = sqrtf((direction.x * direction.x) + (direction.y * direction.y));

	projectile_Velocity = { (direction.x / magnitude) * _Speed, (direction.y / magnitude) * _Speed };


}

void Bullet::init_Variables(float damage)
{
	_Speed = 500.f;
	_Damage = damage;
	despawn_Time = 3.f;
	despawn_Timer = 0.f;
	should_Despawn = false;
}

void Bullet::init_Sprite(sf::Vector2f position, sf::Angle rotation)
{
	projectile_Texture = sf::Texture(sf::Image("C:/Users/vansh/CPP Games/Breda Assignment/Source/Repository/Breda Assignment VANSH MUNOT/Assets/Player/Sword_PNG.png"));
	projectile_Sprite = sf::Sprite(projectile_Texture);
	projectile_Sprite.setTexture(projectile_Texture);
	projectile_Sprite.setScale({ 1,1 });
	projectile_Sprite.setOrigin({ static_cast<float>(projectile_Texture.getSize().x / 2),static_cast<float>(projectile_Texture.getSize().y / 2) });


	projectile_Sprite.setPosition(position);

	projectile_Sprite.setRotation(rotation);
}
void Bullet::update(float deltatime)
{
	projectile_Sprite.move(projectile_Velocity * deltatime);


	if (despawn_Timer < despawn_Time)
	{
		despawn_Timer += deltatime;
	}
	else
	{
		should_Despawn = true;
	}

	collision();

}
void Bullet::render(sf::RenderTarget& target)
{
	target.draw(projectile_Sprite);
}

bool Bullet::shouldDespawn() const
{
	return should_Despawn;
}

void Bullet::collision()
{
	auto& enemies = GameEngine::get_Instance()->get_Enemies();

	for (auto& e : enemies)
	{
		if (projectile_Sprite.getGlobalBounds().findIntersection(e->get_GlobalBounds()))
		{
			should_Despawn = true;
			e->take_Damage(_Damage);
		}
	}

}
