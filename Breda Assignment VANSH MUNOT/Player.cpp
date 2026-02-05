#include "Player.h"
#include "GameEngine.h"


//constructor unity start
Player::Player():
    //intialiser list for things with no default constructors
	player_Sprite_m(player_Texture_m)
{
    init_playerSprite();
    init_Variables();
}

void Player::init_playerSprite()
{
    //loadind from memory - new texture which is an image
    player_Texture_m = sf::Texture(sf::Image("C:/Users/vansh/CPP Games/Breda Assignment/Source/Repository/Breda Assignment VANSH MUNOT/Assets/Player/Triangle_PNG_Red.png"));
    
    //setting sprite properties
    player_Sprite_m.setTexture(player_Texture_m, true);
    player_Sprite_m.setPosition({ 100.f, 100.f });
    player_Sprite_m.setScale({ 1.f, 1.f });
    player_Sprite_m.setOrigin({ static_cast<float>(player_Texture_m.getSize().x / 2),static_cast<float>(player_Texture_m.getSize().y / 2) });
}
void Player::init_Variables()
{
    player_Speed_m = 5.0f;
    //singleton access
    game_Window_m = GameEngine::getInstance()->getWindow();

    
}

#pragma region Setting weapons
void Player::set_Sword(Weapon* weapon)
{
    _sword_m = weapon;  
    current_Weapon_m = _sword_m;
    current_weapon_Ammo_m = _sword_m->_Ammo;
    current_weapon_Cooldown_m = _sword_m->cooldown_Timer;
}
void Player::set_Rifle(Weapon* weapon)
{
    _rifle_m = weapon;
}
void Player::set_RPG(Weapon* weapon)
{
    _RPG_m = weapon;
}

#pragma endregion

void Player::update(float deltatime)
{
    player_Movement();
    current_Weapon_m->weapon_Rotate(*game_Window_m);

    if (is_weapon_Cooldown_m)
    {
        weapon_Cooldown(deltatime);
    }

    player_Attack();
 
}

void Player::render(sf::RenderTarget& target)
{
    target.draw(player_Sprite_m);
}

//checks keyboard inputs and normalises the input vector
void Player::player_Movement()
{
    sf::Vector2f keyboard_Input = {0,0};
    sf::Vector2f player_Velocity;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    {
        keyboard_Input.x -= 1.0f;
        player_Sprite_m.setScale({ -1.f, 1.f });

    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    {
        keyboard_Input.x += 1.0f;
        player_Sprite_m.setScale({ 1.f, 1.f });
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
        player_Velocity = (keyboard_Input / maginutude_Kinput) * player_Speed_m;
    }
    else
    {
        player_Velocity = keyboard_Input * player_Speed_m;
    }
    //sprite move function
    player_Sprite_m.move(player_Velocity);

    current_Weapon_m->weapon_Position(player_Velocity);

}
void Player::player_Attack()
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        if (can_Attack_m)
        {
            current_Weapon_m->Attack();
            current_weapon_Ammo_m--;

            std::cout << current_weapon_Ammo_m << "\n";

            if (current_weapon_Ammo_m <= 0)
            {
                transform_Weapon();
            }
            is_weapon_Cooldown_m = true;


        }
    }
}
void Player::weapon_Cooldown(float deltatime)
{
    can_Attack_m = false;
    if (weapon_cooldown_Timer_m<current_weapon_Cooldown_m)
    {
        weapon_cooldown_Timer_m += deltatime;
    }
    else
    {
        weapon_cooldown_Timer_m = 0;
        can_Attack_m = true;
        is_weapon_Cooldown_m = false;
    }
}

void Player::transform_Weapon()
{
    srand(time(NULL));

    int random_Number = (rand() % 3);

    can_Attack_m = false;

    switch (random_Number)
    {
        case 0:
            current_Weapon_m = _sword_m;
            current_weapon_Ammo_m = _sword_m->_Ammo;
            current_weapon_Cooldown_m = _sword_m->cooldown_Timer;
        break;

        case 1:
            current_Weapon_m = _rifle_m;
            current_weapon_Ammo_m = _rifle_m->_Ammo;
            current_weapon_Cooldown_m = _rifle_m->cooldown_Timer;
        break;

        case 2:
            current_Weapon_m = _RPG_m;
            current_weapon_Ammo_m = _RPG_m->_Ammo;
            current_weapon_Cooldown_m = _RPG_m->cooldown_Timer;
        break;
    }
    can_Attack_m = true;

}




