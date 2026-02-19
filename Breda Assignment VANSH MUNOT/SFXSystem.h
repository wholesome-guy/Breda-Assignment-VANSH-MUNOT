#pragma once
#include <SFML/Audio.hpp>
#include <vector>
#include <memory>
#include <iostream>
class SFXSystem
{
public:
    void play(const sf::SoundBuffer& buffer, float volume = 100.f, float pitch = 1.f, bool randomize_Pitch = false)
    {
        auto sound = std::make_unique<sf::Sound>(buffer);
        sound->setVolume(volume);

        if (randomize_Pitch)
        {
            float random_Offset = (std::rand() % 40 - 20) / 100.f; // -0.2 to +0.2
            sound->setPitch(pitch + random_Offset);
        }
        else
        {
            sound->setPitch(pitch);
        }

        sound->play();
        _Sounds.push_back(std::move(sound));
    }

    void update()
    {
        _Sounds.erase(
            std::remove_if(_Sounds.begin(), _Sounds.end(), 
                [](const std::unique_ptr<sf::Sound>& s) {return s->getStatus() == sf::Sound::Status::Stopped;}),
            _Sounds.end());
    }

private:
    std::vector<std::unique_ptr<sf::Sound>> _Sounds;
};

