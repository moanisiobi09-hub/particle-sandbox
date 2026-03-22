#include "ParticleList.hpp"

bool ParticleList::checkAllCollisions(const Particle &particle) const
{
    for (const Particle &other : list)
    {
        // if the parameter particle is hitting a different particle, return true
        if (&particle != &other && particle.hitParticle(other)) return true;
    }
    return false;
}

bool ParticleList::addParticleAt(sf::Vector2i position)
{
    Particle prospective = Particle(position);
    if (checkAllCollisions(prospective)) return false;
    list.push_back(prospective);
    return true;
}

void ParticleList::drawParticles(sf::RenderTarget &target) const
{
    for (const Particle &particle : list) target.draw(particle);
}

void ParticleList::clearList()
{
    list.clear();
}

std::vector<Particle>& ParticleList::getList()
{
    return list;
}