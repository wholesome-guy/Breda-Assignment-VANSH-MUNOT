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
    if (mini_Game.get_Is_Active())
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
        _Player->set_Can_Interact_Sqaure(true);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) && !mini_Game.get_Is_Active())
        {
            mini_Game.start_Mini_Game();
            GameEngine::get_Instance()->set_Mini_Game_Active(mini_Game.get_Is_Active());
        }  
    }
    else
    {
        _Player->set_Can_Interact_Sqaure(false);
    }

    if (mini_Game.get_Is_Complete())
    {
        GameEngine::get_Instance()->set_Mini_Game_Active(mini_Game.get_Is_Active());
        _Player->set_Can_Interact_Sqaure(false);

        if (mini_Game.get_Is_Won())
        {
            std::cout << "Won" << "\n";
        }
        is_Despawn = true;
    }
}


