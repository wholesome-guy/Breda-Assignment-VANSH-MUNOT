#include "TranformationMiniGame.h"
#include "GameEngine.h"
#include <cmath>
#include <iostream>

TranformationMiniGame::TranformationMiniGame():
    letter_Text(game_font,"1"),
    round_Text(game_font,"1"),
    result_Text(game_font,"1")
{
    init_Variables();
    //init_UI();
}

void TranformationMiniGame::init_Variables()
{
    //intialise randomiser
    std::random_device rd;
    gen = std::mt19937(rd());

}
void TranformationMiniGame::init_UI()
{
    sf::Vector2u window_Size = GameEngine::get_Instance()->get_Window()->getSize();
    float scale_X = static_cast<float>(window_Size.x) / 1280.f;
    float scale_Y = static_cast<float>(window_Size.y) / 720.f;
    float scale = std::min(scale_X, scale_Y); // uniform scale for circular/symmetric elements

    _Centre = { window_Size.x / 2.f, window_Size.y / 2.f };
    ring_Radius = 120.f * scale;

    float panel_Size = 400.f * scale;
    float panel_Half = panel_Size / 2.f;

    overlay.setSize({ static_cast<float>(window_Size.x), static_cast<float>(window_Size.y) });
    overlay.setFillColor(sf::Color(0, 0, 0, 180));

    background_Panel.setSize({ panel_Size, panel_Size });
    background_Panel.setOrigin({ panel_Half, panel_Half });
    background_Panel.setPosition(_Centre);
    background_Panel.setFillColor(sf::Color(30, 30, 30, 220));
    background_Panel.setOutlineColor(sf::Color::White);
    background_Panel.setOutlineThickness(2.f * scale);

    _Ring.setRadius(ring_Radius);
    _Ring.setOrigin({ ring_Radius, ring_Radius });
    _Ring.setPosition(_Centre);
    _Ring.setFillColor(sf::Color::Transparent);
    _Ring.setOutlineColor(sf::Color(100, 100, 100));
    _Ring.setOutlineThickness(8.f * scale);

    white_Bar.setSize({ 12.f * scale, 30.f * scale });
    white_Bar.setOrigin({ 6.f * scale, 15.f * scale });
    white_Bar.setFillColor(sf::Color::White);

    if (game_font.openFromFile("C:/Users/vansh/CPP Games/Breda Assignment/Source/Repository/Breda Assignment VANSH MUNOT/Assets/UI/SpecialElite-Regular.ttf"))
        std::cout << "got the font\n";
    game_font.setSmooth(false);

    letter_Text = sf::Text(game_font, "", static_cast<unsigned int>(160 * scale));
    letter_Text.setFillColor(sf::Color::Color(170, 17, 217));
    letter_Text.setStyle(sf::Text::Bold);
    letter_Text.setScale({ 0.5f,0.5f });

    result_Text = sf::Text(game_font, "", static_cast<unsigned int>(80 * scale));
    result_Text.setScale({ 0.5f,0.5f });

    round_Text = sf::Text(game_font, "", static_cast<unsigned int>(48 * scale));
    round_Text.setScale({ 0.5f,0.5f });
    round_Text.setFillColor(sf::Color::Color(242, 212, 85));
    round_Text.setPosition({ _Centre.x - 180.f * scale, _Centre.y - 180.f * scale });
}
void TranformationMiniGame::start_Mini_Game()
{
    //start mini game
    is_Active = true;

    is_Won = false;
    is_Complete = false;

    current_Round = 0;
    successful_Presses = 0;
    bar_Angle = 0.f;

    show_Result = false;
    key_Was_Pressed = false;

    pick_Random_Letter();
}



void TranformationMiniGame::pick_Random_Letter()
{
    std::uniform_int_distribution<int> random(0, 25);
    target_Char = 'A' + random(gen);
    target_Key = char_To_Key(target_Char);

    letter_Text.setString(target_Char);

    //centre
    sf::FloatRect bounds = letter_Text.getLocalBounds();

    letter_Text.setOrigin({bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f});
    letter_Text.setPosition(_Centre);
}

