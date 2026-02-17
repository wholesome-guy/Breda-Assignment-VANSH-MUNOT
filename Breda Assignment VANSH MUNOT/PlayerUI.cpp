#include "PlayerUI.h"

PlayerUI::PlayerUI(): 
	ammo_Text(game_Font, "x/x"),
	ammo_Sprite(ammo_Texture),
	health_Text(game_Font, "1"),
	health_Sprite(health_Texture),
    kill_Text(game_Font, "0"),
    kill_Sprite(kill_Texture),
    FPS_Text(game_Font,"0"),
    interact_Text(game_Font,"press f")
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
}

void PlayerUI::init_UI()
{
    // Load and configure font
    if (game_Font.openFromFile("C:/Users/vansh/CPP Games/Breda Assignment/Source/Repository/Breda Assignment VANSH MUNOT/Assets/UI/SpecialElite-Regular.ttf"))
    {
        std::cout << "got the font" << "\n";
    }
    game_Font.setSmooth(false);

    // Initialize ammo UI
    setup_Text(ammo_Text, std::to_string(_Player->get_Ammo()), 40,
        sf::Color::White, { 55.f, 65.f });

    setup_Sprite(ammo_Sprite, ammo_Texture,
        "C:/Users/vansh/CPP Games/Breda Assignment/Source/Repository/Breda Assignment VANSH MUNOT/Assets/UI/Ammo_UI_PNG.png",
        { 0.08f, 0.08f }, { 25.f, 88.f });

    // Initialize health UI
    setup_Text(health_Text, std::to_string(static_cast<int>(_Player->get_Health())), 40,
        sf::Color::Red, { 56.f, 5.f });

    setup_Sprite(health_Sprite, health_Texture,
        "C:/Users/vansh/CPP Games/Breda Assignment/Source/Repository/Breda Assignment VANSH MUNOT/Assets/UI/Heart_UI_PNG.png",
        { 0.08f, 0.08f }, { 27.f, 31.f }, sf::Color::Red);

    // Initialize cooldown bar
    cooldown_Bar.setFillColor(sf::Color::White);

    // Initialize kill counter UI
    setup_Text(kill_Text, std::to_string(_EnemySpawner->get_Kill_Count()), 40,
        sf::Color::White, { 1200.f, 5.f });

    setup_Sprite(kill_Sprite, kill_Texture,
        "C:/Users/vansh/CPP Games/Breda Assignment/Source/Repository/Breda Assignment VANSH MUNOT/Assets/UI/Skull_UI_PNG.png",
        { 0.08f, 0.08f }, { 1163.f, 31.f });

    // Initialize FPS counter
    setup_Text(FPS_Text, "0", 30, sf::Color::White, { 1226.f, 688.f }, { 0.5f, 0.5f });

    setup_Text(interact_Text, "Press F", 40,
        sf::Color::White, { 598.f, 635.f });
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

    target.draw(FPS_Text);
    if (_Player->get_Can_Interact_Square())
    {
        target.draw(interact_Text);
    }
}

void PlayerUI::update_UI()
{
    ammo_Text.setString(std::to_string(_Player->get_Ammo()));
    health_Text.setString(std::to_string(static_cast<int>(_Player->get_Health())));

    kill_Text.setString(std::to_string(_EnemySpawner->get_Kill_Count()));

    //UI_Mover(interact_Text);
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

// Helper function to setup text elements
void PlayerUI::setup_Text(sf::Text& text, const std::string& content, unsigned int size,
    const sf::Color& color, const sf::Vector2f& position,
    const sf::Vector2f& scale)
{
    text.setString(content);
    text.setCharacterSize(size);
    text.setStyle(sf::Text::Bold);
    text.setFillColor(color);
    text.setPosition(position);
    text.setScale(scale);
}

// Helper function to setup sprite elements
void PlayerUI::setup_Sprite(sf::Sprite& sprite, sf::Texture& texture, const std::string& filepath,
    const sf::Vector2f& scale, const sf::Vector2f& position,
    const sf::Color& color)
{
    texture = sf::Texture(sf::Image(filepath));
    sprite.setTexture(texture, true);
    sprite.setScale(scale);
    sprite.setPosition(position);
    sprite.setOrigin({ static_cast<float>(texture.getSize().x / 2),
                      static_cast<float>(texture.getSize().y / 2) });
    sprite.setColor(color);
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

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
        std::cout << "Position: " << transform.getPosition().x << ", "
        << transform.getPosition().y << std::endl;
}
