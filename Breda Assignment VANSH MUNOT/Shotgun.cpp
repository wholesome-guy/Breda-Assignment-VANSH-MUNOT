#include "Shotgun.h"


Shotgun::Shotgun() : Weapon()
{
    init_Variables();
    init_Sprite();
}

void Shotgun::init_Variables()
{
    //properties
    _Ammo = 15;
    _Damage = 80.f;
    //despawn time for bullet
    _Range = 1.f;
    cooldown_Timer = 1.5f;


     number_Pellets = 10;
     spread_Angle = 45.f;
     step = spread_Angle / (number_Pellets - 1);
     start_Angle = -spread_Angle / 2.f;

}

void Shotgun::init_Sprite()
{
    //sprite
    weapon_Texture = sf::Texture(sf::Image("C:/Users/vansh/CPP Games/Breda Assignment/Source/Repository/Breda Assignment VANSH MUNOT/Assets/Player/Shotgun_PNG.png"));
    weapon_Sprite.setTexture(weapon_Texture, true);
    weapon_Sprite.setOrigin({ 4,8 });
    weapon_Sprite.setPosition({ 100,100 });
}

void Shotgun::update(float deltatime)
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

void Shotgun::render(sf::RenderTarget& target)
{
    target.draw(weapon_Sprite);

    for (auto& b : Bullets)
    {
        b->render(target);
    }
}

void Shotgun::Attack()
{
    sf::Vector2f Position = weapon_Sprite.getPosition();
    sf::Angle Rotation = weapon_Sprite.getRotation();
      

    for (int i = 0; i < number_Pellets; i++)
    {
        sf::Angle pellet_Rotation = Rotation + sf::degrees(start_Angle + step * i);
        sf::Vector2f shoot_Direction(
            std::cos(pellet_Rotation.asRadians()),
            std::sin(pellet_Rotation.asRadians())
        );
        Bullets.push_back(std::make_unique<Bullet>(Position, pellet_Rotation, shoot_Direction, _Damage, _Range));
    }
}

void Shotgun::weapon_Rotate(sf::RenderWindow& game_Window)
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

void Shotgun::weapon_Position(sf::Vector2f player_position)
{
    sf::Vector2f offset = { 0,10 };
    weapon_Sprite.setPosition(player_position + offset);
}

void Shotgun::weapon_Scale(sf::Vector2f _Scale)
{
    weapon_Sprite.setScale(_Scale);
}



