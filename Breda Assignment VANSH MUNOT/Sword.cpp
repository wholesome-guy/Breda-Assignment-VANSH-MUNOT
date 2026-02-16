#include "Sword.h"
#include <iostream>
#include "GameEngine.h"
#include "Enemy.h"
#include <cmath>
#include "MathFunctions.h"

Sword::Sword() : Weapon()
{
    init_Variables();
    init_Sprite();
}

void Sword::init_Variables()
{
    _Ammo = 15;
    _Damage = 25.f;
    _Range = 150.f;
    cooldown_Timer = 0.5f;

    is_Swinging = false;
    swing_Timer = 0.f;
    swing_Duration = 0.25f;

    _EnemySpawner = GameEngine::get_Instance()->get_EnemySpawner();

}

void Sword::init_Sprite()
{
    weapon_Texture = sf::Texture(sf::Image("C:/Users/vansh/CPP Games/Breda Assignment/Source/Repository/Breda Assignment VANSH MUNOT/Assets/Player/Sword_PNG.png"));
    weapon_Sprite.setTexture(weapon_Texture,true);
    weapon_Sprite.setOrigin({10,15});
    weapon_Sprite.setPosition({ 100,100 });
    weapon_Scale({3.5f,1.f});
}

void Sword::attack_Animation_Intialiser()
{
    is_Swinging = true;

    swing_Timer = 0.f;
    swing_Start_Angle = weapon_RotationAngle - 60.f;
    swing_End_Angle = weapon_RotationAngle + 30.f;

    weapon_Sprite.setRotation(sf::degrees(swing_Start_Angle));
}

void Sword::attack_Animation_Update(float deltatime)
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
            float currentAngle = MathFunctions::Quint_Lerp(swing_Start_Angle,swing_End_Angle,t);
            weapon_Sprite.setRotation(sf::degrees(currentAngle));

            float current_scale = MathFunctions::Lerp(2.5,3, t);
            weapon_Scale({current_scale,current_scale});
        }
        else
        {
            //swinging complete
            is_Swinging = false;
            swing_Timer = 0.f;
            weapon_Scale({ 1,1 });

        }
    }
}

void Sword::attack_Enemy_Collision()
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
            if (angle_Difference < 45.f) // 90 cone
            {
                e->take_Damage(_Damage);
            }
        }
    }
}

void Sword::update(float deltatime)
{
    attack_Animation_Update(deltatime);
}

void Sword::render(sf::RenderTarget& target)
{
    target.draw(weapon_Sprite);
}

void Sword::Attack()
{
    std::cout << "sword attack" << "\n";

    attack_Animation_Intialiser();

    attack_Enemy_Collision();

    
}

void Sword::weapon_Rotate(sf::RenderWindow& game_Window)
{
    if (is_Swinging) 
    {
        return;
    }

    sf::Vector2f weapon_Position = weapon_Sprite.getPosition();
    sf::Vector2f mouse_Position = game_Window.mapPixelToCoords(sf::Mouse::getPosition());

    float dy_Weaponouse = mouse_Position.y - weapon_Position.y;
    float dx_Weaponouse = mouse_Position.x - weapon_Position.x;

    weapon_RotationAngle = (atan2(dy_Weaponouse, dx_Weaponouse) * Radians_To_Degrees);
    if (weapon_RotationAngle < 0)
    {
        weapon_RotationAngle += 360;
    }

    sf::Angle rotation_Angle = sf::degrees(weapon_RotationAngle);
    weapon_Sprite.setRotation(rotation_Angle);

    if (weapon_RotationAngle >= 90.f && weapon_RotationAngle <= 270.f)
    {
        //  player face left
        weapon_Scale({ 1,-1 });
    }
    else
    {
        // player face right
        weapon_Scale({ 1,1 });

    }
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


