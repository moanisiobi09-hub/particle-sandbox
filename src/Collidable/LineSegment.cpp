#include "LineSegment.hpp"

bool LineSegment::collidesWith(Particle& particle) const
{
    std::pair<float, float> center = {(float)particle.getPosition().x, (float)particle.getPosition().y};

    return pm::circleAndLineCollide(center, particle.RADIUS, {A.x, A.y}, {B.x, B.y});
}