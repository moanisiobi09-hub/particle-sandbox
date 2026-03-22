// A single particle affected by gravity.
#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <SFML/Graphics.hpp>
#include "../Config/Constants.hpp"

const float gravity = 0.5f;

class Particle : public sf::Drawable, public sf::Transformable
{
    private:
        sf::Color color;
        bool isStationary;
        const float radius = 5.0f;
        sf::Vector2f velocity;
        sf::CircleShape create() const; // create a circle to represent this particle
        void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    public:
        Particle(sf::Vector2i position);
        void fall(); // simulate this particle falling.
        sf::FloatRect getBounds() const; // find a square that fits this particle
        void fallOn(float y); // place the ball on top of a y coord
        bool hitParticle(const Particle &other) const; // check if this particle is colliding with another.
        bool getIsStationary() const;
        void setIsStationary(bool change);
        float getRadius() const;
};


#endif