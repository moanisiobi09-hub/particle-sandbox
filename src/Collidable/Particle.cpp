#include "Particle.hpp"

sf::CircleShape Particle::create() const
{
    sf::CircleShape figure(radius);
    figure.setOrigin(figure.getGeometricCenter());
    figure.setFillColor(color);
    return figure;
}

void Particle::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::CircleShape figure = create();
    states.transform = getTransform();
    target.draw(figure, states);
}

Particle::Particle(sf::Vector2i position)
{
    isStationary = false;
    velocity = sf::Vector2f({((float)(rand() % 100)) / 10 - 5, 0.0f});
    color = sf::Color({(uint8_t)(rand() % 255), (uint8_t)(rand() % 255), (uint8_t)(rand() % 255)});
    setPosition({(float)position.x, (float)position.y});
}

void Particle::fall()
{
    // add to the velocity (accelerate)
    velocity.y += gravity;
    // add to the position
    move({velocity.x, velocity.y});
}

sf::FloatRect Particle::getBounds() const
{
    float x = getPosition().x, y = getPosition().y;
    sf::FloatRect box = sf::FloatRect({x - radius, y - radius}, {2 * radius, 2 * radius});
    return box;
}

void Particle::fallOn(float y)
{
    setPosition({getPosition().x, y - radius});
    isStationary = true;
}

bool Particle::hitParticle(const Particle &other) const
{
    sf::FloatRect box = getBounds();
    sf::FloatRect otherBox = other.getBounds();
    if (const std::optional<sf::FloatRect> intersect = box.findIntersection(otherBox)) return true;
    return false;
}

bool Particle::getIsStationary() const
{
    return isStationary;
}

void Particle::setIsStationary(bool change)
{
    isStationary = change;
}

float Particle::getRadius() const
{
    return radius;
}