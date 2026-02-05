#pragma once

#include <SFML/Graphics.hpp>

//base class like unity monobehaviour
class EntityBase
{
	//public things
public:

	EntityBase() = default;
	// #virtual# the traditional of destructor(i dont know what it means, 
	// it refers to creation of a Virtual table where it stores these 
	// virtual mehtods/functions' addresses for the child/derived classes to copy or use for their own Vtable)
	//#defualt# is standard cpp destructor
	virtual ~EntityBase() =default;

	//same virtual thing, but #=0# basically means its PURELY virtual
	// only a derived/child classes can call/impliment these functions
	//basically abstraction 
	virtual void update(float deltatime) =0;
	virtual void render(sf::RenderTarget& target) =0;


};

