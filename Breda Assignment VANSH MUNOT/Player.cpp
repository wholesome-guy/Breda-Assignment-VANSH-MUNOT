#include "Player.h"
#include "GameEngine.h"
#include "Sword.h"
#include "Rifle.h"
#include "RPG.h"
#include "Mace.h"
#include "WarAxe.h"
#include "LMG.h"
#include "Pistol.h"
#include "Shotgun.h"
#include "GrenadeLauncher.h"
#include <sstream>

#include "EnemySpawner.h"
//constructor unity start
Player::Player():
    //intialiser list for things with no default constructors
	player_Sprite(player_Texture),rng(rd())
{
    init_Character();
    init_Variables();
    init_playerSprite();
    init_Weapons();

}

#pragma region Intialise Variables

void Player::init_playerSprite()
{

    //setting sprite properties
    player_Sprite.setTexture(player_Texture, true);
    player_Sprite.setPosition({ screen_Width/2, screen_Height/2 });
    player_Sprite.setScale({ 1.0f, 1.0f });
    player_Sprite.setOrigin({ static_cast<float>(player_Texture.getSize().x / 2),static_cast<float>(player_Texture.getSize().y / 2) });
}
void Player::init_Variables()
{
    current_player_Health = max_player_Health;

    //singleton access
    game_Window = GameEngine::get_Instance()->get_Window();  

    //Sprite Offset
    sprite_Offset_X = static_cast<float>(player_Texture.getSize().x / 2);
    sprite_Offset_Y = static_cast<float>(player_Texture.getSize().y / 2);

    //Screen Size
    sf::Vector2u screen_Size = game_Window->getSize();
    screen_Width = static_cast<float>(screen_Size.x);
    screen_Height = static_cast<float>(screen_Size.y);

    _EnemySpawner = GameEngine::get_Instance()->get_EnemySpawner();

    //Timers
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
    _Mace = new Mace();
    _WarAxe = new WarAxe();
    _LMG = new LMG();
    _Pistol = new Pistol();
    _Shotgun = new Shotgun();
    _Grenade_Launcher = new GrenadeLauncher();

    //first weapon on start
    current_Weapon = _Sword;
    current_weapon_Ammo = _Sword->_Ammo;
    current_weapon_Cooldown = _Sword->cooldown_Timer;

    
}

void Player::init_Character()
{
    Penguin.character_Speed = 250.f;
    Penguin.max_character_Health = 150;
    Penguin.character_Terraforming_Factor = 2;
    Penguin.character_Texture = sf::Texture(sf::Image("C:/Users/vansh/CPP Games/Breda Assignment/Source/Repository/Breda Assignment VANSH MUNOT/Assets/Player/Penguin_PNG.png"));
    Penguin.character_Index = 0;

    Crocodile.character_Speed = 150.f;
    Crocodile.max_character_Health = 300;
    Crocodile.character_Terraforming_Factor = 1;
    Crocodile.character_Texture = sf::Texture(sf::Image("C:/Users/vansh/CPP Games/Breda Assignment/Source/Repository/Breda Assignment VANSH MUNOT/Assets/Player/Crocodile_PNG.png"));
    Crocodile.character_Index = 1;


    Peacock.character_Speed = 350.f;
    Peacock.max_character_Health = 50;
    Peacock.character_Terraforming_Factor = 5;
    Peacock.character_Texture = sf::Texture(sf::Image("C:/Users/vansh/CPP Games/Breda Assignment/Source/Repository/Breda Assignment VANSH MUNOT/Assets/Player/Peacock_PNG.png"));
    Peacock.character_Index = 2;


    player_Speed = Penguin.character_Speed;
    max_player_Health = Penguin.max_character_Health;
    terraforming_Factor = Penguin.character_Terraforming_Factor;
    player_Texture = Penguin.character_Texture;
    current_Character = Penguin.character_Index;

}

#pragma endregion

#pragma region Core Loops

void Player::update(float deltatime)
{

    //movement
    player_Movement(deltatime);
    weapon_Movement();

    //weapon cooldown timer
    weapon_Cooldown(deltatime);

    //weapon updating
    current_Weapon->update(deltatime);

    //attack
    player_Attack();
    weapon_Transformation_Cooldown(deltatime);
    
    //collision
    wall_Collision();
    player_Invincibility(deltatime);
}

void Player::render(sf::RenderTarget& target)
{
    //render
    target.draw(player_Sprite);

    current_Weapon->render(*game_Window);
}
void Player::on_Event(const Event& event)
{
    if (auto* data = dynamic_cast<const player_Health_Change*>(&event))
    {
        player_Health(data->_Change);
    }
    else if (dynamic_cast<const minigame_Win*>(&event))
    {
        character_Transform();
    }
}
#pragma endregion

