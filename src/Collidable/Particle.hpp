// A single particle affected by gravity.
#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "../Config/Constants.hpp"

class Particle
{
    private:
        int masterIndex; // the index in the master vector of ParticleArray that this obj is in
        sf::Color color;
        sf::Vector2i velocity;
        sf::Vector2i position; // the index of the ParticleArray that this occupies.
    public:
        static constexpr float RADIUS = 20.0f;
        Particle(int index);
        sf::CircleShape create(sf::Vector2i position) const; // create a circle to represent this particle
        void collideWith(Particle &particle); // edit the velocity of another particle to simulate a collision
        sf::Vector2i getVelocity() const;
        void setVelocity(sf::Vector2i change);
        int getMasterIndex() const;
        void setMasterIndex(int change);
        sf::Vector2i getPosition() const; // returns the position of this particle on screen IN {X, Y} FORMAT
        void setPosition(sf::Vector2i change);
        void push(sf::Vector2i force); // add the force vector to the current velocity
};


#endif