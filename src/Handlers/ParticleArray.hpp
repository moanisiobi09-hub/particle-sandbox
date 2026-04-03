// A particle array structure is a 2D array with an index for every screen pixel. It is used to more efficiently detect collisions and move particles.

#ifndef PARTICLE_ARRAY_HPP
#define PARTICLE_ARRAY_HPP

#include "../Config/Constants.hpp"
#include "../Collidable/Particle.hpp"
#include <memory>
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>

class ParticleArray
{
    private:
        std::vector<std::shared_ptr<Particle>> particlePtrs;
    public:
        std::vector<std::vector<std::shared_ptr<Particle>>> screen;
        ParticleArray();
        void drawParticles(sf::RenderTarget &target) const; // take all the particles as they are currently stored and draw them to the screen
        void checkCollisions(); // find any collisions that will occur given the current state and call each particle's collide function.
        bool collides(sf::Vector2i pos1, sf::Vector2i pos2) const; // use the distance and radii of two circles to determine if they collide
        void moveParticles(); // move each particle one step according to its velocity.
        void removeCondemed(std::vector<int> condemned); // delete particles that moved off the screen
        bool addParticle(sf::Vector2i mPos); // add a particle if there is space at that position. Return whether or not a change was made
        int getParticleCount() const;
};

#endif