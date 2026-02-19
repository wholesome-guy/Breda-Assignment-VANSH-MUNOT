#include "Enemy.h"
#include "GameEngine.h"
#include <iostream>
#include <random>
//full declare in cpp file to avoid circualar dependency
#include "Player.h" 
#include "EnemySpawner.h"

Enemy::Enemy(float damage_Multiplier, float health_Multiplier, sf::Vector2f position) :enemy_Sprite(enemy_Texture)
{
    random_Enemy_Type();
    init_Variables(damage_Multiplier, health_Multiplier);
    init_Sprite(position);

}
#pragma region Intialise

void Enemy::init_Variables(float damage_Multiplier, float health_Multiplier)
{
    //random type selected for two category of enemies
    switch (type)
    {
    case 0:
        enemy_Speed = 150.0f;
        max_enemy_Health = 100.f * health_Multiplier;
        enemy_Damage = 10 * damage_Multiplier;
        break;
    case 1:
        enemy_Speed = 75.0f;
        max_enemy_Health = 200.f * health_Multiplier;
        enemy_Damage = 30 * damage_Multiplier;
        break;
    }
    enemy_Health = max_enemy_Health;

    _Player = GameEngine::get_Instance()->get_Player();
    _EnemySpawner = GameEngine::get_Instance()->get_EnemySpawner();

    flash_Time = 0.2f;
    flash_Timer = 0;

    contact_Time = 1.f;
    contact_Timer = 0;

    Despawning = false;
    despawn_Time = 0.25f;
    despawn_Timer = 0;
}
void Enemy::init_Sprite(sf::Vector2f position)
{
    //sprite properties
    switch (type)
    {
    case 0:
        enemy_Texture = sf::Texture(sf::Image("C:/Users/vansh/CPP Games/Breda Assignment/Source/Repository/Breda Assignment VANSH MUNOT/Assets/Player/Enemy_0_PNG.png"));

        break;
    case 1:
        enemy_Texture = sf::Texture(sf::Image("C:/Users/vansh/CPP Games/Breda Assignment/Source/Repository/Breda Assignment VANSH MUNOT/Assets/Player/Enemy_1_PNG.png"));

        break;
    }
    //setting sprite properties
    enemy_Sprite.setTexture(enemy_Texture, true);
    enemy_Sprite.setPosition(position);
    enemy_Sprite.setScale({ 0.25f, 0.25f });
    enemy_Sprite.setOrigin({ static_cast<float>(enemy_Texture.getSize().x / 2),static_cast<float>(enemy_Texture.getSize().y / 2) });

    //health bar sprite
    health_Bar.setFillColor(sf::Color::Red);

    background_Bar.setFillColor(sf::Color::Color(0,0,0,100));
    background_Bar.setSize({ max_enemy_Health * 0.5f,5 });

}

void Enemy::random_Enemy_Type()
{
    //randomiser type
    std::random_device rd;

    std::mt19937 gen(rd());

    std::uniform_int_distribution<int> random_Type(0, 100);

    int probability = random_Type(gen);
    if (probability < 70)
    {
        type = 0;
    }
    else
    {
        type = 1;
    }

}

#pragma endregion

#pragma region Core Loops

void Enemy::update(float deltatime)
{
    //player to enemy direction
    direction_Calculate();
    //collision data
    collision_Data_Calculate(deltatime);
    //movement
    enemy_Movement(deltatime);

    //flash on damage
    enemy_Flashing(deltatime);

    //health bar
    health_Bar_Update();

    despawning_Enemy(deltatime);
}

void Enemy::render(sf::RenderTarget& target)
{
	target.draw(enemy_Sprite);
    target.draw(background_Bar);
    target.draw(health_Bar);
}
void Enemy::on_Event(const Event& event)
{
    if (dynamic_cast<const minigame_Complete*>(&event))
    {
        enemy_KnockBack();
    }
    else if (auto* data = dynamic_cast<const game_Difficulty*>(&event))
    {
        /*damage_Multiplier = data->damage_Multiplier;
        health_Multiplier = data->Health_Multiplier;

        switch (type)
        {
        case 0:
            max_enemy_Health = 100.f * health_Multiplier;
            enemy_Damage = 10 * damage_Multiplier;
            break;
        case 1:
            max_enemy_Health = 200.f * health_Multiplier;
            enemy_Damage = 30 * damage_Multiplier;
            break;
        }
        enemy_Health = max_enemy_Health;*/
    }
}
#pragma endregion

