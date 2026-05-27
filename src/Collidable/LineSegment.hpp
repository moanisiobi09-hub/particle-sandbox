// A line segment represents any flat surface that can be collided with and/or moved around on screen.
#include <SFML\Graphics.hpp>
#include "../Handlers/ParticleArray.hpp"
#include "../Config/PureFunctions.hpp"

class LineSegment : public sf::Transformable
{
    public:
        sf::Vector2f A;
        sf::Vector2f B;

        bool collidesWith(Particle& particle) const; // check for collisions between this and a particle
};