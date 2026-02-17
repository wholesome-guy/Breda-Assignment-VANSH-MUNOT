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
}

void Shape::update(float deltatime)
{
    despawn(deltatime);
    collision();
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
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
        {
            std::cout << "interacted" << "\n";
        }  
    }
    else
    {
        _Player->set_Can_Interact_Sqaure(false);
    }
}


