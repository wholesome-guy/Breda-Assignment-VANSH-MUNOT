#include "Player.h"
#include "GameEngine.h"
#include "Sword.h"
#include "Rifle.h"
#include "RPG.h"
#include <sstream>
#include <random>



//constructor unity start
Player::Player():
    //intialiser list for things with no default constructors
	player_Sprite(player_Texture)
{
    init_playerSprite();
    init_Variables();
    init_Weapons();

}

void Player::init_playerSprite()
{
    //loadind from memory - new texture which is an image
    player_Texture = sf::Texture(sf::Image("C:/Users/vansh/CPP Games/Breda Assignment/Source/Repository/Breda Assignment VANSH MUNOT/Assets/Player/Player_PNG.png"));
    
    //setting sprite properties
    player_Sprite.setTexture(player_Texture, true);
    player_Sprite.setPosition({ 100.f, 100.f });
    player_Sprite.setScale({ 1.f, 1.f });
    player_Sprite.setOrigin({ static_cast<float>(player_Texture.getSize().x / 2),static_cast<float>(player_Texture.getSize().y / 2) });
}
void Player::init_Variables()
{
    player_Speed = 250.0f;
    max_player_Health = 100;
    player_Health = max_player_Health;
    //singleton access
    game_Window = GameEngine::get_Instance()->get_Window();  

    sprite_Offset_X = static_cast<float>(player_Texture.getSize().x / 2);
    sprite_Offset_Y = static_cast<float>(player_Texture.getSize().y / 2);

    sf::Vector2u screen_Size = GameEngine::get_Instance()->get_Window_Size();
    screen_Width = static_cast<float>(screen_Size.x);
    screen_Height = static_cast<float>(screen_Size.y);

    invincibility_Time = 0.5f;
    invincibility_Timer = 0;

    weapon_Transform_Timer = 0;
    weapon_Transfrom_Time = 3.f;
}


void Player::init_Weapons()
{
    //initialising weapons
    _Sword = new Sword();  
    _Rifle = new Rifle();
    _RPG = new RPG();

    current_Weapon = _Sword;
    current_weapon_Ammo = _Sword->_Ammo;
    current_weapon_Cooldown = _Sword->cooldown_Timer;

}


void Player::update(float deltatime)
{
    
    player_Movement(deltatime);
    weapon_Movement();

    //weapon cooldown timer
    if (is_weapon_Cooldown)
    {
        weapon_Cooldown(deltatime);
    }

    current_Weapon->update(deltatime);

    //attack
    player_Attack();
    weapon_Transformation_Cooldown(deltatime);
    
    wall_Collision();
    enemy_Collision(deltatime);

}

void Player::render(sf::RenderTarget& target)
{
    target.draw(player_Sprite);

    //only current weapon is rendered
    current_Weapon->render(*game_Window);


}

