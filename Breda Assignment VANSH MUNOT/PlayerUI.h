#pragma once
#include "EntityBase.h"
#include "Player.h"
#include "GameEngine.h"
#include <sstream>
#include "EnemySpawner.h"
class PlayerUI:public EntityBase
{
public:

	PlayerUI();

	void update(float deltatime) override;
	void render(sf::RenderTarget& target) override;


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

	bool is_Bar_Visible = false;
	float bar_Size_Offset;


	//FPS 
	float updateInterval = 0.5f; // How often to update the FPS display
	float accum = 0; // FPS accumulated over the interval
	int frames = 0; // Frames drawn over the interval
	float timeleft = updateInterval;

	//functions

	void cooldown_Bar_Update();
	void cooldown_Bar_Render(sf::RenderTarget& target);

	void render_UI(sf::RenderTarget& target);
	void update_UI();

	void FPS_Counter(float deltatime);


	void UI_Mover(sf::Transformable& transform);
	Player* _Player;
	EnemySpawner* _EnemySpawner;
};

