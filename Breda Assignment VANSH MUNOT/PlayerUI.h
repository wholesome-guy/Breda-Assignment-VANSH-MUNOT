#pragma once
#include "EntityBase.h"
#include "Player.h"
#include "GameEngine.h"
#include <sstream>
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

	bool is_Bar_Visible = false;
	float bar_Size_Offset;
	void cooldown_Bar_Update();
	void cooldown_Bar_Render(sf::RenderTarget& target);

	void render_UI(sf::RenderTarget& target);
	void update_UI();

	Player* _Player;

};

