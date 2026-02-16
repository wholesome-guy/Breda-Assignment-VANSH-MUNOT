#include "PlayerUI.h"

PlayerUI::PlayerUI(): 
	ammo_Text(game_Font, "x/x"),
	ammo_Sprite(ammo_Texture),
	health_Text(game_Font, "1"),
	health_Sprite(health_Texture),
    kill_Text(game_Font, "0"),
    kill_Sprite(kill_Texture),
    FPS_Text(game_Font,"0")
{
    _Player = GameEngine::get_Instance()->get_Player();
    _EnemySpawner = GameEngine::get_Instance()->get_EnemySpawner();
	init_UI();
}

void PlayerUI::update(float deltatime)
{
    update_UI();
    cooldown_Bar_Update();
    FPS_Counter(deltatime);

}

void PlayerUI::render(sf::RenderTarget& target)
{
    render_UI(target);
    cooldown_Bar_Render(target);

    target.draw(FPS_Text);
}

void PlayerUI::init_UI()
{
    if (game_Font.openFromFile("C:/Users/vansh/CPP Games/Breda Assignment/Source/Repository/Breda Assignment VANSH MUNOT/Assets/UI/SpecialElite-Regular.ttf"))
    {
        std::cout << "got the font" << "\n";
    }
    game_Font.setSmooth(false);

    ammo_Text.setString(std::to_string(_Player->get_Ammo()));
    ammo_Text.setCharacterSize(40);
    ammo_Text.setStyle(sf::Text::Bold);
    ammo_Text.setFillColor(sf::Color::White);
    ammo_Text.setPosition({ 55.f, 65.f });
    ammo_Text.setScale({ 1.f,1.f });

    ammo_Texture = sf::Texture(sf::Image("C:/Users/vansh/CPP Games/Breda Assignment/Source/Repository/Breda Assignment VANSH MUNOT/Assets/UI/Ammo_UI_PNG.png"));
    ammo_Sprite.setTexture(ammo_Texture, true);
    ammo_Sprite.setScale({ 0.08f, 0.08f });
    ammo_Sprite.setPosition({ 25.f, 88.f });
    ammo_Sprite.setOrigin({ static_cast<float>(ammo_Texture.getSize().x / 2),static_cast<float>(ammo_Texture.getSize().y / 2) });


    health_Text.setString(std::to_string(static_cast<int>(_Player->get_Health())));

    health_Text.setCharacterSize(40);
    health_Text.setStyle(sf::Text::Bold);
    health_Text.setFillColor(sf::Color::Red);
    health_Text.setPosition({ 56.f, 5.f });
    health_Text.setScale({ 1.f,1.f });


    health_Texture = sf::Texture(sf::Image("C:/Users/vansh/CPP Games/Breda Assignment/Source/Repository/Breda Assignment VANSH MUNOT/Assets/UI/Heart_UI_PNG.png"));
    health_Sprite.setTexture(health_Texture, true);
    health_Sprite.setScale({ 0.08f, 0.08f });
    health_Sprite.setPosition({ 27.f,31.f });
    health_Sprite.setOrigin({ static_cast<float>(health_Texture.getSize().x / 2),static_cast<float>(health_Texture.getSize().y / 2) });
    health_Sprite.setColor(sf::Color::Red);

    cooldown_Bar.setFillColor(sf::Color::White);

    kill_Text.setString(std::to_string(_EnemySpawner->get_Kill_Count()));

    kill_Text.setCharacterSize(40);
    kill_Text.setStyle(sf::Text::Bold);
    kill_Text.setFillColor(sf::Color::White);
    kill_Text.setPosition({ 1200.f, 5.f });
    kill_Text.setScale({ 1.f,1.f });

    kill_Texture = sf::Texture(sf::Image("C:/Users/vansh/CPP Games/Breda Assignment/Source/Repository/Breda Assignment VANSH MUNOT/Assets/UI/Skull_UI_PNG.png"));
    kill_Sprite.setTexture(kill_Texture, true);
    kill_Sprite.setScale({ 0.08f, 0.08f });
    kill_Sprite.setPosition({ 1163.f, 31.f });
    kill_Sprite.setOrigin({ static_cast<float>(kill_Texture.getSize().x / 2),static_cast<float>(kill_Texture.getSize().y / 2) });

    FPS_Text.setString("0");
    FPS_Text.setCharacterSize(30);
    FPS_Text.setStyle(sf::Text::Bold);
    FPS_Text.setFillColor(sf::Color::White);
    FPS_Text.setPosition({ 600.f, 0.f });
    FPS_Text.setScale({ 0.5f,0.5f });
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

    target.draw(kill_Text);
    target.draw(kill_Sprite);
}

void PlayerUI::update_UI()
{
    ammo_Text.setString(std::to_string(_Player->get_Ammo()));
    health_Text.setString(std::to_string(static_cast<int>(_Player->get_Health())));

    kill_Text.setString(std::to_string(_EnemySpawner->get_Kill_Count()));

   // UI_Mover(health_Text);
}

void PlayerUI::FPS_Counter(float deltatime)
{
    timeleft -= deltatime;
    accum += 1 / deltatime;
    ++frames;

    // Interval ended - update GUI text and start new interval
    if (timeleft <= 0.0)
    {
        // Calculate the FPS
        float fps = accum / frames;

        sf::String string = std::to_string(static_cast<int>(roundf(fps)));

        FPS_Text.setString(string);

        if (fps < 30)
            FPS_Text.setFillColor(sf::Color::Yellow);
        else
            if (fps < 10)
                FPS_Text.setFillColor(sf::Color::Red);
            else
                FPS_Text.setFillColor(sf::Color::Green);

        timeleft = updateInterval;
        accum = 0.0f;
        frames = 0;
    }
}


void PlayerUI::UI_Mover(sf::Transformable& transform)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
        transform.move({ 0, -1 });
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        transform.move({ 0, 1 });
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        transform.move({ 1, 0 });
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        transform.move({ -1, 0 });
    // Print position on spacebar
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
        std::cout << "Position: " << transform.getPosition().x << ", "
        << transform.getPosition().y << std::endl;
}