#pragma region Movement

void Player::player_Movement(float deltatime)
{
    //Inputs
    sf::Vector2f keyboard_Input = {0,0};
    

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
    
    enemy_Collision(deltatime);
    // move after collision done
    player_Sprite.move(player_Velocity);

}

void Player::weapon_Movement()
{
    //rotates current weapon to always point at mouse
    current_Weapon->weapon_Rotate(*game_Window);

    //moves to player position
    current_Weapon->weapon_Position(player_Sprite.getPosition());

    //flip player when weapon rotates
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

#pragma endregion

#pragma region Attack
void Player::player_Attack()
{
    //attack inputs
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        Attack();
    }
}

void Player::Attack()
{
    if (can_Attack)
    {
        current_Weapon->Attack();
        current_weapon_Ammo--;

        //ui event
        ammo_Event.ammo = current_weapon_Ammo;
        notify_Observers(ammo_Event);

        can_Attack = false;
        is_weapon_Cooldown = true;

        //turn on rendering of bar
        weapon_State_Event.state = true;
        notify_Observers(weapon_State_Event);

        if (current_weapon_Ammo <= 0)
        {
            //start Transformation
            is_weapon_Transforming = true;
            is_weapon_Cooldown = false;

            //turn on rendering of bar
            weapon_State_Event.state = true;
            notify_Observers(weapon_State_Event);
        }
    }
}
#pragma endregion

# pragma region Weapon

void Player::weapon_Cooldown(float deltatime)
{
    //weapon cooldown
    if (is_weapon_Cooldown)
    {
        if (weapon_cooldown_Timer < current_weapon_Cooldown)
        {
            weapon_cooldown_Timer += deltatime;

            // reload event
            reload_Weapon.value = current_weapon_Cooldown - weapon_cooldown_Timer;
            notify_Observers(reload_Weapon);
        }
        else
        {
            weapon_cooldown_Timer = 0;
            //weapon cooldown over
            
            //turn off rendering of bar
            weapon_State_Event.state = false;
            notify_Observers(weapon_State_Event);

            can_Attack = true;
            is_weapon_Cooldown = false;
        }
    }    
}
void Player::weapon_Transformation_Cooldown(float deltatime)
{
    //weapon transformation start
    if (is_weapon_Transforming)
    {
        can_Attack = false;

        weapon_Transform_Timer += deltatime;

        // transformation event
        transform_Weapon_Event.value = weapon_Transfrom_Time - weapon_Transform_Timer;
        notify_Observers(transform_Weapon_Event);

        if (weapon_Transform_Timer > weapon_Transfrom_Time)
        {
            //complete tranformation
            is_weapon_Transforming = false;
            is_weapon_Cooldown = true;

            weapon_Transform_Timer = 0;
            transform_Weapon();
            //turn off rendering of bar
            weapon_State_Event.state = false;
            notify_Observers(weapon_State_Event);
        }
    }
}

void Player::transform_Weapon()
{
        //get seed
        //std::random_device rd;
        //intialise  m twister
        //std::mt19937 gen(rd());

        std::uniform_int_distribution<int> random_weapon(0, 2);

        //random number
        int random = random_weapon(rng);
        while (random == last_Weapon)
        {
            random = random_weapon(rng);
        }

        switch (current_Character)
        {
        case 0:
            switch (random)
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
            break;

        case 1:
            switch (random)
            {
            case 0:
                weapon_Assigner(_Mace);
                break;

            case 1:
                weapon_Assigner(_Pistol);

                break;

            case 2:
                weapon_Assigner(_LMG);

                break;
            }

            break;

        case 2:
            switch (random)
            {
            case 0:
                weapon_Assigner(_WarAxe);
                break;

            case 1:
                weapon_Assigner(_Shotgun);

                break;

            case 2:
                weapon_Assigner(_Grenade_Launcher);

                break;
            }

            break;
        }
        
        last_Weapon = random;
        // set weapon position to player
        current_Weapon->weapon_Position(player_Sprite.getPosition());
        can_Attack = true;
        //ui event
        ammo_Event.ammo = current_weapon_Ammo;
        notify_Observers(ammo_Event);
}

void Player::weapon_Assigner(Weapon* weapon)
{
    current_Weapon = weapon;
    current_weapon_Ammo = weapon->_Ammo;
    current_weapon_Cooldown = weapon->cooldown_Timer;
}

