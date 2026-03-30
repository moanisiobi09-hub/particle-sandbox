#include "ParticleArray.hpp"
#include <iostream>

ParticleArray::ParticleArray() : screen(SCREEN_HEIGHT, std::vector<std::shared_ptr<Particle>>(SCREEN_WIDTH, nullptr))
{

}

void ParticleArray::drawParticles(sf::RenderTarget &target) const
{
    for (std::shared_ptr<Particle> particle : particlePtrs)
    {
        sf::CircleShape figure = particle->create(sf::Vector2i(particle->getPosition()));
        target.draw(figure);
    }
}

void ParticleArray::checkCollisions()
{
    // go through each particle and check its local region
    // if there is a particle colliding with this one, run
    // the collide function
    for (std::shared_ptr<Particle> particle : particlePtrs)
    {
        int row = particle->getPosition().y, col = particle->getPosition().x;
        // check the a square that includes all points of the circle
        // for other particles
        for (int i = std::max(row - (int)(particle->RADIUS * 3), 0); i < std::min(row + (int)(particle->RADIUS * 3), (int)screen.size()); i++)
        {
            for (int j = std::max(col - (int)(particle->RADIUS * 3), 0); j < std::min(col + (int)(particle->RADIUS * 3), (int)screen.size()); j++)
            {
                // if the center of a particle is within these
                // bounds check if the sum of the particles' radii
                // is less than their distance
                if (screen.at(i).at(j) != nullptr)
                {
                    Particle& other = *screen.at(i).at(j);
                    if (&other != &(*particle) && collides({row, col}, {i, j})) particle->collideWith(other);
                }
            }
        }
    }
}

bool ParticleArray::collides(sf::Vector2i pos1, sf::Vector2i pos2) const
{
    const Particle &first = *screen.at(pos1.x).at(pos1.y);
    const Particle &second = *screen.at(pos2.x).at(pos2.y);

    int dx = pos1.x - pos2.x;
    int dy = pos1.y - pos2.y;
    int radiusSum = first.RADIUS + second.RADIUS;

    return (dx * dx + dy * dy) < (radiusSum * radiusSum);
}

void ParticleArray::moveParticles()
{
    std::vector<int> condemnedList;
    // move each particle to the right row and column depending on its velocity
    for (std::shared_ptr<Particle> particle : particlePtrs)
    {
        sf::Vector2i velocity = particle->getVelocity();
        sf::Vector2i pos = particle->getPosition();
        // attempt to move the particle according to its velocity (and update position member)
        try
        {
            screen.at(pos.y + velocity.y).at(pos.x + velocity.x) = screen.at(pos.y).at(pos.x);
            particle->setPosition(sf::Vector2i({pos.x + velocity.x, pos.y + velocity.y}));
        }
        // if the particle goes out of range, mark it for deletion
        catch (std::out_of_range) {condemnedList.push_back(particle->getMasterIndex());}
        // if the particle is not stationary, 'delete' it by voiding its ptr
        if (!(velocity.x == 0 && velocity.y == 0)) screen.at(pos.y).at(pos.x) = nullptr;
    }
    removeCondemed(condemnedList);
}

void ParticleArray::removeCondemed(std::vector<int> condemned)
{
    // remove particles using 'swap and pop' to preserve the validity
    // of indexes stored inside particle objects
    for (int i = 0; i < condemned.size(); i++)
    {
        int index = condemned.at(i);
        std::swap(particlePtrs.at(index), particlePtrs.back());
        particlePtrs.at(index)->setMasterIndex(index); // update the index of the particle that was moved
        particlePtrs.pop_back();
    }
}

bool ParticleArray::addParticle(sf::Vector2i mPos)
{
    // make sure there isn't already a particle at this position
    if (screen.at(mPos.y).at(mPos.x) == nullptr)
    {
        // add this particle to the master, telling it where it is in the master list
        particlePtrs.push_back(std::make_shared<Particle>(particlePtrs.size()));
        // add the particle to the screen
        screen.at(mPos.y).at(mPos.x) = particlePtrs.back();
        // let the particle know its position on the grid
        particlePtrs.back()->setPosition(sf::Vector2i({mPos.x, mPos.y}));
        return true;
    }
    return false;
}

int ParticleArray::getParticleCount() const
{
    // return the size of the master
    return particlePtrs.size();
}