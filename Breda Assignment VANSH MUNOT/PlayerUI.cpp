#include "PlayerUI.h"

PlayerUI::PlayerUI(): 
	ammo_Text(game_Font, "x/x"),
	ammo_Sprite(ammo_Texture),
	health_Text(game_Font, "1"),
	health_Sprite(health_Texture)
{
    _Player = GameEngine::get_Instance()->get_Player();
	init_UI();
}

void PlayerUI::update(float deltatime)
{
    update_UI();
    cooldown_Bar_Update();
}

void PlayerUI::render(sf::RenderTarget& target)
{
    render_UI(target);
    cooldown_Bar_Render(target);
}

void PlayerUI::init_UI()
{
    if (game_Font.openFromFile("C:/Users/vansh/CPP Games/Breda Assignment/Source/Repository/Breda Assignment VANSH MUNOT/Assets/UI/SpecialElite-Regular.ttf"))
    {
        std::cout << "got the font" << "\n";
    }
    game_Font.setSmooth(false);

    ammo_Text.setString(std::to_string(_Player->get_Ammo()));

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


    health_Text.setString(std::to_string(static_cast<int>(_Player->get_Health())));

    health_Text.setCharacterSize(30);
    health_Text.setStyle(sf::Text::Bold);
    health_Text.setFillColor(sf::Color::White);
    health_Text.setPosition({ 18.f, 11.f });
    health_Text.setScale({ 0.5f,0.5f });


    health_Texture = sf::Texture(sf::Image("C:/Users/vansh/CPP Games/Breda Assignment/Source/Repository/Breda Assignment VANSH MUNOT/Assets/UI/Heart_UI_PNG.png"));
    health_Sprite.setTexture(health_Texture, true);
    health_Sprite.setScale({ 0.025f, 0.025f });
    health_Sprite.setPosition({ 8.f,20.f });
    health_Sprite.setOrigin({ static_cast<float>(health_Texture.getSize().x / 2),static_cast<float>(health_Texture.getSize().y / 2) });

    cooldown_Bar.setFillColor(sf::Color::White);
}

void PlayerUI::cooldown_Bar_Update()
{
    if (_Player->get_CoolDown_Bool(0))
    {
        sf::Vector2f offset = { -20,-30 };

        cooldown_Bar.setPosition(_Player->get_Position() + offset);


        cooldown_Bar.setSize({ _Player->get_Cooldown(0) * 100,5 });

        cooldown_Bar.setFillColor(sf::Color::White);

    }

    if (_Player->get_CoolDown_Bool(1))
    {
        sf::Vector2f offset = { -20,-30 };

        cooldown_Bar.setPosition(_Player->get_Position() + offset);

        cooldown_Bar.setSize({ _Player->get_Cooldown(1) * 30,5 });

        cooldown_Bar.setFillColor(sf::Color::Magenta);

    }
}

void PlayerUI::cooldown_Bar_Render(sf::RenderTarget& target)
{
    if (_Player->get_CoolDown_Bool(0) || _Player->get_CoolDown_Bool(1))
    {
        target.draw(cooldown_Bar);
    }

}

void PlayerUI::render_UI(sf::RenderTarget& target)
{
    target.draw(ammo_Text);
    target.draw(ammo_Sprite);

    target.draw(health_Text);
    target.draw(health_Sprite);
}

void PlayerUI::update_UI()
{
    ammo_Text.setString(std::to_string(_Player->get_Ammo()));
    health_Text.setString(std::to_string(static_cast<int>(_Player->get_Health())));
}
