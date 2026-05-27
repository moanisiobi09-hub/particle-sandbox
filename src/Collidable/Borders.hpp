// The Borders manages 4 invisible, stationary line segments that stay at the edge of the screen and have infinite mass.
#include "LineSegment.hpp"
#include "../Handlers/ParticleArray.hpp"
#include <iostream>

class Borders
{
    private:
        const int WALL_CHECK_THICKNESS = 50; // how far from the wall to check for collisions

        enum class Wall
        {
            Left, Right, Top, Bottom, None
        };

        LineSegment leftWall;
        LineSegment rightWall;
        LineSegment topWall;
        LineSegment bottomWall;
    public:
        Borders();
        int applyForce(ParticleArray& array); // apply force to particles in proximity of the borders. return the number affected
        Wall collidesWithAny(Particle& particle) const; // check if a particle collides with any of the walls, return that wall
};