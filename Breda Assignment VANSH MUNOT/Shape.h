#pragma once
#include <SFML/Graphics.hpp>
class Player;
class Shape
{
public:

	Shape(sf::Vector2f position,sf::Color);
	virtual ~Shape() = default;

	void render(sf::RenderTarget& target);
	void update(float deltatime);

	bool get_Despawn();

private:

	sf::Texture square_Texture;
	sf::Sprite shape_Sprite;


	float despawn_Timer = 0;
	float despawn_Time = 5.f;

	//bool is_InContact;
	bool is_Despawn = false;

	Player* _Player;

	void init_Sprite(sf::Vector2f position, sf::Color);
	void despawn(float deltatime);
	void collision();
};

