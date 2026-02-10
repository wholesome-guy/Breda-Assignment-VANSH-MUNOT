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
	player_Sprite(player_Texture),
    ammo_Text(game_Font,"x/x"),
    ammo_Sprite(ammo_Texture),
    health_Text(game_Font,"1"),
    health_Sprite(health_Texture)
{
    init_playerSprite();
    init_Variables();
    init_Weapons();
    init_UI();

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
void Player::init_UI()
{
    if (game_Font.openFromFile("C:/Users/vansh/CPP Games/Breda Assignment/Source/Repository/Breda Assignment VANSH MUNOT/Assets/UI/SpecialElite-Regular.ttf"))
    {
        std::cout << "got the font" << "\n";
    }
    game_Font.setSmooth(false);

    std::stringstream ammo_string;

    ammo_string << current_weapon_Ammo;

    ammo_Text.setString(ammo_string.str());
    ammo_Text.setCharacterSize(30);
    ammo_Text.setStyle(sf::Text::Bold);
    ammo_Text.setFillColor(sf::Color::White);
    ammo_Text.setPosition({ 18.f, 31.f });
    ammo_Text.setScale({ 0.5f,0.5f });

    ammo_Texture = sf::Texture(sf::Image("C:/Users/vansh/CPP Games/Breda Assignment/Source/Repository/Breda Assignment VANSH MUNOT/Assets/UI/Ammo_UI_PNG.png"));
    ammo_Sprite.setTexture(ammo_Texture, true);
    ammo_Sprite.setScale({ 0.025f, 0.025f });
    ammo_Sprite.setPosition({ 8.f, 40.f });
    ammo_Sprite.setOrigin({ static_cast<float>(ammo_Texture.getSize().x / 2),static_cast<float>(ammo_Texture.getSize().y / 2) });

    std::stringstream health_string;

    health_string << player_Health;

    health_Text.setString(health_string.str());
    health_Text.setCharacterSize(30);
    health_Text.setStyle(sf::Text::Bold);
    health_Text.setFillColor(sf::Color::White);
    health_Text.setPosition({ 18.f, 11.f });
    health_Text.setScale({ 0.5f,0.5f });
    

    health_Texture = sf::Texture(sf::Image("C:/Users/vansh/CPP Games/Breda Assignment/Source/Repository/Breda Assignment VANSH MUNOT/Assets/UI/Heart_UI_PNG.png"));
    health_Sprite.setTexture(health_Texture,true);
    health_Sprite.setScale({ 0.025f, 0.025f });
    health_Sprite.setPosition({ 8.f,20.f });
    health_Sprite.setOrigin({ static_cast<float>(health_Texture.getSize().x / 2),static_cast<float>(health_Texture.getSize().y / 2) });
}
void Player::init_Variables()
{
    player_Speed = 5.0f;
    max_player_Health = 100;
    player_Health = max_player_Health;
    //singleton access
    game_Window = GameEngine::get_Instance()->get_Window();  

    sprite_Offset_X = static_cast<float>(player_Texture.getSize().x / 2);
    sprite_Offset_Y = static_cast<float>(player_Texture.getSize().y / 2);

    sf::Vector2u screen_Size = GameEngine::get_Instance()->get_Window_Size();
    screen_Width = static_cast<float>(screen_Size.x);
    screen_Height = static_cast<float>(screen_Size.y);
}


void Player::init_Weapons()
{
    //initialising weapons
    _Sword = new Sword();  
    current_Weapon = _Sword;
    current_weapon_Ammo = _Sword->_Ammo;
    current_weapon_Cooldown = _Sword->cooldown_Timer;

    _Rifle = new Rifle();
    _RPG = new RPG();

}


void Player::update(float deltatime)
{
    
    player_Movement();
    weapon_Movement();

    //weapon cooldown timer
    if (is_weapon_Cooldown)
    {
        weapon_Cooldown(deltatime);
    }

    //attack
    player_Attack();

    wall_Collision();
    enemy_Collision();

    update_UI();
 
}

void Player::render(sf::RenderTarget& target)
{
    target.draw(player_Sprite);

    render_UI(target);

    //only current weapon is rendered
    current_Weapon->render(*game_Window);
}

//checks keyboard inputs and normalises the input vector
void Player::player_Movement()
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
        player_Velocity = (keyboard_Input / maginutude_Kinput) * player_Speed;
    }
    else
    {
        player_Velocity = keyboard_Input * player_Speed;
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
        //  player face left
        player_Sprite.setScale({ -1.f, 1.f });
        current_Weapon->weapon_Scale({ 1,-1 });
    }
    else
    {
        // player face right
        player_Sprite.setScale({ 1.f, 1.f });
        current_Weapon->weapon_Scale({ 1,1 });

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
                transform_Weapon();
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

    can_Attack = false;

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

void Player::update_UI()
{
    std::stringstream ammo_string;

    ammo_string << current_weapon_Ammo;

    ammo_Text.setString(ammo_string.str());


    std::stringstream health_string;

    health_string << player_Health;

    health_Text.setString(health_string.str());
}

void Player::render_UI(sf::RenderTarget& target)
{
    target.draw(ammo_Text);
    target.draw(ammo_Sprite);

    target.draw(health_Text);
    target.draw(health_Sprite);
}

void Player::enemy_Collision()
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

            }

        }

    }

}





