// A single particle affected by gravity.
#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <SFML/Graphics.hpp>
#include "../Config/Constants.hpp"

class Particle
{
    private:
        sf::Color color;
        sf::Vector2i velocity;
        bool movedThisFrame;
    public:
        static constexpr float RADIUS = 20.0f;
        Particle();
        sf::CircleShape create(sf::Vector2i position) const; // create a circle to represent this particle
        void collideWith(Particle &particle); // edit the velocity of another particle to simulate a collision
        sf::Vector2i getVelocity() const;
        void setVelocity(sf::Vector2i change);
        bool getMovedThisFrame() const;
        void setMovedThisFrame(bool change);
};


#endif