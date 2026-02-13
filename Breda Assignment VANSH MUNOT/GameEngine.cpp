#include "GameEngine.h"
#include "Player.h"


//singleton method 
//a pointer of instance is set to null
GameEngine* GameEngine::instance = nullptr;

//constructor -- unity start method
GameEngine::GameEngine() :
	FPS_Text(game_Font, "0")
{
	//instance is assigned
	instance = this;
	init_gameWindow();
	init_Entities();
	init_Text();
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
	//game_Window->setFramerateLimit(60);
}



void GameEngine::init_Text()
{
	if (game_Font.openFromFile("C:/Users/vansh/CPP Games/Breda Assignment/Source/Repository/Breda Assignment VANSH MUNOT/Assets/UI/SpecialElite-Regular.ttf"))
	{
		std::cout << "got the font" << "\n";
	}
	game_Font.setSmooth(false);


	FPS_Text.setString("0");
	FPS_Text.setCharacterSize(30);
	FPS_Text.setStyle(sf::Text::Bold);
	FPS_Text.setFillColor(sf::Color::White);
	FPS_Text.setPosition({ 600.f, 0.f });
	FPS_Text.setScale({ 0.5f,0.5f });
}

void GameEngine::init_Entities()
{
	//make player
	auto player = std::make_unique<Player>();
	//pass for reference raw pointer
	_Player = player.get();
	//transfer ownership by using move? smart pointer owns the object, transfer of ownership = move
	Entities.push_back(std::move(player));


	auto enemy_list = std::make_unique<EnemySpawner>();

	_EnemySpawner = enemy_list.get();
	
	Entities.push_back(std::move(enemy_list));

}
std::vector<std::unique_ptr<Enemy>>& GameEngine::get_Enemies()
{
	return _EnemySpawner->get_Enemies();
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
	}
}

//unity update
void GameEngine::update(float deltatime)
{
	for (auto& e : Entities)
	{
		e->update(deltatime);
	}	

	FPS_Counter(deltatime);
}

//render objects here
void GameEngine::render()
{
	game_Window->clear();

	for (auto& e : Entities)
	{
		e->render(*game_Window);
	}

	game_Window->draw(FPS_Text);

	game_Window->display();
}

void GameEngine::FPS_Counter(float deltatime)
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
