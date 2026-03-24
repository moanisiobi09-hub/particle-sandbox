#include "Particle.hpp"
#include <iostream>

Particle::Particle()
{
    movedThisFrame = false;
    color = sf::Color({(uint8_t)(rand() % 255), (uint8_t)(rand() % 255), (uint8_t)(rand() % 255)});
    velocity = sf::Vector2i({(rand() % 10) - 5, (rand() % 10) - 5});
}

sf::CircleShape Particle::create(sf::Vector2i position) const
{
    sf::CircleShape figure = sf::CircleShape(RADIUS);
    figure.setFillColor(color);

    sf::Vector2f precise = sf::Vector2f({(float)position.x, (float)position.y});
    figure.setPosition(precise);
    
    return figure;
}

void Particle::collideWith(Particle &particle)
{
    sf::Vector2i currentVel = particle.getVelocity();

    currentVel.x += velocity.x;
    currentVel.y += velocity.y;

    particle.setVelocity(currentVel);
}

sf::Vector2i Particle::getVelocity() const
{
    return velocity;
}

void Particle::setVelocity(sf::Vector2i change)
{
    velocity = change;
}

bool Particle::getMovedThisFrame() const
{
    return movedThisFrame;
}

void Particle::setMovedThisFrame(bool change)
{
    movedThisFrame = change;
}