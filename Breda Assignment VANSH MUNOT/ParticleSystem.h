#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
struct Particle {
    sf::Vector2f position;
    sf::Vector2f velocity;
    float lifetime;
    float maxLifetime;
    sf::Color color;
};
class ParticleSystem
{
public:
    void emit(sf::Vector2f position, int count, sf::Color color, float speed, float lifetime)
    {
        for (int i = 0; i < count; i++)
        {
            Particle p;
            p.position = position;
            p.lifetime = lifetime;
            p.maxLifetime = lifetime;
            p.color = color;

            // Random direction
            float angle = (std::rand() % 360) * 3.14f / 180.f;

            float spd = speed * (0.5f + (std::rand() % 100) / 100.f);
            p.velocity = { std::cos(angle) * spd, std::sin(angle) * spd };

            m_particles.push_back(p);
        }
    }

    void update(float dt)
    {
        for (auto& p : m_particles)
        {
            p.lifetime -= dt;
            p.position += p.velocity * dt;
            p.velocity *= 0.98f; // friction

            // Fade out 
            float ratio = p.lifetime / p.maxLifetime;

            p.color.a = static_cast<uint8_t>(255 * ratio);

            
        }

        
        // Remove dead particles
        m_particles.erase(std::remove_if(m_particles.begin(), m_particles.end(),[](const Particle& p) { return p.lifetime <= 0.f; }),m_particles.end());
    }

    void render(sf::RenderTarget& target)
    {
        for (auto& p : m_particles)
        {
            sf::CircleShape circle(2.f);
            circle.setFillColor(p.color);
            circle.setPosition(p.position);
            target.draw(circle);
        }
    }

private:
    std::vector<Particle> m_particles;
};

