#include "GameEngine.h"
#include "Player.h"
#include "PlayerUI.h"
#include "TileMap.h"

//singleton method 
//a pointer of instance is set to null
GameEngine* GameEngine::instance = nullptr;

//constructor -- unity start method
GameEngine::GameEngine() :cursor_Sprite(cursor_Texture),
end_Text(game_Font,""), game_Text(game_Font,"")
{
	//instance is assigned
	instance = this;
	init_gameWindow();
	init_Entities();
}
//destructor -- unity on destroy method
GameEngine::~GameEngine()
{
	delete game_Window;
	//clean instance
	instance = nullptr;
}

//intialise the game window
void GameEngine::init_gameWindow()
{
	_Window_Size = { 640,360 };
	game_Window = new sf::RenderWindow(sf::VideoMode(_Window_Size), "Game");
	mini_Game.init_UI();
	//game_Window->setFramerateLimit(60);

	game_Window->setMouseCursorVisible(false);
	cursor_Texture = sf::Texture(sf::Image("C:/Users/vansh/CPP Games/Breda Assignment/Source/Repository/Breda Assignment VANSH MUNOT/Assets/UI/Cursor_PNG.png"));
	cursor_Sprite.setTexture(cursor_Texture,true);
	cursor_Sprite.setOrigin({ cursor_Texture.getSize().x / 2.f,cursor_Texture.getSize().y / 2.f });

	if (game_Font.openFromFile("C:/Users/vansh/CPP Games/Breda Assignment/Source/Repository/Breda Assignment VANSH MUNOT/Assets/UI/SpecialElite-Regular.ttf"))
	{
		std::cout << "got the font\n";
	}
	game_Font.setSmooth(false);

	end_Text = sf::Text(game_Font, "", 100);
	end_Text.setStyle(sf::Text::Bold);
	end_Text.setScale({ 0.5f,0.5f });

	game_Text = sf::Text(game_Font, "", 60);
	game_Text.setString("Press any key to Start");
	game_Text.setFillColor(sf::Color::White);
	game_Text.setOutlineColor(sf::Color::Black);
	game_Text.setOutlineThickness(5.f);
	game_Text.setStyle(sf::Text::Bold);
	game_Text.setScale({ 0.5f,0.5f });
	sf::FloatRect bounds = game_Text.getLocalBounds();
	game_Text.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });
	game_Text.setPosition({ _Window_Size.x / 2.f, 300 });
	
}



void GameEngine::init_Entities()
{
	//make enemyspawner
	auto enemy_list = std::make_unique<EnemySpawner>();
	//raw pointer
	_EnemySpawner = enemy_list.get();

	//make player
	auto player = std::make_unique<Player>();
	//pass for reference raw pointer
	_Player = player.get();

	auto player_UI = std::make_unique<PlayerUI>();

	_PlayerUI = player_UI.get();

	auto tilemap = std::make_unique<TileMap>();

	_TileMap = tilemap.get();

	Entities.push_back(std::move(tilemap));

	//transfer ownership by using move? smart pointer owns the object, transfer of ownership = move
	Entities.push_back(std::move(player));

	Entities.push_back(std::move(enemy_list));

	Entities.push_back(std::move(player_UI));


	//making player the observe of enemy_spawner
	_EnemySpawner->add_Observer(_Player);
	_EnemySpawner->add_Observer(_PlayerUI);
	_EnemySpawner->add_Observer(_TileMap);

	//enemyspawner is an observer of gameengine
	this->add_Observer(_EnemySpawner);
	this->add_Observer(_Player);

	//Player Ui is the observer for Player
	_Player->add_Observer(_PlayerUI);
	_Player->add_Observer(_TileMap);
	_Player->add_Observer(_EnemySpawner);
	_Player->add_Observer(this);
	
	_TileMap->add_Observer(_PlayerUI);
	_TileMap->add_Observer(this);
}

void GameEngine::run()
{
	// update and render loop when game is running
	while (game_Window->isOpen())
	{
		poll_Event();

		//deltatime unity deltatime, time between previous and current frame
		float deltatime = clock.restart().asSeconds();
		//loops
		update(deltatime);

		render();
	}
}


//method to check(poll) events of the game window/ application
void GameEngine::poll_Event()
{
	while (auto event = game_Window->pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{
			game_Window->close();
		}
		if (event->is<sf::Event::KeyPressed>())
		{
			if (event->getIf<sf::Event::KeyPressed>())
			{
				is_Game_Start = true;
				if (is_Game_Over)
				{
					game_Window->close();
				}
			}
		}
	}
}

void GameEngine::on_Event(const Event& event)
{
	if (dynamic_cast<const minigame_Start*>(&event))
	{
		pending_MiniGame_Start = true;
	}
	else if (auto* data = dynamic_cast<const game_Over*>(&event))
	{
		is_Game_Over = true;
		game_End_State = data->state;
	}
}

//unity update
void GameEngine::update(float deltatime)
{
	cursor();

	if (is_Game_Over)
	{
		return;
	}
	if (is_Game_Start)
	{
		for (auto& e : Entities)
		{
			e->update(deltatime);
		}

		miniGame_Update(deltatime);
	}

}



//render objects here
void GameEngine::render()
{
	game_Window->clear();

	for (auto& e : Entities)
	{
		e->render(*game_Window);
	}
	mini_Game.render(*game_Window);

	if (is_Game_Over)
	{
		if (game_End_State == 0)
		{
			end_Text.setFillColor(sf::Color::Red);
			end_Text.setString("You Died");
		}
		else
		{
			end_Text.setFillColor(sf::Color::Yellow);
			end_Text.setString("Terraforming Complete");
		}
		sf::FloatRect bounds = end_Text.getLocalBounds();
		end_Text.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });
		end_Text.setPosition({ _Window_Size.x / 2.f, _Window_Size.y / 2.f });

		game_Text.setString("Press any key to Quit");
		game_Window->draw(game_Text);


		game_Window->draw(end_Text);

	}
	if (!is_Game_Start)
	{
		game_Window->draw(game_Text);
	}
	
	game_Window->draw(cursor_Sprite);


	game_Window->display();
}


void GameEngine::miniGame_Update(float deltatime)
{
	mini_Game.update(deltatime);

	if (pending_MiniGame_Start)
	{
		pending_MiniGame_Start = false;
		minigame_Completed_Handled = false;

		mini_Game.start_Mini_Game();
		minigame_Active_Event.active = true;
		notify_Observers(minigame_Active_Event);
		_TileMap->set_MiniGame_Active(true);
	}

	if (mini_Game.get_Is_Complete() && !minigame_Completed_Handled)
	{
		minigame_Completed_Handled = true;
		minigame_Active_Event.active = false;
		notify_Observers(minigame_Active_Event);
		_TileMap->set_MiniGame_Active(false);

		if (mini_Game.get_Is_Won())
		{
			notify_Observers(miniGame_Win_Event);
		}
		notify_Observers(minigame_Complete{});
	}
}

void GameEngine::cursor()
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(*game_Window);
	sf::Vector2f worldPos = game_Window->mapPixelToCoords(mousePos);
	cursor_Sprite.setPosition(worldPos);
}


