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

    sf::Vector2f sword_Position = weapon_Sprite.getPosition();

    auto& enemies = GameEngine::get_Instance()->get_Enemies();

    for (auto& e : enemies)
    {
        sf::Vector2f enemy_Position = e->get_Position();
        sf::Vector2f difference = enemy_Position - sword_Position;

        float distance = std::sqrt(difference.x * difference.x + difference.y * difference.y);
        if (distance < _Range)
        {

            float angle_To_Enemy = atan2(difference.y, difference.x) * Radians_To_Degrees;

            if (angle_To_Enemy < 0)
            {
                angle_To_Enemy += 360.f;
            }
            float angle_Difference = std::abs(angle_To_Enemy - weapon_RotationAngle);

            if (angle_Difference < 45.f) // 90° cone
            {
                e->take_Damage(_Damage);
            }
        }
    }
        

}

void Sword::weapon_Rotate(sf::RenderWindow& game_Window)
{
    sf::Vector2f weapon_Position = weapon_Sprite.getPosition();
    sf::Vector2f mouse_Position = game_Window.mapPixelToCoords(sf::Mouse::getPosition());

    

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

