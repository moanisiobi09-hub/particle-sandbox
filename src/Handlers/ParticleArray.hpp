// A particle array structure is a 2D array with an index for every screen pixel. It is used to more efficiently detect collisions and move particles.

#ifndef PARTICLE_ARRAY_HPP
#define PARTICLE_ARRAY_HPP

#include "../Config/Constants.hpp"
#include "../Collidable/Particle.hpp"
#include "../Config/PureFunctions.hpp"
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
        void checkCollisions(); // detect collision between particles and call the collide function
        bool collides(sf::Vector2i pos1, sf::Vector2i pos2) const; // detect if two circles are colliding
        void collide(Particle& dotOne, Particle& dotTwo); // edit the velocities of two particles to simulate an elastic collision
        void resetCollisions(); // reset the collidedThisFrame switch in each of the particles
        void moveParticles(); // move each particle one step according to its velocity.
        void moveParticle(Particle& particle); // move a single particle according to its velocity
        void unstick(Particle& dotOne, Particle& dotTwo); // move two particles enough to not be inside one another
        void removeCondemed(std::vector<int> condemned); // delete particles that moved off the screen
        bool addParticle(sf::Vector2i mPos); // add a particle if there is space at that position. Return whether or not a change was made
        int getParticleCount() const;
};

#endif