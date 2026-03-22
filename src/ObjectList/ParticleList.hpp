// a structure to manage all particle simultaneously drawn on screen.
// - add and remove particles
// - have a method to draw all of the particles
// - detect collisions between particles
#ifndef PARTICLE_LIST_HPP
#define PARTICLE_LIST_HPP

#include "../Collidable/Particle.hpp"
#include <vector>

class ParticleList
{
    private:
        std::vector<Particle> list;
    public:
        bool checkAllCollisions(const Particle &particle) const; // check if a given particle is colliding with any other particle
        bool addParticleAt(sf::Vector2i position); // add a particle if there is open space, return whether or not it was added.
        void drawParticles(sf::RenderTarget &target) const; // draw every particle in the list
        void clearList();
        std::vector<Particle>& getList(); // return a reference to the list of particles
}; 

#endif