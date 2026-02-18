#pragma once
#include <vector>
#include <algorithm>
//make a base event to use as a package which we will send
// we can make specfic events and even add data if we have to transfer it
struct Event 
{ 
	virtual ~Event() = default; 
};



//add this to observer class as parent class to get
class Observer
{
public:
	virtual void on_Event(const Event& event) = 0;
};


//make spefic Events structs and inherit from Event like this:
// 
// struct spefic_Event_Name:Event
// {
// data here, or nothing
// }
 
// on the observed inherited child class
//overide the func

/*
    void some_Observer::on_Event(const Event& event)
{
       for no data in event
    if (dynamic_cast<const spefic_Event_Name*>(&event))
    {
        some_func();
    }
       for data in event
    else if (auto* d = dynamic_cast<const spefic_Event_Name*>(&event))
    {
        use d->data and d->data
    }
}
    */

class Subject
{
private:
    std::vector<Observer*> observers;

public:
    void add_Observer(Observer* observer)
    {
        observers.push_back(observer);
    }

    void remove_Observer(Observer* observer)
    {
        observers.erase(
            std::remove(observers.begin(), observers.end(), observer),
            observers.end()
        );
    }

protected:
    void notify_Observers(const Event& event)
    {
        for (auto* o : observers)
        {
            o->on_Event(event);
        }
    }
};
//on the subject/ speaker class create a vector<Observer*> list_Name
// add the observes with proper instance pointer, mostly from the GameEngine.cpp
//and call it like this 

//for no data
// notify_Oberserver(specific_Event_Name{});

// for data
//  specific_Event_Name event;
//  event.data = some data;
//  notify_Observers(event);

//stucts here please

struct player_Health_Change : Event
{
    float _Change;
};

struct player_Health_Event :Event 
{
    float health;
};
struct player_Ammo_Event :Event
{
    int ammo;
};
struct weapon_Reload_Cooldown :Event 
{
    float value;
};
struct weapon_Transform_Cooldown :Event 
{ 
    float value; 
};
struct weapon_State :Event 
{
    bool state;
};
struct interaction_State :Event
{
    bool state;
};
struct kill_Count_Event :Event 
{
    int kill_Count;
};


struct minigame_Active_State : Event 
{
    bool active;
};

struct minigame_Start: Event {};

struct minigame_Complete : Event {};

struct minigame_Win : Event {};

struct player_terraforming_Factor_Event :Event 
{
    int Terraforming_Factor;
};
struct kill_reduction_Terraforming_Time_Event :Event{};

struct terraforming_Percentage_Event :Event 
{
    int percent;
};
struct game_Difficulty :Event 
{
    float damage_Multiplier;
    float Health_Multiplier;

    int max_Enemies;
    float spawn_Time;

};
struct game_Over :Event 
{
    int state;
};

