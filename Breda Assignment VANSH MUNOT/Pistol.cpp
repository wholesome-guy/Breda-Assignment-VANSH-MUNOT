#include "Pistol.h"
#include<iostream>

Pistol::Pistol() : Weapon()
{
    init_Variables();
    init_Sprite();
}

void Pistol::init_Variables()
{
    //properties
    _Ammo = 10;
    _Damage = 200.f;
    //despawn time for bullet
    _Range = 4.f;
    cooldown_Timer = 0.5f;

    if (!shoot.loadFromFile("Assets/Sound/Shoot_WAV.wav"))
        std::cout << "Failed to load shoot\n";

}

void Pistol::init_Sprite()
{
    //sprite
    weapon_Texture = sf::Texture(sf::Image("Assets/Player/Pistol_PNG.png"));
    weapon_Sprite.setTexture(weapon_Texture, true);
    weapon_Sprite.setOrigin({ 10,17 });
    weapon_Sprite.setPosition({ 100,100 });
}

void Pistol::update(float deltatime)
{
    for (int i = 0; i < (int)Bullets.size(); i++)
    {
        Bullets[i]->update(deltatime);

        if (Bullets[i]->shouldDespawn())
        {
            Bullets.erase(Bullets.begin() + i);
        }
    }
}

void Pistol::render(sf::RenderTarget& target)
{
    target.draw(weapon_Sprite);

    for (auto& b : Bullets)
    {
        b->render(target);
    }
}

void Pistol::Attack()
{
    sf::Vector2f Position = weapon_Sprite.getPosition();
    sf::Angle Rotation = weapon_Sprite.getRotation();
    //cos = horizontal component, sin = vertical component
    sf::Vector2f shoot_Direction(std::cos(Rotation.asRadians()), std::sin(Rotation.asRadians()));

    //make new bullet
    Bullets.push_back(std::make_unique<Bullet>(Position, Rotation, shoot_Direction, _Damage, _Range));

    sfx_Event.buffer = &shoot;
    sfx_Event.volume = 100;
    sfx_Event.pitch = 1;
    sfx_Event.randomise_pitch = true;
    notify_Observers(sfx_Event);
}

void Pistol::weapon_Rotate(sf::RenderWindow& game_Window)
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
        weapon_Scale({ 2,-2 });
    }
    else
    {
        // player face right
        weapon_Scale({ 2,2 });

    }
}

void Pistol::weapon_Position(sf::Vector2f player_position)
{
    sf::Vector2f offset = { 0,10 };
    weapon_Sprite.setPosition(player_position + offset);
}

void Pistol::weapon_Scale(sf::Vector2f _Scale)
{
    weapon_Sprite.setScale(_Scale);
}