void TranformationMiniGame::update(float deltatime)
{
    if (!is_Active) 
    {
        return;
    } 

    white_bar_Rotation(deltatime);

    // result display timer
    result_Display(deltatime);

    check_Input(deltatime);

    update_UI();
}

void TranformationMiniGame::white_bar_Rotation(float deltatime)
{
    // rotate bar
    bar_Angle += bar_Speed * deltatime;
    //cant have angle greater than 360
    if (bar_Angle >= 360.f)
    {
        bar_Angle -= 360.f;
    }

    // position bar on ring
    float radians = bar_Angle * (3.14159f / 180.f);
    //polar coordinates to xyz coordinates
    // x = cos(theta)* radius when centre is at 0,0, so here just add centre.x
    // y = sin(theta)* radius when centre is at 0,0, so here just add centre.y

    white_Bar.setPosition({ _Centre.x + std::cos(radians) * ring_Radius, _Centre.y + std::sin(radians) * ring_Radius});
    white_Bar.setRotation(sf::degrees(bar_Angle));
}

void TranformationMiniGame::result_Display(float deltatime)
{
    if (show_Result)
    {
        result_Timer += deltatime;
        if (result_Timer >= result_Display_Time)
        {
            show_Result = false;
            result_Timer = 0.f;

            new_Round();
        }
        return;
    }
}

void TranformationMiniGame::new_Round()
{
    current_Round++;

    pick_Random_Letter();
    randomise_Green_Zone();

    if (current_Round >= total_Rounds)
    {
        if (successful_Presses >= rounds_To_Win)
        {
            is_Won = true;
        }
        else
        {
            is_Won = false;
        }
        is_Active = false;
        is_Complete = true;
        //return when there are more rounds
        return;
    }
}

void TranformationMiniGame::update_UI()
{
    round_Text.setString("Round: " + std::to_string(current_Round + 1) + "/" + std::to_string(total_Rounds) 
        +"   Success: " + std::to_string(successful_Presses));
}

void TranformationMiniGame::check_Input(float deltatime)
{
    bool key_Down = sf::Keyboard::isKeyPressed(target_Key);

    
    if (key_Down && !key_Was_Pressed)
    {
        //checks wheter that bar is in green zone
        bool in_Green = (bar_Angle >= green_Zone_Start && bar_Angle <= green_Zone_End);
        on_Key_Pressed(in_Green);
    }

    key_Was_Pressed = key_Down;
}

void TranformationMiniGame::on_Key_Pressed(bool in_Green_Zone)
{
    if (in_Green_Zone)
    {
        successful_Presses++;

        result_Text.setString("GREAT!");
        result_Text.setFillColor(sf::Color::Green);
    }
    else
    {

        result_Text.setString("MISS!");
        result_Text.setFillColor(sf::Color::Red);
    }

    // centre result text
    sf::FloatRect bounds = result_Text.getLocalBounds();
    result_Text.setOrigin({bounds.position.x + bounds.size.x / 2.f,bounds.position.y + bounds.size.y / 2.f});
    result_Text.setPosition({ _Centre.x, _Centre.y + 80.f });

    //start showing result
    show_Result = true;
    result_Timer = 0.f;
}