void Player::character_Transform()
{
    sf::Vector2f last_Position = player_Sprite.getPosition();
    std::uniform_int_distribution<int> random_character(0, 2);

    //random number
    int random = random_character(rng);

    while (last_Character == random)
    {
        random = random_character(rng);
    }

    switch (random)
    {
    case 0:
        character_Assigner(Penguin, last_Position);

        game_Difficulty_Event.damage_Multiplier = 1;
        game_Difficulty_Event.Health_Multiplier = 1;
        game_Difficulty_Event.max_Enemies = 10;
        game_Difficulty_Event.spawn_Time = 2;
        notify_Observers(game_Difficulty_Event);
        break;

    case 1:
        character_Assigner(Crocodile, last_Position);
        game_Difficulty_Event.damage_Multiplier = 2;
        game_Difficulty_Event.Health_Multiplier = 2;
        game_Difficulty_Event.max_Enemies = 50;
        game_Difficulty_Event.spawn_Time = 1;
        notify_Observers(game_Difficulty_Event);

        break;

    case 2:
        character_Assigner(Peacock, last_Position);
        game_Difficulty_Event.damage_Multiplier = 0.75;
        game_Difficulty_Event.Health_Multiplier = 1.5;
        game_Difficulty_Event.max_Enemies = 25;
        game_Difficulty_Event.spawn_Time = 3;
        notify_Observers(game_Difficulty_Event);

        break;
    }
    last_Character = random;
    transform_Weapon();
}

void Player::character_Assigner(character_Values character, sf::Vector2f position)
{
    player_Speed = character.character_Speed;
    max_player_Health = character.max_character_Health;
    terraforming_Factor = character.character_Terraforming_Factor;
    player_Texture = character.character_Texture;
    current_player_Health = max_player_Health;
    current_Character = character.character_Index;

    player_Sprite.setTexture(player_Texture, true);
    player_Sprite.setPosition(position);
    player_Sprite.setScale({ 1.0f, 1.0f });
    player_Sprite.setOrigin({ static_cast<float>(player_Texture.getSize().x / 2),static_cast<float>(player_Texture.getSize().y / 2) });

    //move to on tranform player
    terraforming_Factor_Event.Terraforming_Factor = terraforming_Factor;
    notify_Observers(terraforming_Factor_Event);

    health_Event.health = current_player_Health;
    notify_Observers(health_Event);
}

#pragma endregion

#pragma region Getters

sf::Vector2f Player::get_Position()
{
    return player_Sprite.getPosition();
}
sf::FloatRect Player::get_GlobalBounds()
{
    return player_Sprite.getGlobalBounds();
}
bool Player::get_can_Damage()
{
    return can_Damage;
}

#pragma endregion

#pragma region Collision

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
    //BOTTOM WALL COLLISION
    else if (position_Y + sprite_Offset_Y > screen_Height)
    {
        player_Sprite.setPosition({ position_X,screen_Height - sprite_Offset_Y });
    }
}
void Player::player_Health(float _Damage)
{

    current_player_Health -= _Damage;
    can_Damage = false;

    current_player_Health = std::clamp(current_player_Health, 0.f, max_player_Health);

    health_Event.health = current_player_Health;
    notify_Observers(health_Event);

    if (current_player_Health <= 0)
    {
        game_Over event;
        event.state = 0;
        notify_Observers(event);
    }
}



void Player::enemy_Collision(float deltatime)
{
    sf::FloatRect current_Bounds = player_Sprite.getGlobalBounds();

    //calculate next position of global bounds
    sf::FloatRect next_Bounds = current_Bounds;
    next_Bounds.position.x += player_Velocity.x;
    next_Bounds.position.y += player_Velocity.y;

    auto& enemies = _EnemySpawner->get_Enemies();
    for (auto& e : enemies)
    {
        sf::FloatRect enemy_Bounds = e->get_GlobalBounds();

        //intersections
        bool touching_X = next_Bounds.findIntersection(enemy_Bounds).has_value();
        bool touching_Y = next_Bounds.findIntersection(enemy_Bounds).has_value();


        if (touching_X)
        {
            player_Velocity.x = 0;
        }

        if (touching_Y)
        {
            player_Velocity.y = 0;
        }

        if (can_Damage && (touching_X || touching_Y))
        {
            player_Health(e->get_Damage());
        }
    }
     

}

void Player::player_Invincibility(float deltatime)
{
    if (can_Damage) 
    {
        return;
    }
        //invincibilty for time when player gets hurt

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
#pragma endregion





