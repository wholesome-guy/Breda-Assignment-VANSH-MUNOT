#pragma once
#include <SFML/Graphics.hpp>
#include <random>
class TranformationMiniGame
{
public:
    TranformationMiniGame();
    void start_Mini_Game();
    void update(float deltatime);
    void render(sf::RenderTarget& target);

    bool get_Is_Active();
    bool get_Is_Won();
    bool get_Is_Complete();  
    void init_UI();

private:
    
    //propeties
    bool is_Active = false;
    bool is_Won = false;
    bool is_Complete = false;

    int current_Round = 0;
    int successful_Presses = 0;
    int total_Rounds = 4;
    int rounds_To_Win = 3;

    
    bool key_Was_Pressed = false;

    // rotating bar
    float bar_Angle = 0.f;
    float bar_Speed = 120.f;

    
    float green_Zone_Start = 60.f;
    float green_Zone_End = 120.f;

    
    char target_Char = 'A';
    sf::Keyboard::Key target_Key = sf::Keyboard::Key::A;

    // result display timer
    float result_Timer = 0.f;
    float result_Display_Time = 0.5f;
    bool show_Result = false;

    // randomiser
    std::mt19937 gen;

    //UI
    float ring_Radius = 120.f;
    sf::Vector2f _Centre;

    sf::CircleShape _Ring;
    sf::RectangleShape white_Bar;
    sf::RectangleShape green_Zone_Arc;

    sf::Font game_font;
    sf::Text letter_Text;
    sf::Text result_Text;
    sf::Text round_Text;

    sf::RectangleShape overlay;
    sf::RectangleShape background_Panel;



    //functions
    void init_Variables();

    void white_bar_Rotation(float deltatime);
    void result_Display(float deltatime);
    void new_Round();
    void update_UI();
    void randomise_Green_Zone();
    void pick_Random_Letter();
    void check_Input(float deltatime);

    void on_Key_Pressed(bool in_Green_Zone);
    sf::Keyboard::Key char_To_Key(char c);

    void draw_Green_Arc(sf::RenderTarget& target);
};

