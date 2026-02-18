#pragma once
#include "EntityBase.h"
#include "Player.h"
#include "GameEngine.h"
#include <sstream>
#include "EnemySpawner.h"
#include "EventSystem.h"
class PlayerUI:public EntityBase,public Observer
{
public:

	PlayerUI();

	void update(float deltatime) override;
	void render(sf::RenderTarget& target) override;

	void on_Event(const Event& event) override;

private:
	void init_UI();

	//UI
	sf::Font game_Font;
	sf::Text ammo_Text;
	sf::Text health_Text;

	sf::Texture ammo_Texture;
	sf::Sprite ammo_Sprite;
	sf::Texture health_Texture;
	sf::Sprite health_Sprite;

	sf::RectangleShape cooldown_Bar;

	sf::Text kill_Text;
	sf::Texture kill_Texture;
	sf::Sprite kill_Sprite;

	sf::Text FPS_Text;

	sf::Text interact_Text;

	bool is_Bar_Visible = false;
	float bar_Size_Offset;

	bool is_InContact_Interactable = false;

	sf::RenderWindow* game_Window;

	//FPS 
	float updateInterval = 0.5f; // How often to update the FPS display
	float accum = 0; // FPS accumulated over the interval
	int frames = 0; // Frames drawn over the interval
	float timeleft = updateInterval;

	//functions

	void cooldown_Bar_Update(float,float,sf::Color);

	void render_UI(sf::RenderTarget& target);

	void FPS_Counter(float deltatime);

	void setup_Text(sf::Text& text, const std::string& content, unsigned int size,
		const sf::Color& color, const sf::Vector2f& position,
		const sf::Vector2f& scale = { 1.f, 1.f });

	void setup_Sprite(sf::Sprite& sprite, sf::Texture& texture, const std::string& filepath,
		const sf::Vector2f& scale, const sf::Vector2f& position,
		const sf::Color& color = sf::Color::White);


	void UI_Mover(sf::Transformable& transform);
	Player* _Player;
};

