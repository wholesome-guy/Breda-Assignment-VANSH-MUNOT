#include "Rifle.h"
#include<iostream>

Rifle::Rifle() : Weapon()
{
    init_Variables();
    init_Sprite();
}

void Rifle::init_Variables()
{
    //properties
    _Ammo = 30;
    _Damage = 50.f;
    //despawn time for bullet
    _Range = 4.f;
    cooldown_Timer = 0.1f;

}

void Rifle::init_Sprite()
{
    //sprite
    weapon_Texture = sf::Texture(sf::Image("Assets/Player/Rifle_PNG.png"));
    weapon_Sprite.setTexture(weapon_Texture, true);
    weapon_Sprite.setOrigin({ 9,9 });
    weapon_Sprite.setPosition({ 100,100 });
}

void Rifle::update(float deltatime)
{
    for (int i = 0; i < Bullets.size(); i++)
    {
        Bullets[i]->update(deltatime);

        if (Bullets[i]->shouldDespawn())
        {
            Bullets.erase(Bullets.begin() + i);
        }
    }
}

void Rifle::render(sf::RenderTarget& target)
{
    target.draw(weapon_Sprite);

    for (auto& b : Bullets)
    {
        b->render(target);
    }
}

void Rifle::Attack()
{
    sf::Vector2f Position = weapon_Sprite.getPosition();
    sf::Angle Rotation = weapon_Sprite.getRotation();
    //cos = horizontal component, sin = vertical component
    sf::Vector2f shoot_Direction(std::cos(Rotation.asRadians()), std::sin(Rotation.asRadians()));

    //make new bullet
    Bullets.push_back(std::make_unique<Bullet>(Position, Rotation, shoot_Direction,_Damage,_Range));

}

void Rifle::weapon_Rotate(sf::RenderWindow& game_Window)
{
    //rotate to alwasy point at mouse
    sf::Vector2f weapon_Position = weapon_Sprite.getPosition();
    sf::Vector2f mouse_Position = game_Window.mapPixelToCoords(sf::Mouse::getPosition());

    Radians_To_Degrees = 180 / 3.14f;

    float dy_Weaponouse = mouse_Position.y - weapon_Position.y;
    float dx_Weaponouse = mouse_Position.x - weapon_Position.x;

    weapon_RotationAngle = (atan2(dy_Weaponouse, dx_Weaponouse) * Radians_To_Degrees);
    if (weapon_RotationAngle < 0)
    {
        weapon_RotationAngle += 360;
    }

    sf::Angle rotation_Angle = sf::degrees(weapon_RotationAngle);    
    weapon_Sprite.setRotation(rotation_Angle);

    //flip weapon to make it turn
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

void Rifle::weapon_Position(sf::Vector2f player_position)
{
    sf::Vector2f offset = { 0,10 };
    weapon_Sprite.setPosition(player_position + offset);
}

void Rifle::weapon_Scale(sf::Vector2f _Scale)
{
    weapon_Sprite.setScale(_Scale);
}

