#include "ParticleArray.hpp"
#include <iostream>

ParticleArray::ParticleArray() : screen(SCREEN_HEIGHT, std::vector<std::optional<Particle>>(SCREEN_WIDTH))
{

}

void ParticleArray::drawParticles(sf::RenderTarget &target) const
{
    for (int row = 0; row < screen.size(); row++)
    {
        for (int col = 0; col < screen.at(row).size(); col++)
        {
            if (screen.at(row).at(col).has_value())
            {
                const Particle &particle = screen.at(row).at(col).value();
                sf::CircleShape figure = particle.create(sf::Vector2i({col, row}));
                target.draw(figure);
            }
        }
    }
}

void ParticleArray::checkCollisions()
{
    // look through every particle on screen and check if it is
    // colliding with any other particle in its local region.
    // If it is, run the collide method on that particle
    for (int row = 0; row < screen.size(); row++)
    {
        for (int col = 0; col < screen.at(row).size(); col++)
        {
            if (std::optional<Particle> &particle = screen.at(row).at(col))
            {
                // check a square with length of the
                // diameter of this particle (respect bounds)
                for (int i = std::max(row - (int)(*particle).RADIUS, 0); i < std::min(row + (int)(*particle).RADIUS, (int)screen.size()); i++)
                {
                    for (int j = std::max(col - (int)(*particle).RADIUS, 0); j < std::min(col + (int)(*particle).RADIUS, (int)screen.size()); j++)
                    {
                        // if there is a particle here, there must be a collision.
                        if (std::optional<Particle> &other = screen.at(i).at(j))
                        {
                            if (&other != &particle)
                            {
                                (*particle).collideWith((*other));
                                std::cerr << "THERE HAS BEEN A COLLISION" << std::endl;
                            }
                        }
                    }
                }
            }
        }
    }
}

void ParticleArray::moveParticles()
{
    for (int row = 0; row < screen.size(); row++)
    {
        for (int col = 0; col < screen.at(row).size(); col++)
        {
            if (std::optional<Particle> particle = screen.at(row).at(col))
            {
                if (!(*particle).getMovedThisFrame())
                {
                    (*particle).setMovedThisFrame(true);
                    // if there is a particle at this position and it
                    // has not moved this frame try to move it to a
                    // new index according to its velocity. If this
                    // new position is out of bounds, allow the
                    // particle to vanish from existence.
                    sf::Vector2i velocity = (*particle).getVelocity();
                    try {screen.at(row + velocity.y).at(col + velocity.x) = particle;}
                    catch (std::out_of_range) {}
                    screen.at(row).at(col) = std::nullopt;
                }
            }
        }
    }
    resetMovedFlag();
}

bool ParticleArray::addParticle(sf::Vector2i mPos)
{
    screen.at(mPos.y).at(mPos.x).emplace(Particle());
    return true;
}

int ParticleArray::getParticleCount() const
{
    int count = 0;
    for (auto row : screen)
    {
        for (auto particle : row)
        {
            if (particle.has_value()) count++;
        }
    }
    return count;
}

void ParticleArray::resetMovedFlag()
{
    for (auto &row : screen)
    {
        for (auto &particle : row)
        {
            if (particle.has_value()) (*particle).setMovedThisFrame(false);
        }
    }
}