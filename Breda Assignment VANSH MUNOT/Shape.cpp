#include "Shape.h"
#include "Player.h"
#include "GameEngine.h"

Shape::Shape(sf::Vector2f position, sf::Color colour):
	shape_Sprite(square_Texture)
{
    init_Sprite(position, colour);
    _Player = GameEngine::get_Instance()->get_Player();
}

void Shape::render(sf::RenderTarget& target)
{
	target.draw(shape_Sprite);
    mini_Game.render(target);
}

void Shape::update(float deltatime)
{
    despawn(deltatime);
    collision();
    mini_Game.update(deltatime);
}

bool Shape::get_Despawn()
{
    return is_Despawn;
}

void Shape::init_Sprite(sf::Vector2f position, sf::Color colour)
{
    square_Texture = sf::Texture(sf::Image("C:/Users/vansh/CPP Games/Breda Assignment/Source/Repository/Breda Assignment VANSH MUNOT/Assets/Player/Square_PNG.png"));

    //setting sprite properties
    shape_Sprite.setTexture(square_Texture, true);
    shape_Sprite.setPosition(position);
    shape_Sprite.setColor(colour);
    shape_Sprite.setScale({ 3.f, 3.f });
    shape_Sprite.setOrigin({ static_cast<float>(square_Texture.getSize().x / 2),static_cast<float>(square_Texture.getSize().y / 2) });
}

void Shape::despawn(float deltatime)
{
    if (is_MiniGame_Active)
    {
        return;
    }
    despawn_Timer += deltatime;
    if (despawn_Timer > despawn_Time)
    {
        is_Despawn = true;
        despawn_Timer = 0;
    }
}

void Shape::collision()
{
    if (shape_Sprite.getGlobalBounds().findIntersection(_Player->get_GlobalBounds()))
    {
        minigame_Interaction_State.state = true;
        notify_Observers(minigame_Interaction_State);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) && !mini_Game.get_Is_Active())
        {
            mini_Game.start_Mini_Game();

            is_MiniGame_Active = true;
            minigame_Active_Event.active = true;
            notify_Observers(minigame_Active_Event);
        }  
    }
    else
    {
        minigame_Interaction_State.state = false;
        notify_Observers(minigame_Interaction_State);
    }

    if (mini_Game.get_Is_Complete())
    {

        is_MiniGame_Active = false;
        minigame_Active_Event.active = false;
        notify_Observers(minigame_Active_Event);

        minigame_Interaction_State.state = false;
        notify_Observers(minigame_Interaction_State);

        if (mini_Game.get_Is_Won())
        {
            std::cout << "Won" << "\n";
        }
        notify_Observers(minigame_Complete{});

        is_Despawn = true;  
        return;
    }
}


