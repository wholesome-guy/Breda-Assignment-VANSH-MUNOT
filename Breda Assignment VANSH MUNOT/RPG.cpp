#include "RPG.h"
#include<iostream>
RPG::RPG() : Weapon()
{
    init_Variables();
    init_Sprite();
}

void RPG::init_Variables()
{
    //properties
    _Ammo = 4;
    _Damage = 100.f;
    //despawn time for missle
    _Range = 1.5f;
    cooldown_Timer = 2.f;

    if (!shoot.loadFromFile("Assets/Sound/Shoot_WAV.wav"))
        std::cout << "Failed to load shoot\n";

}

void RPG::init_Sprite()
{
    //sprrite
    weapon_Texture = sf::Texture(sf::Image("Assets/Player/RPG_PNG.png"));
    weapon_Sprite.setTexture(weapon_Texture, true);
    weapon_Sprite.setOrigin({15,13});
    weapon_Sprite.setPosition({ 100,100 });
}

void RPG::update(float deltatime)
{
    for (int i = 0; i < Missiles.size(); i++)
    {
        Missiles[i]->update(deltatime);

        if (Missiles[i]->shouldDespawn())
        {
            Missiles.erase(Missiles.begin() + i);
        }
    }
}

void RPG::render(sf::RenderTarget& target)
{
    target.draw(weapon_Sprite);
    for (auto& m : Missiles)
    {
        m->render(target);
    }
}

void RPG::Attack()
{
    sf::Vector2f Position = weapon_Sprite.getPosition();
    sf::Angle Rotation = weapon_Sprite.getRotation();
    //cos = horizontal component, sin = vertical component
    sf::Vector2f shoot_Direction(std::cos(Rotation.asRadians()), std::sin(Rotation.asRadians()));

    //make new missile
    Missiles.push_back(std::make_unique<Missile>(Position, Rotation, shoot_Direction, _Damage, _Range));

    sfx_Event.buffer = &shoot;
    sfx_Event.volume = 100;
    sfx_Event.pitch = 1;
    sfx_Event.randomise_pitch = true;
    notify_Observers(sfx_Event);
}

void RPG::weapon_Rotate(sf::RenderWindow& game_Window)
{
    //point at mouse
    sf::Vector2f weapon_Position = weapon_Sprite.getPosition();
    //screen space to world space
    sf::Vector2f mouse_Position = game_Window.mapPixelToCoords(sf::Mouse::getPosition());

    const float Radians_To_Degrees = 180 / 3.14f;

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

void RPG::weapon_Position(sf::Vector2f player_position)
{
    sf::Vector2f offset = { 0,10 };
    weapon_Sprite.setPosition(player_position + offset);
}
void RPG::weapon_Scale(sf::Vector2f _Scale)
{
    weapon_Sprite.setScale(_Scale);
}