void TranformationMiniGame::randomise_Green_Zone()
{
    switch (current_Round)
    {
    case 0:
    {
        // easy 
        std::uniform_real_distribution<float> random_Start(30.f, 180.f);
        green_Zone_Start = random_Start(gen);

        std::uniform_real_distribution<float> random_Size(80.f, 100.f);
        green_Zone_End = green_Zone_Start + random_Size(gen);

        bar_Speed = 80.f;
        break;
    }
    case 1:
    {
        // medium 
        std::uniform_real_distribution<float> random_Start(30.f, 240.f);
        green_Zone_Start = random_Start(gen);

        std::uniform_real_distribution<float> random_Size(50.f, 70.f);
        green_Zone_End = green_Zone_Start + random_Size(gen);

        bar_Speed = 120.f;
        break;
    }
    case 2:
    {
        // hard 
        std::uniform_real_distribution<float> random_Start(30.f, 270.f);
        green_Zone_Start = random_Start(gen);

        std::uniform_real_distribution<float> random_Size(30.f, 45.f);

        green_Zone_End = green_Zone_Start + random_Size(gen);

        bar_Speed = 180.f;
        break;
    }
    case 3:
    {
        // very hard 
        std::uniform_real_distribution<float> random_Start(30.f, 300.f);
        green_Zone_Start = random_Start(gen);

        std::uniform_real_distribution<float> random_Size(15.f, 25.f);

        green_Zone_End = green_Zone_Start + random_Size(gen);

        bar_Speed = 250.f;
        break;
    }
    }

}
void TranformationMiniGame::draw_Green_Arc(sf::RenderTarget& target)
{

    int number_of_Squares = 30;
    float angle_Range = green_Zone_End - green_Zone_Start;
    //base size for each square
    float segment_Size = angle_Range / number_of_Squares;

    for (int i = 0; i < number_of_Squares; i++)
    {
        //place where they should be on the circle outline
        float angle = green_Zone_Start + i * segment_Size;
        float radians = angle * (3.14f / 180.f);

        sf::RectangleShape segment({ 10.f, 10.f });
        segment.setOrigin({ 5.f, 5.f });

        segment.setFillColor(sf::Color(0, 255, 0, 180));
        //polar to normal
        segment.setPosition({_Centre.x + std::cos(radians) * ring_Radius,_Centre.y + std::sin(radians) * ring_Radius});

        target.draw(segment);
    }
}

void TranformationMiniGame::render(sf::RenderTarget& target)
{
    if (!is_Active) 
    {
        return;
    }
    //draw first
    target.draw(overlay);
    target.draw(background_Panel);
    target.draw(_Ring);

    draw_Green_Arc(target);
    //draw last
    target.draw(white_Bar);
    target.draw(letter_Text);
    target.draw(round_Text);

    if (show_Result)
    {
        target.draw(result_Text);
    }
}

bool TranformationMiniGame::get_Is_Active()
{ 
    return is_Active; 
}
bool TranformationMiniGame::get_Is_Won() 
{
    return is_Won; 
}
bool TranformationMiniGame::get_Is_Complete() 
{ 
    return is_Complete; 
}

sf::Keyboard::Key TranformationMiniGame::char_To_Key(char c)
{
    // maps A-Z to SFML keys
    switch (c)
    {
    case 'A': return sf::Keyboard::Key::A;
    case 'B': return sf::Keyboard::Key::B;
    case 'C': return sf::Keyboard::Key::C;
    case 'D': return sf::Keyboard::Key::D;
    case 'E': return sf::Keyboard::Key::E;
    case 'F': return sf::Keyboard::Key::F;
    case 'G': return sf::Keyboard::Key::G;
    case 'H': return sf::Keyboard::Key::H;
    case 'I': return sf::Keyboard::Key::I;
    case 'J': return sf::Keyboard::Key::J;
    case 'K': return sf::Keyboard::Key::K;
    case 'L': return sf::Keyboard::Key::L;
    case 'M': return sf::Keyboard::Key::M;
    case 'N': return sf::Keyboard::Key::N;
    case 'O': return sf::Keyboard::Key::O;
    case 'P': return sf::Keyboard::Key::P;
    case 'Q': return sf::Keyboard::Key::Q;
    case 'R': return sf::Keyboard::Key::R;
    case 'S': return sf::Keyboard::Key::S;
    case 'T': return sf::Keyboard::Key::T;
    case 'U': return sf::Keyboard::Key::U;
    case 'V': return sf::Keyboard::Key::V;
    case 'W': return sf::Keyboard::Key::W;
    case 'X': return sf::Keyboard::Key::X;
    case 'Y': return sf::Keyboard::Key::Y;
    case 'Z': return sf::Keyboard::Key::Z;
    default:  return sf::Keyboard::Key::A;
    }
}
