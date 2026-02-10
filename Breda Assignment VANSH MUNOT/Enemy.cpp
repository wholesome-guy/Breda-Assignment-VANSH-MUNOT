#include "Enemy.h"
#include "GameEngine.h"
#include <iostream>

Enemy::Enemy() :enemy_Sprite(enemy_Texture)
{
    init_Sprite();
    init_Variables();
}
void Enemy::init_Variables()
{
    enemy_Speed = 50.0f;
    _Player = GameEngine::get_Instance()->get_Player();
}
void Enemy::init_Sprite()
{
    enemy_Texture = sf::Texture(sf::Image("C:/Users/vansh/CPP Games/Breda Assignment/Source/Repository/Breda Assignment VANSH MUNOT/Assets/Player/Enemy_PNG.png"));

    //setting sprite properties
    enemy_Sprite.setTexture(enemy_Texture, true);
    enemy_Sprite.setPosition({ 100.f, 100.f });
    enemy_Sprite.setScale({ 1.f, 1.f });
    enemy_Sprite.setOrigin({ static_cast<float>(enemy_Texture.getSize().x / 2),static_cast<float>(enemy_Texture.getSize().y / 2) });
}

void Enemy::update(float deltatime)
{
    enemy_Movement(deltatime);
}

void Enemy::render(sf::RenderTarget& target)
{
	target.draw(enemy_Sprite);
}

sf::Vector2f Enemy::get_Position()
{
	return enemy_Sprite.getPosition();
}
sf::FloatRect Enemy::get_GlobalBounds()
{
    return enemy_Sprite.getGlobalBounds();
}
void Enemy::set_Position(sf::Vector2f position)
{
    enemy_Sprite.setPosition(position);
}

void Enemy::take_Damage(float _Damage)
{
	std::cout << "took damage " << _Damage << "\n";
}

void Enemy::enemy_Movement(float deltatime)
{
    sf::Vector2f player_Position = _Player->get_Position();
    sf::Vector2f enemy_Position = enemy_Sprite.getPosition();

    sf::Vector2f _Direction = player_Position - enemy_Position;

    float agnitude = std::sqrt(_Direction.x * _Direction.x + _Direction.y * _Direction.y);

    if (agnitude != 0)
    {
        _Direction = _Direction / agnitude; // normalize

    }
    enemy_Sprite.move(_Direction * enemy_Speed * deltatime);
}
void Enemy::set_Color(sf::Color colour)
{
    enemy_Sprite.setColor(colour);
}
