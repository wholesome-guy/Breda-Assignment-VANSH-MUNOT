#include "Sword.h"
#include <iostream>
#include "GameEngine.h"
#include "Enemy.h"

Sword::Sword() : Weapon()
{
    init_Variables();
    init_Sprite();
}

void Sword::init_Variables()
{
    _Ammo = 6;
    _Damage = 50.f;
    _Range = 100.f;
    cooldown_Timer = 0.1f;
}

void Sword::init_Sprite()
{
    weapon_Texture = sf::Texture(sf::Image("C:/Users/vansh/CPP Games/Breda Assignment/Source/Repository/Breda Assignment VANSH MUNOT/Assets/Player/Sword_PNG.png"));
    weapon_Sprite.setTexture(weapon_Texture,true);
    weapon_Sprite.setOrigin({10,15});
    weapon_Sprite.setPosition({ 100,100 });
}

void Sword::update(float deltatime)
{
}

void Sword::render(sf::RenderTarget& target)
{
    target.draw(weapon_Sprite);
}

void Sword::Attack()
{
    std::cout << "sword attack" << "\n";

    /*sf::Vector2f swordPos = weapon_Sprite.getPosition();

    Enemy* enemy = GameEngine::get_Instance()->get_Enemy();

        sf::Vector2f enemyPos = enemy->get_Position();
        sf::Vector2f diff = enemyPos - swordPos;

        float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);
        if (dist < _Range)
        {

            float angleToEnemy = atan2(diff.y, diff.x) * 180.f / 3.14159f;

            if (angleToEnemy < 0)
            {
                angleToEnemy += 360.f;
            }
            float angleDiff = std::abs(angleToEnemy - weapon_RotationAngle);

            if (angleDiff < 45.f) // 90° cone
            {           
                enemy->take_Damage(_Damage);
            }
        }*/

}

void Sword::weapon_Rotate(sf::RenderWindow& game_Window)
{
    sf::Vector2f weapon_Position = weapon_Sprite.getPosition();
    sf::Vector2f mouse_Position = game_Window.mapPixelToCoords(sf::Mouse::getPosition());

    const float Radians_To_Degrees = 180 / 3.14f;

    float dy_Weapon_Mouse = mouse_Position.y - weapon_Position.y;
    float dx_Weapon_Mouse = mouse_Position.x - weapon_Position.x;

    
    weapon_RotationAngle = (atan2(dy_Weapon_Mouse, dx_Weapon_Mouse) * Radians_To_Degrees);
    if (weapon_RotationAngle < 0)
    {
        weapon_RotationAngle += 360;
    }

    sf::Angle rotation_Angle = sf::degrees(weapon_RotationAngle);
    weapon_Sprite.setRotation(rotation_Angle);
}

void Sword::weapon_Position(sf::Vector2f player_position)
{
    sf::Vector2f offset = { 0,10 };
    weapon_Sprite.setPosition(player_position + offset);
}
void Sword::weapon_Scale(sf::Vector2f _Scale)
{
    weapon_Sprite.setScale(_Scale);
}

