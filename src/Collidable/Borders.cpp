#include "Borders.hpp"

Borders::Borders()
{
    leftWall.A = {0, 600}, leftWall.B = {0, 0};

    topWall.A = {0, 0}, topWall.B = {800, 0};

    rightWall.A = {800, 0}, rightWall.B = {800, 600};

    bottomWall.A = {800, 600}, bottomWall.B = {0, 600};
}

int Borders::applyForce(ParticleArray& array)
{
    int count = 0;
    
    for (int i = 0; i < array.screen.size(); i++)
    {
        for (int j = 0; j < array.screen.at(i).size(); j++)
        {
            bool farFromTop = i > WALL_CHECK_THICKNESS, farFromBottom = i < array.screen.size() - WALL_CHECK_THICKNESS;
            bool farFromLeft = j > WALL_CHECK_THICKNESS, farFromRight = j < array.screen.at(i).size() - WALL_CHECK_THICKNESS;

            if (farFromTop && farFromBottom && farFromLeft && farFromRight) continue;

            if (array.screen.at(i).at(j) != nullptr)
            {
                Particle& particle = *(array.screen.at(i).at(j));
                auto which = collidesWithAny(particle);
                sf::Vector2i vel = particle.getVelocity();

                if (which == Wall::Left || which == Wall::Right)
                {
                    particle.setVelocity({-vel.x, vel.y});
                    count++;
                }
                else if (which == Wall::Top || which == Wall::Bottom)
                {
                    particle.setVelocity({vel.x, -vel.y});
                    count++;
                }
            }
        }
    }
    return count;
}

Borders::Wall Borders::collidesWithAny(Particle& particle) const
{
    if (leftWall.collidesWith(particle)) return Wall::Left;
    if (topWall.collidesWith(particle)) return Wall::Top;
    if (rightWall.collidesWith(particle)) return Wall::Right;
    if (bottomWall.collidesWith(particle)) return Wall::Bottom;
    else return Wall::None;
}