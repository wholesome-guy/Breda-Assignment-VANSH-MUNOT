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
	init_UI();
}

void PlayerUI::update(float deltatime)
{
    UI_Mover(interact_Text);
    FPS_Counter(deltatime);

}

void PlayerUI::render(sf::RenderTarget& target)
{
    render_UI(target);
}

void PlayerUI::on_Event(const Event& event)
{
    if (auto* data = dynamic_cast<const player_Health_Event*>(&event))
    {
        float health = data->health;
        health_Text.setString(std::to_string(static_cast<int>(health)));
    }
    else if(auto * data = dynamic_cast<const player_Ammo_Event*>(&event))
    {
        int ammo = data->ammo;
        ammo_Text.setString(std::to_string(ammo));
    }
    else if (auto* data = dynamic_cast<const weapon_Reload_Cooldown*>(&event))
    {
        float Value = data->value;
        cooldown_Bar_Update(Value, 100, sf::Color::White);
    }
    else if (auto* data = dynamic_cast<const weapon_Transform_Cooldown*>(&event))
    {
        float Value = data->value;
        cooldown_Bar_Update(Value, 30, sf::Color::Magenta);
    }
    else if (auto* data = dynamic_cast<const weapon_State*>(&event))
    {
        is_Bar_Visible = data->state;
    }
    else if (auto* data = dynamic_cast<const interaction_State*>(&event))
    {
        is_InContact_Interactable = data->state;
    }
    else if (auto* data = dynamic_cast<const kill_Count_Event*>(&event))
    {
        int kills = data->kill_Count;
        kill_Text.setString(std::to_string(kills));
    }

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
    setup_Text(ammo_Text, std::to_string(15), 40,
        sf::Color::White, { 55.f, 65.f });

    setup_Sprite(ammo_Sprite, ammo_Texture,
        "C:/Users/vansh/CPP Games/Breda Assignment/Source/Repository/Breda Assignment VANSH MUNOT/Assets/UI/Ammo_UI_PNG.png",
        { 0.08f, 0.08f }, { 25.f, 88.f });

    // Initialize health UI
    setup_Text(health_Text, std::to_string(100), 40,
        sf::Color::Red, { 56.f, 5.f });

    setup_Sprite(health_Sprite, health_Texture,
        "C:/Users/vansh/CPP Games/Breda Assignment/Source/Repository/Breda Assignment VANSH MUNOT/Assets/UI/Heart_UI_PNG.png",
        { 0.08f, 0.08f }, { 27.f, 31.f }, sf::Color::Red);

    // Initialize cooldown bar
    cooldown_Bar.setFillColor(sf::Color::White);

    // Initialize kill counter UI
    setup_Text(kill_Text, std::to_string(0), 40,
        sf::Color::White, { 1200.f, 5.f });

    setup_Sprite(kill_Sprite, kill_Texture,
        "C:/Users/vansh/CPP Games/Breda Assignment/Source/Repository/Breda Assignment VANSH MUNOT/Assets/UI/Skull_UI_PNG.png",
        { 0.08f, 0.08f }, { 1163.f, 31.f });

    // Initialize FPS counter
    setup_Text(FPS_Text, "0", 30, sf::Color::White, { 1226.f, 688.f }, { 0.5f, 0.5f });

    setup_Text(interact_Text, "Right Click", 40,
        sf::Color::White, { 510.f, 635.f });
}

void PlayerUI::cooldown_Bar_Update(float value, float mulitplier, sf::Color colour)
{

        sf::Vector2f offset = { -20,-30 };

        cooldown_Bar.setPosition(_Player->get_Position() + offset);

        cooldown_Bar.setSize({ value * mulitplier,5 });

        cooldown_Bar.setFillColor(colour);

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

    if (is_Bar_Visible)
    {
        target.draw(cooldown_Bar);
    }

    if (is_InContact_Interactable)
    {
        target.draw(interact_Text);
    }
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
