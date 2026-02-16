#include "Enemy.h"
#include "GameEngine.h"
#include <iostream>
#include <random>
//full declare in cpp file to avoid circualar dependency
#include "Player.h" 

Enemy::Enemy() :enemy_Sprite(enemy_Texture)
{
    random_Enemy_Type();
    init_Variables();
    init_Sprite();

}
#pragma region Intialise

void Enemy::init_Variables()
{
    //random type selected for two category of enemies
    switch (type)
    {
    case 0:
        enemy_Speed = 150.0f;
        max_enemy_Health = 100.f;
        enemy_Damage = 10;
        break;
    case 1:
        enemy_Speed = 75.0f;
        max_enemy_Health = 200.f;
        enemy_Damage = 30;
        break;
    }
    enemy_Health = max_enemy_Health;

    _Player = GameEngine::get_Instance()->get_Player();

    flash_Time = 0.2f;
    flash_Timer = 0;
}
void Enemy::init_Sprite()
{
    //sprite properties
    switch (type)
    {
    case 0:
        enemy_Texture = sf::Texture(sf::Image("C:/Users/vansh/CPP Games/Breda Assignment/Source/Repository/Breda Assignment VANSH MUNOT/Assets/Player/Enemy_0_PNG.png"));

        break;
    case 1:
        enemy_Texture = sf::Texture(sf::Image("C:/Users/vansh/CPP Games/Breda Assignment/Source/Repository/Breda Assignment VANSH MUNOT/Assets/Player/Enemy_1_PNG.png"));

        break;
    }
    //setting sprite properties
    enemy_Sprite.setTexture(enemy_Texture, true);
    enemy_Sprite.setPosition({ 100.f, 100.f });
    enemy_Sprite.setScale({ 0.5f, 0.5f });
    enemy_Sprite.setOrigin({ static_cast<float>(enemy_Texture.getSize().x / 2),static_cast<float>(enemy_Texture.getSize().y / 2) });

    //health bar sprite
    health_Bar.setFillColor(sf::Color::Red);

    background_Bar.setFillColor(sf::Color::Color(255,255,255,150));
    background_Bar.setSize({ max_enemy_Health * 0.5f,5 });

}

void Enemy::random_Enemy_Type()
{
    //randomiser type
    std::random_device rd;

    std::mt19937 gen(rd());

    std::uniform_int_distribution<int> random_Type(0, 100);

    int probability = random_Type(gen);
    if (probability < 70)
    {
        type = 0;
    }
    else
    {
        type = 1;
    }

}

#pragma endregion

#pragma region Core Loops

void Enemy::update(float deltatime)
{
    //player to enemy direction
    direction_Calculate();

    //movement
    enemy_Movement(deltatime);

    //flash on damage
    enemy_Flashing(deltatime);

    //health bar
    health_Bar_Update();
}

void Enemy::render(sf::RenderTarget& target)
{
	target.draw(enemy_Sprite);
    target.draw(background_Bar);
    target.draw(health_Bar);
}
#pragma endregion

#pragma region Movement
void Enemy::direction_Calculate()
{
    //player to enemy, towards player direction calculator
    sf::Vector2f player_Position = _Player->get_Position();
    sf::Vector2f enemy_Position = enemy_Sprite.getPosition();

    player_enemy_Direction = player_Position - enemy_Position;

    float _magnitude = std::sqrt(player_enemy_Direction.x * player_enemy_Direction.x + player_enemy_Direction.y * player_enemy_Direction.y);

    if (_magnitude != 0)
    {
        player_enemy_Direction = player_enemy_Direction / _magnitude; // normalize
    }
}

void Enemy::enemy_Movement(float deltatime)
{
    enemy_Sprite.move(player_enemy_Direction * enemy_Speed * deltatime);

    //flip to look at player
    if (player_enemy_Direction.x < 0)
    {
        enemy_Sprite.setScale({ -1, 1 });
    }
    else
    {
        enemy_Sprite.setScale({ 1, 1 });
    }

    //knockback
    enemy_Sprite.move(knockback_Velocity * deltatime);
    knockback_Velocity *= knockback_Decay;
}
#pragma endregion

#pragma region Health
void Enemy::take_Damage(float _Damage)
{
    //damage
    enemy_Health -= _Damage;
    enemy_Health = std::clamp(enemy_Health, 0.f, max_enemy_Health);

    //knockback
    knockback_Velocity = -1.f * player_enemy_Direction * knockback_Force;

    //flash
    set_Color(sf::Color::Red);
    start_Flash = true;

    if (enemy_Health <= 0)
    {
        is_Dead = true;
    }
}
void Enemy::enemy_Flashing(float deltatime)
{
    if (start_Flash)
    {
        flash_Timer += deltatime;
        if (flash_Timer > flash_Time)
        {
            start_Flash = false;
            set_Color(sf::Color::White);
            flash_Timer = 0;
        }
    }
}
void Enemy::health_Bar_Update()
{
    sf::Vector2f offset;

    switch (type)
    {
    case 0:
        offset = { -25,-30 };
        break;
    case 1:
        offset = { -50,-30 };
        break;
    }    

    health_Bar.setPosition(enemy_Sprite.getPosition() + offset);
    background_Bar.setPosition(enemy_Sprite.getPosition() + offset);

    health_Bar.setSize({ enemy_Health * 0.5f,5 });
}
#pragma endregion

#pragma region Setters and Getter
void Enemy::set_Color(sf::Color colour)
{
    enemy_Sprite.setColor(colour);
}
void Enemy::set_Position(sf::Vector2f position)
{
    enemy_Sprite.setPosition(position);
}

sf::Vector2f Enemy::get_Position()
{
    return enemy_Sprite.getPosition();
}
sf::FloatRect Enemy::get_GlobalBounds()
{
    return enemy_Sprite.getGlobalBounds();
}
bool Enemy::get_Dead_Bool()
{
    return is_Dead;
}
float Enemy::get_Damage()
{
    return enemy_Damage;
}
#pragma endregion
