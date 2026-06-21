#include "Particle.hpp"
#include <iostream>

Particle::Particle(int index)
{
    masterIndex = index;
    collidedThisFrame = false;
    color = sf::Color({(uint8_t)(rand() % 255), (uint8_t)(rand() % 255), (uint8_t)(rand() % 255)});
    velocity = sf::Vector2i({(rand() % 10) - 5, (rand() % 10) - 5});
    mass = 30;
}

sf::CircleShape Particle::create(sf::Vector2i position) const
{
    sf::CircleShape figure = sf::CircleShape(RADIUS);
    figure.setFillColor(color);

    sf::Vector2f precise = sf::Vector2f({(float)position.x, (float)position.y});
    figure.setPosition(precise);
    figure.setOrigin(figure.getGeometricCenter());
    
    return figure;
}

sf::Vector2i Particle::getVelocity() const
{
    return velocity;
}

void Particle::setVelocity(sf::Vector2i change)
{
    velocity = change;
}

int Particle::getMasterIndex() const
{
    return masterIndex;
}

void Particle::setMasterIndex(int change)
{
    masterIndex = change;
}

sf::Vector2i Particle::getPosition() const
{
    return position;
}

void Particle::setPosition(sf::Vector2i change)
{
    position = change;
}

void Particle::push(sf::Vector2i force)
{
    velocity.x += force.x;
    velocity.y += force.y;
}

bool Particle::getCollidedThisFrame() const
{
    return collidedThisFrame;
}

void Particle::setCollidedThisFrame(bool change)
{
    collidedThisFrame = change;
}

float Particle::getMass() const
{
    return mass;
}

void Particle::setMass(float change)
{
    mass = change;
}