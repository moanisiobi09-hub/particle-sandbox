#include "ParticleArray.hpp"

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
        for (int i = std::max(row - (int)(particle->RADIUS * COLLISON_CHECK_FACTOR), 0); i < std::min(row + (int)(particle->RADIUS * COLLISON_CHECK_FACTOR), (int)screen.size()); i++)
        {
            for (int j = std::max(col - (int)(particle->RADIUS * COLLISON_CHECK_FACTOR), 0); j < std::min(col + (int)(particle->RADIUS * COLLISON_CHECK_FACTOR), (int)screen.size()); j++)
            {
                // if the center of a particle is within these
                // bounds check if the sum of the particles' radii
                // is less than their distance
                if (screen.at(i).at(j) != nullptr)
                {
                    Particle& other = *screen.at(i).at(j);
                    if (&other != &(*particle) && !particle->getCollidedThisFrame() && collides({row, col}, {i, j}))
                    {
                        // run the collide function on the two particles
                        collide(*particle, other);
                        // tell the other particle it has already collided
                        particle->setCollidedThisFrame(true);
                        other.setCollidedThisFrame(true);
                    }
                }
            }
        }
    }
    resetCollisions();
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

void ParticleArray::collide(Particle& dotOne, Particle& dotTwo)
{
    // velocity one an two
    sf::Vector2f v1 = {(float)dotOne.getVelocity().x, (float)dotOne.getVelocity().y}, v2 = {(float)dotTwo.getVelocity().x, (float)dotTwo.getVelocity().y};
    // position one and two
    sf::Vector2f pos1 = {(float)dotOne.getPosition().x, (float)dotOne.getPosition().y}, pos2 = {(float)dotTwo.getPosition().x, (float)dotTwo.getPosition().y};
    // mass 1 and mass 1
    float mass1 = dotOne.getMass(), mass2 = dotTwo.getMass();
    // vector that draws a straight line between the two particles' centers
    sf::Vector2f normalVector = {pos1.x - pos2.x, pos1.y - pos2.y};
    // normal vector with a length of 1
    sf::Vector2f unitNormalVector = normalVector.normalized();
    // find the unit tangent vector (opposite reciprocal)
    sf::Vector2f unitTangentVector = {-unitNormalVector.y, unitNormalVector.x};
    // scalars representing the ball's collision in the normal plane
    float v1Normal = pm::dotProductOf({unitNormalVector.x, unitNormalVector.y}, {v1.x, v1.y}), v1Tangential = pm::dotProductOf({unitTangentVector.x, unitTangentVector.y}, {v1.x, v1.y});
    float v2Normal = pm::dotProductOf({unitNormalVector.x, unitNormalVector.y}, {v2.x, v2.y}), v2Tangential = pm::dotProductOf({unitTangentVector.x, unitTangentVector.y}, {v2.x, v2.y});
    // find the new velocity of each particle on the normal plane (still scalars)
    float v1PrimeScalar = (v1Normal * (mass1 - mass2) + 2 * mass2 * v2Normal) / (mass1 + mass2);
    float v2PrimeScalar = (v2Normal * (mass2 - mass1) + 2 * mass1 * v1Normal) / (mass2 + mass1);
    // find the vectors representing motion normal to the collision after the collision or smth like that
    sf::Vector2f v1NormalPrime = v1PrimeScalar * unitNormalVector, v1TangentialPrime = v1Tangential * unitTangentVector;
    sf::Vector2f v2NormalPrime = v2PrimeScalar * unitNormalVector, v2TangentialPrime = v2Tangential * unitTangentVector;
    // the final velocities in the general 2D environment
    sf::Vector2f v1Prime = v1NormalPrime + v1TangentialPrime, v2Prime = v2NormalPrime + v2TangentialPrime;
    // set the velocities of both particles
    dotOne.setVelocity({(int)v1Prime.x, (int)v1Prime.y});
    dotTwo.setVelocity({(int)v2Prime.x, (int)v2Prime.y});
    // to prevent stickiness, move the particles in their
    // new velocity enough that they are not with in each other
    unstick(dotOne, dotTwo);
}

