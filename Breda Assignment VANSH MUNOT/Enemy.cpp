#include "Enemy.h"
#include "GameEngine.h"
#include <iostream>

Enemy::Enemy() :enemy_Sprite_m(enemy_Texture_m)
{
    init_Sprite();
    init_Variables();
}
void Enemy::init_Variables()
{
    enemy_Speed_m = 50.0f;
    _Player_m = GameEngine::get_Instance()->get_Player();
}
void Enemy::init_Sprite()
{
    enemy_Texture_m = sf::Texture(sf::Image("C:/Users/vansh/CPP Games/Breda Assignment/Source/Repository/Breda Assignment VANSH MUNOT/Assets/Player/Enemy_PNG.png"));

    //setting sprite properties
    enemy_Sprite_m.setTexture(enemy_Texture_m, true);
    enemy_Sprite_m.setPosition({ 100.f, 100.f });
    enemy_Sprite_m.setScale({ 1.f, 1.f });
    enemy_Sprite_m.setOrigin({ static_cast<float>(enemy_Texture_m.getSize().x / 2),static_cast<float>(enemy_Texture_m.getSize().y / 2) });
}

void Enemy::update(float deltatime)
{
    enemy_Movement(deltatime);
}

void Enemy::render(sf::RenderTarget& target)
{
	target.draw(enemy_Sprite_m);
}

sf::Vector2f Enemy::get_Position()
{
	return enemy_Sprite_m.getPosition();
}

void Enemy::take_Damage(float _Damage)
{
	std::cout << "took damage " << _Damage << "\n";
}

void Enemy::enemy_Movement(float deltatime)
{
    sf::Vector2f player_Position = _Player_m->get_Position();
    sf::Vector2f enemy_Position = enemy_Sprite_m.getPosition();

    sf::Vector2f _Direction = player_Position - enemy_Position;

    float _Magnitude = std::sqrt(_Direction.x * _Direction.x + _Direction.y * _Direction.y);

    if (_Magnitude != 0)
    {
        _Direction = _Direction / _Magnitude; // normalize

    }
    enemy_Sprite_m.move(_Direction * enemy_Speed_m * deltatime);
}