//checks keyboard inputs and normalises the input vector
void Player::player_Movement(float deltatime)
{
    sf::Vector2f keyboard_Input = {0,0};
    sf::Vector2f player_Velocity;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    {
        keyboard_Input.x -= 1.0f;

    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    {
        keyboard_Input.x += 1.0f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
    {
        keyboard_Input.y -= 1.0f;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    {
        keyboard_Input.y += 1.0f;
    }

   
    
    //when two keys from different axis are pressed simultenously
    //calculate the magnitude and divide the input vector with its magnitude to get a unit vector
    
    if (keyboard_Input.x != 0 && keyboard_Input.y != 0)
    {
        float maginutude_Kinput = std::sqrtf(powf(keyboard_Input.x, 2) + powf(keyboard_Input.y, 2));
        player_Velocity = (keyboard_Input / maginutude_Kinput) * player_Speed * deltatime;
    }
    else
    {
        player_Velocity = keyboard_Input * player_Speed * deltatime;
    }
    //sprite move function
    player_Sprite.move(player_Velocity);
}

void Player::weapon_Movement()
{
    //rotates current weapon to always point at mouse
    current_Weapon->weapon_Rotate(*game_Window);

    //moves to player position
    current_Weapon->weapon_Position(player_Sprite.getPosition());

    float angle = current_Weapon->weapon_RotationAngle;

    if (angle >= 90.f && angle <= 270.f)
    {
        //  player face left when weapn faces left
        player_Sprite.setScale({ -1.f, 1.f });
    }
    else
    {
        // player face right when when faces right
        player_Sprite.setScale({ 1.f, 1.f });
    }

}
void Player::player_Attack()
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        if (can_Attack)
        {
            current_Weapon->Attack();
            current_weapon_Ammo--;

            if (current_weapon_Ammo <= 0)
            {
                //start Transformation
                is_weapon_Transforming = true;
            }
            is_weapon_Cooldown = true;

        }
    }
}
void Player::weapon_Cooldown(float deltatime)
{
    can_Attack = false;
    if (weapon_cooldown_Timer<current_weapon_Cooldown)
    {
        weapon_cooldown_Timer += deltatime;
    }
    else
    {
        weapon_cooldown_Timer = 0;
        can_Attack = true;
        is_weapon_Cooldown = false;
    }
}

void Player::transform_Weapon()
{
        //get seed
        std::random_device rd;
        //intialise  m twister
        std::mt19937 gen(rd());

        std::uniform_int_distribution<int> random_weapon(0, 2);

        int random_Number = random_weapon(gen);

        switch (random_Number)
        {
        case 0:
            weapon_Assigner(_Sword);
            break;

        case 1:
            weapon_Assigner(_Rifle);

            break;

        case 2:
            weapon_Assigner(_RPG);

            break;
        }
        current_Weapon->weapon_Position(player_Sprite.getPosition());
        can_Attack = true;
}

void Player::weapon_Transformation_Cooldown(float deltatime)
{
    if (is_weapon_Transforming)
    {
        can_Attack = false;

        weapon_Transform_Timer += deltatime;
        if (weapon_Transform_Timer > weapon_Transfrom_Time)
        {
            //complete tranformation
            is_weapon_Transforming = false;
            weapon_Transform_Timer = 0;
            transform_Weapon();
        }
    }
}

void Player::weapon_Assigner(Weapon* weapon)
{
    current_Weapon = weapon;
    current_weapon_Ammo = weapon->_Ammo;
    current_weapon_Cooldown = weapon->cooldown_Timer;
}

sf::Vector2f Player::get_Position()
{
    return player_Sprite.getPosition();
}

float Player::get_Cooldown(int i)
{
    switch (i)
    {
    case 0:
        return(current_weapon_Cooldown - weapon_cooldown_Timer);
        break;

    case 1:    
        return(weapon_Transfrom_Time - weapon_Transform_Timer);
        break;
    }
}

bool Player::get_CoolDown_Bool(int i)
{
    switch (i)
    {
    case 0:
        return is_weapon_Cooldown;
        break;

    case 1:
        return is_weapon_Transforming;
        break;
    }
}

float Player::get_Health()
{
    return player_Health;
}

int Player::get_Ammo()
{
    return current_weapon_Ammo;
}

void Player::wall_Collision()
{
    float position_X = player_Sprite.getPosition().x;
    float position_Y = player_Sprite.getPosition().y;


    //if player goes to the left, beyond the screen. X : left = negative, right = positive
    // Y: up = negative,down = positive
    //LEFT WALL COLLISION
    if (position_X - sprite_Offset_X < 0)
    {
        player_Sprite.setPosition({ 0 + sprite_Offset_X,position_Y });
    }
    //RIGHT WALL COLLISION
    else if (position_X + sprite_Offset_X > screen_Width)
    {
        player_Sprite.setPosition({ screen_Width - sprite_Offset_X,position_Y });
    }
    //TOP WALL COLLISION
    if (position_Y - sprite_Offset_Y < 0)
    {
        player_Sprite.setPosition({ position_X,0 +sprite_Offset_Y });
    }
    else if (position_Y + sprite_Offset_Y > screen_Height)
    {
        player_Sprite.setPosition({ position_X,screen_Height - sprite_Offset_Y });
    }
}

void Player::enemy_Collision(float deltatime)
{
    if (can_Damage)
    {
        auto& enemies = GameEngine::get_Instance()->get_Enemies();
        for (auto& e : enemies)
        {
            if (player_Sprite.getGlobalBounds().findIntersection(e->get_GlobalBounds()))
            {
                player_Health--;

                player_Health = std::clamp(player_Health, 0.f, max_player_Health);

                if (player_Health <= 0)
                {
                    //death
                }
                can_Damage = false;
            }
        }
    }
    else
    {
        invincibility_Timer += deltatime;

        if (invincibility_Timer > invincibility_Time)
        {
            can_Damage = true;

            player_Sprite.setColor(sf::Color::White);
            current_Weapon->weapon_Colour(sf::Color::White);

            invincibility_Timer = 0;
        }
        else
        {
            player_Sprite.setColor(sf::Color::Color(255, 255, 255, 55));
            current_Weapon->weapon_Colour(sf::Color::Color(255, 255, 255, 55));
        }
    }   

}