#pragma region Movement
void Enemy::direction_Calculate()
{
    //player to enemy, towards player direction calculator
    sf::Vector2f player_Position = _Player->get_Position();
    sf::Vector2f enemy_Position = enemy_Sprite.getPosition();

    player_enemy_Direction = player_Position - enemy_Position;

    float _magnitude = std::sqrt(player_enemy_Direction.x * player_enemy_Direction.x + player_enemy_Direction.y * player_enemy_Direction.y);

    if (_magnitude != 0)
    {
        player_enemy_Direction = player_enemy_Direction / _magnitude; // normalize
    }


}
void Enemy::collision_Data_Calculate(float deltatime)
{
    sf::FloatRect current_Bounds = enemy_Sprite.getGlobalBounds();

    next_X_Bounds = current_Bounds;
    next_X_Bounds.position.x += (player_enemy_Direction.x * enemy_Speed * deltatime)
        + (knockback_Velocity.x * deltatime);

    next_Y_Bounds = current_Bounds;
    next_Y_Bounds.position.y += (player_enemy_Direction.y * enemy_Speed * deltatime)
        + (knockback_Velocity.y * deltatime);
}
void Enemy::enemy_player_Collision(float deltatime)
{
    //collision
    sf::FloatRect player_Bounds = _Player->get_GlobalBounds();

    movement_Direction = player_enemy_Direction;

    bool contact_X = next_X_Bounds.findIntersection(player_Bounds).has_value();
    bool contact_Y = next_Y_Bounds.findIntersection(player_Bounds).has_value();
    bool contact_Now = contact_X || contact_Y;

    // only damage on the frame contact
    if (contact_Now && !is_InContact_Player && _Player->get_can_Damage())
    {
        //damage player EVENT
        player_Health_Change event;
        event._Change = enemy_Damage;
        notify_Observers(event);
    }

    is_InContact_Player = contact_Now;

    if (contact_X)
        movement_Direction.x = 0;

    if (contact_Y)
        movement_Direction.y = 0;

    if (is_InContact_Player)
    {
        contact_Timer += deltatime;
        if (contact_Timer > contact_Time)
        {
            is_InContact_Player = false;
            contact_Timer = 0;
        }
    }
    
}

void Enemy::enemy_enemy_Collision(float deltatime)
{
    auto& enemies = _EnemySpawner->get_Enemies();
    for (auto& e : enemies)
    {
        sf::FloatRect enemy_Bounds = e->get_GlobalBounds();

        if (e.get() == this)
            continue;
        //intersections
        bool contact_X = next_X_Bounds.findIntersection(enemy_Bounds).has_value();
        bool contact_Y = next_Y_Bounds.findIntersection(enemy_Bounds).has_value();


        if (contact_X)
        {
            movement_Direction.x = 0;
            knockback_Velocity.x = 0;
        }

        if (contact_Y)
        {
            movement_Direction.y = 0;
            knockback_Velocity.y = 0;
        }
    }
}

void Enemy::enemy_Movement(float deltatime)
{
    enemy_player_Collision(deltatime);
    enemy_enemy_Collision(deltatime);

    enemy_Sprite.move(movement_Direction * enemy_Speed * deltatime);

    //flip to look at player
    if (player_enemy_Direction.x < 0)
    {
        enemy_Sprite.setScale({ -0.75, 0.75 });
    }
    else
    {
        enemy_Sprite.setScale({ 0.75, 0.75 });
    }

    //knockback
    enemy_Sprite.move(knockback_Velocity * deltatime);
    knockback_Velocity *= std::pow(knockback_Decay, deltatime * 60.f);

    float magnitude_Knockback = sqrtf(knockback_Velocity.x * knockback_Velocity.x + knockback_Velocity.y * knockback_Velocity.y);
    if (magnitude_Knockback < 10)
    {
        knockback_Applied = false;
    }
}

void Enemy::enemy_KnockBack()
{
    if (!knockback_Applied)
    {
        knockback_Velocity = -1.f * player_enemy_Direction * knockback_Force;
        knockback_Applied = true;
    }
}
#pragma endregion

#pragma region Health
void Enemy::take_Damage(float _Damage)
{
    //damage
    enemy_Health -= _Damage;
    enemy_Health = std::clamp(enemy_Health, 0.f, max_enemy_Health);

    //knockback
    enemy_KnockBack();

    //flash
    set_Color(sf::Color::Red);
    start_Flash = true;

    if (enemy_Health <= 0)
    {
        Despawning = true;
    }
}
void Enemy::enemy_Flashing(float deltatime)
{
    if (start_Flash)
    {
        flash_Timer += deltatime;
        if (flash_Timer > flash_Time)
        {
            start_Flash = false;
            set_Color(sf::Color::White);
            flash_Timer = 0;
        }
    }
}
void Enemy::health_Bar_Update()
{
    sf::Vector2f offset;

    switch (type)
    {
    case 0:
        offset = { -25,-30 };
        break;
    case 1:
        offset = { -50,-30 };
        break;
    }    

    health_Bar.setPosition(enemy_Sprite.getPosition() + offset);
    background_Bar.setPosition(enemy_Sprite.getPosition() + offset);

    health_Bar.setSize({ enemy_Health * 0.5f,5 });
}
void Enemy::despawning_Enemy(float deltatime)
{
    if (!Despawning)
    {
        return;
    }

    despawn_Timer += deltatime;
    if (despawn_Timer > despawn_Time)
    {
        is_Dead = true;

    }

}
#pragma endregion

#pragma region Setters and Getter
void Enemy::set_Color(sf::Color colour)
{
    enemy_Sprite.setColor(colour);
}
void Enemy::set_Position(sf::Vector2f position)
{
    enemy_Sprite.setPosition(position);
}

sf::Vector2f Enemy::get_Position()
{
    return enemy_Sprite.getPosition();
}
sf::FloatRect Enemy::get_GlobalBounds()
{
    return enemy_Sprite.getGlobalBounds();
}
float Enemy::get_Damage()
{
    return enemy_Damage;
}
bool Enemy::get_Dead_Bool()
{
    return is_Dead;
}
#pragma endregion
