#include "WarAxe.h"
#include <iostream>
#include "GameEngine.h"
#include "Enemy.h"
#include <cmath>
#include "MathFunctions.h"
WarAxe::WarAxe() : Weapon()
{
    init_Variables();
    init_Sprite();
}

void WarAxe::init_Variables()
{
    //properties
    _Ammo = 10;
    _Damage = 60.f;
    //actual distance in pixels
    _Range = 250.f;
    cooldown_Timer = 1.0f;

    is_Swinging = false;
    swing_Timer = 0.f;
    swing_Duration = 0.25f;

    _EnemySpawner = GameEngine::get_Instance()->get_EnemySpawner();

}

void WarAxe::init_Sprite()
{
    weapon_Texture = sf::Texture(sf::Image("C:/Users/vansh/CPP Games/Breda Assignment/Source/Repository/Breda Assignment VANSH MUNOT/Assets/Player/War_Axe_PNG.png"));
    weapon_Sprite.setTexture(weapon_Texture, true);
    weapon_Sprite.setOrigin({ 197,295 });
    weapon_Sprite.setPosition({ 100,100 });
    weapon_Scale({ 0.08f,0.08f });
}

void WarAxe::attack_Animation_Intialiser()
{
    is_Swinging = true;

    swing_Timer = 0.f;
    // swing is larger than 120 for looks, looks better
    swing_Start_Angle = weapon_RotationAngle - 180.f;
    swing_End_Angle = weapon_RotationAngle + 180.f;

    weapon_Sprite.setRotation(sf::degrees(swing_Start_Angle));
}

void WarAxe::attack_Animation_Update(float deltatime)
{
    if (is_Swinging)
    {
        swing_Timer += deltatime;
        float t = swing_Timer / swing_Duration;

        if (t < 1.f)
        {
            //lerp,add the start angle t make sure when t=0, its at the start position
            //swing end- swing start multipled by t 
            ///float currentAngle = swing_Start_Angle + (swing_End_Angle - swing_Start_Angle) * t;
            float currentAngle = MathFunctions::Quint_Lerp(swing_Start_Angle, swing_End_Angle, t);
            weapon_Sprite.setRotation(sf::degrees(currentAngle));

            float current_scale = MathFunctions::Lerp(0.2, 0.24, t);
            weapon_Scale({ current_scale,current_scale });
        }
        else
        {
            //swinging complete
            is_Swinging = false;
            swing_Timer = 0.f;
            weapon_Scale({ 0.08f,0.08f });

        }
    }
}

void WarAxe::attack_Enemy_Collision()
{
    sf::Vector2f sword_Position = weapon_Sprite.getPosition();

    auto& enemies = _EnemySpawner->get_Enemies();

    for (auto& e : enemies)
    {
        sf::Vector2f enemy_Position = e->get_Position();
        sf::Vector2f difference = enemy_Position - sword_Position;

        //distance between sowrd and enemy
        float distance = std::sqrtf(difference.x * difference.x + difference.y * difference.y);

        if (distance < _Range)
        {
            float angle_To_Enemy = atan2(difference.y, difference.x) * Radians_To_Degrees;

            //converting to 0 tp 360
            if (angle_To_Enemy < 0)
            {
                angle_To_Enemy += 360.f;
            }

            float angle_Difference = std::abs(angle_To_Enemy - weapon_RotationAngle);

            //check both sides of the vector, 45 + 45 = 90
            if (angle_Difference < 180.f) // 360 cone
            {
                e->take_Damage(_Damage);
            }
        }
    }
}

void WarAxe::update(float deltatime)
{
    attack_Animation_Update(deltatime);
}

void WarAxe::render(sf::RenderTarget& target)
{
    target.draw(weapon_Sprite);
}

void WarAxe::Attack()
{
    attack_Animation_Intialiser();

    attack_Enemy_Collision();

}

void WarAxe::weapon_Rotate(sf::RenderWindow& game_Window)
{
    if (is_Swinging)
    {
        return;
    }

    sf::Vector2f weapon_Position = weapon_Sprite.getPosition();
    sf::Vector2f mouse_Position = game_Window.mapPixelToCoords(sf::Mouse::getPosition());

    float dy_Weaponouse = mouse_Position.y - weapon_Position.y;
    float dx_Weaponouse = mouse_Position.x - weapon_Position.x;

    weapon_RotationAngle = ((atan2(dy_Weaponouse, dx_Weaponouse)) * Radians_To_Degrees);
    if (weapon_RotationAngle < 0)
    {
        weapon_RotationAngle += 360;
    }

    sf::Angle rotation_Angle = sf::degrees(weapon_RotationAngle);
    weapon_Sprite.setRotation(rotation_Angle);

    if (weapon_RotationAngle >= 90.f && weapon_RotationAngle <= 270.f)
    {
        //  player face left
        weapon_Scale({ 0.08f,-0.08f });
    }
    else
    {
        // player face right
        weapon_Scale({ 0.08f,-0.08f });

    }

}

void WarAxe::weapon_Position(sf::Vector2f player_position)
{
    sf::Vector2f offset = { 0,10 };
    weapon_Sprite.setPosition(player_position + offset);
}
void WarAxe::weapon_Scale(sf::Vector2f _Scale)
{
    weapon_Sprite.setScale(_Scale);
}
