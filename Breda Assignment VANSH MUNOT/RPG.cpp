#include "RPG.h"
#include <iostream>
RPG::RPG() : Weapon()
{
    init_Variables();
    init_Sprite();
}

void RPG::init_Variables()
{
    _Ammo = 2;
    _Damage = 25.f;
    _Range = 60.f;
    cooldown_Timer = 2.f;

}

void RPG::init_Sprite()
{
    weapon_Texture = sf::Texture(sf::Image("C:/Users/vansh/CPP Games/Breda Assignment/Source/Repository/Breda Assignment VANSH MUNOT/Assets/Player/RPG_PNG.png"));
    weapon_Sprite.setTexture(weapon_Texture, true);
    weapon_Sprite.setOrigin({15,13});
    weapon_Sprite.setPosition({ 100,100 });
}

void RPG::update(float deltatime)
{
}

void RPG::render(sf::RenderTarget& target)
{
    target.draw(weapon_Sprite);
}

void RPG::Attack()
{
    std::cout << "RPG attack" << std::endl;
}

void RPG::weapon_Rotate(sf::RenderWindow& game_Window)
{
    sf::Vector2f weapon_Position = weapon_Sprite.getPosition();
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

