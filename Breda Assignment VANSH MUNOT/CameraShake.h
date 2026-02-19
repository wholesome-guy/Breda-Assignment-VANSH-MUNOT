#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
class CameraShake
{
public:
    void trigger(float duration, float magnitude)
    {
        _Duration = duration;
        _Elapsed = 0.f;
        _Magnitude = magnitude;
    }

    sf::Vector2f update(float dt)
    {
        if (_Elapsed >= _Duration)
            return { 0.f, 0.f };

        _Elapsed += dt;

        // Ease out: shake fades over time
        float progress = 1.f - (_Elapsed / _Duration);
        float strength = _Magnitude * progress;

        float offsetX = ((std::rand() % 200 - 100) / 100.f) * strength;
        float offsetY = ((std::rand() % 200 - 100) / 100.f) * strength;

        return { offsetX, offsetY };
    }

private:
    float _Duration = 0.f;
    float _Elapsed = 0.f;
    float _Magnitude = 0.f;
};


