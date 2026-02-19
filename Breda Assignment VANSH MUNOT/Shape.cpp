#include "Shape.h"
#include "Player.h"
#include "GameEngine.h"

//enemy heart, name is shape because of previous mechahic(scraped)
Shape::Shape(sf::Vector2f position):
	shape_Sprite(square_Texture)
{
    init_Sprite(position);
    _Player = GameEngine::get_Instance()->get_Player();
    _GameEngine = GameEngine::get_Instance();
}

Shape::~Shape()
{
    _GameEngine->remove_Observer(this);
}

void Shape::render(sf::RenderTarget& target)
{
	target.draw(shape_Sprite);
    //mini_Game.render(target);
}

void Shape::update(float deltatime)
{
    despawn(deltatime);
    collision();
    //mini_Game.update(deltatime);
}

void Shape::on_Event(const Event& event)
{
    if (dynamic_cast<const minigame_Complete*>(&event))
    {
        is_MiniGame_Active = false;
       
        minigame_Interaction_State.state = false;
        notify_Observers(minigame_Interaction_State);

        is_Despawn = true;
    }
}

bool Shape::get_Despawn()
{
    return is_Despawn;
}

void Shape::init_Sprite(sf::Vector2f position)
{
    square_Texture = sf::Texture(sf::Image("C:/Users/vansh/CPP Games/Breda Assignment/Source/Repository/Breda Assignment VANSH MUNOT/Assets/Player/Square_PNG.png"));

    //setting sprite properties
    shape_Sprite.setTexture(square_Texture, true);
    shape_Sprite.setPosition(position);
    shape_Sprite.setScale({ 1.f, 1.f });
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

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && !is_MiniGame_Active)
        {

            is_MiniGame_Active = true;
            notify_Observers(minigame_Start{});
            minigame_Interaction_State.state = false;
            notify_Observers(minigame_Interaction_State);

        }  
    }
    else
    {
        minigame_Interaction_State.state = false;
        notify_Observers(minigame_Interaction_State);
    }
}