void ParticleArray::resetCollisions()
{
    // go to every particle and reset it's collided frame
    for (std::shared_ptr<Particle> particle : particlePtrs)
        particle->setCollidedThisFrame(false);
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

void ParticleArray::moveParticle(Particle& particle)
{
    sf::Vector2i velocity = particle.getVelocity();
    sf::Vector2i pos = particle.getPosition();
    // try to move the particle, see if it goes off screen
    try
    {
        screen.at(pos.y + velocity.y).at(pos.x + velocity.x) = screen.at(pos.y).at(pos.x);
        particle.setPosition(sf::Vector2i({pos.x + velocity.x, pos.y + velocity.y}));
    }
    // if it goes out of bounds, delete it
    catch (std::out_of_range) {removeCondemed({particle.getMasterIndex()});}
    // 'delete' the particle unless it's stationary
    if (velocity.x != 0 || velocity.y != 0) screen.at(pos.y).at(pos.x) = nullptr;
    
}

void ParticleArray::unstick(Particle& dotOne, Particle& dotTwo)
{
    // preserve the current grid positions before moving either particle
    sf::Vector2i oldPosOne = dotOne.getPosition();
    sf::Vector2i oldPosTwo = dotTwo.getPosition();
    sf::Vector2f posOneF = {(float)oldPosOne.x, (float)oldPosOne.y};
    sf::Vector2f posTwoF = {(float)oldPosTwo.x, (float)oldPosTwo.y};

    // find how far the two particles overlap
    sf::Vector2f delta = {posOneF.x - posTwoF.x, posOneF.y - posTwoF.y};
    float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);
    float radiusSum = dotOne.RADIUS + dotTwo.RADIUS;
    float overlap = radiusSum - distance;

    // no overlap means nothing to unstick
    if (overlap <= 0.0f)
        return;

    // compute a collision normal; if the centers coincide, fall back on velocity direction
    sf::Vector2f unitNormal;
    if (distance > 0.0f)
    {
        unitNormal = {delta.x / distance, delta.y / distance};
    }
    else
    {
        sf::Vector2i v1 = dotOne.getVelocity();
        sf::Vector2i v2 = dotTwo.getVelocity();
        sf::Vector2f velDelta = {(float)v1.x - (float)v2.x, (float)v1.y - (float)v2.y};
        float velLength = std::sqrt(velDelta.x * velDelta.x + velDelta.y * velDelta.y);
        if (velLength > 0.0f)
            unitNormal = {velDelta.x / velLength, velDelta.y / velLength};
        else
            unitNormal = {1.0f, 0.0f};
    }

    // move each particle half the overlap distance apart, with a small extra offset
    float pushAmount = overlap / 2.0f + 0.5f;
    sf::Vector2f newPosOneF = posOneF + unitNormal * pushAmount;
    sf::Vector2f newPosTwoF = posTwoF - unitNormal * pushAmount;

    // convert the corrected positions back to integer grid coordinates
    sf::Vector2i newPosOne = {
        (int)std::round(newPosOneF.x),
        (int)std::round(newPosOneF.y)
    };
    sf::Vector2i newPosTwo = {
        (int)std::round(newPosTwoF.x),
        (int)std::round(newPosTwoF.y)
    };

    // clamp corrected positions inside the screen bounds
    int maxRow = (int)screen.size() - 1;
    int maxCol = (int)screen.at(0).size() - 1;
    newPosOne.x = std::clamp(newPosOne.x, 0, maxCol);
    newPosOne.y = std::clamp(newPosOne.y, 0, maxRow);
    newPosTwo.x = std::clamp(newPosTwo.x, 0, maxCol);
    newPosTwo.y = std::clamp(newPosTwo.y, 0, maxRow);

    // if both particles still end up at the same grid cell, nudge one of them to separate
    if (newPosOne == newPosTwo)
    {
        if (std::fabs(unitNormal.x) >= std::fabs(unitNormal.y))
        {
            newPosOne.x = std::clamp(newPosOne.x + (unitNormal.x >= 0.0f ? 1 : -1), 0, maxCol);
        }
        else
        {
            newPosOne.y = std::clamp(newPosOne.y + (unitNormal.y >= 0.0f ? 1 : -1), 0, maxRow);
        }

        if (newPosOne == newPosTwo)
        {
            if (newPosOne.x < maxCol)
                newPosOne.x += 1;
            else if (newPosOne.x > 0)
                newPosOne.x -= 1;
            else if (newPosOne.y < maxRow)
                newPosOne.y += 1;
            else if (newPosOne.y > 0)
                newPosOne.y -= 1;
        }
    }

    // preserve shared pointers for grid updates
    std::shared_ptr<Particle> onePtr = screen.at(oldPosOne.y).at(oldPosOne.x);
    std::shared_ptr<Particle> twoPtr = screen.at(oldPosTwo.y).at(oldPosTwo.x);

    // clear old grid cells if the particles moved
    if (oldPosOne != newPosOne && screen.at(oldPosOne.y).at(oldPosOne.x).get() == &dotOne)
        screen.at(oldPosOne.y).at(oldPosOne.x) = nullptr;
    if (oldPosTwo != newPosTwo && screen.at(oldPosTwo.y).at(oldPosTwo.x).get() == &dotTwo)
        screen.at(oldPosTwo.y).at(oldPosTwo.x) = nullptr;

    // place each particle back into its new grid cell
    if (screen.at(newPosOne.y).at(newPosOne.x) == nullptr || screen.at(newPosOne.y).at(newPosOne.x).get() == &dotOne)
        screen.at(newPosOne.y).at(newPosOne.x) = onePtr;
    if (screen.at(newPosTwo.y).at(newPosTwo.x) == nullptr || screen.at(newPosTwo.y).at(newPosTwo.x).get() == &dotTwo)
        screen.at(newPosTwo.y).at(newPosTwo.x) = twoPtr;

    // update the particle positions to the unclipped values
    dotOne.setPosition(newPosOne);
    dotTwo.setPosition(newPosTwo);
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