// A fan object is a line that applies a force every frame to particles in its line of attack. Also included is the arrow class used to
// help draw this figure
#ifndef FAN_HPP
#define FAN_HPP

#include <SFML\Graphics.hpp>
#include "..\Handlers\ParticleArray.hpp"
#include "..\Config\PureFunctions.hpp"
#include <cmath>
#include <tuple>

class Arrow : public sf::Drawable, public sf::Transformable
{
    private:
        float magnitude;
        std::tuple<sf::RectangleShape, sf::ConvexShape> create() const;
        void draw(sf::RenderTarget &target, sf::RenderStates state) const override;
    public:
        Arrow(float magnitude);
};

class Fan : public sf::Drawable, public sf::Transformable
{
    private:
        sf::VertexArray DEBUG_points;
        float magnitude;
        bool showMarker; // whether or not the 'marker' is shown. to be active before knob2's position is defined
        static constexpr float KNOB_RADIUS = 10.f;
        sf::Vector2f knob1Pos, knob2Pos; // position of the first and second 'knob'
        sf::Vector2f knob1Ray, knob2Ray; // implied position of the other points in the rectangle of effect
        sf::Vector2i forceVector; // the force to be applied to particles in this region
        float length; // the total length of this figure
        sf::Angle rotation; // how much is this figure rotated about the center of knob1
        std::tuple<sf::CircleShape, sf::CircleShape, sf::RectangleShape, std::vector<Arrow>> create() const;
        sf::VertexArray createMarker() const;
        void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
        // find the x-value that the line between pointA and pointB would intersect at y
        float findLineX(sf::Vector2f pointA, sf::Vector2f pointB, float y) const;
        sf::Vector2i findForceVector(); // use the two knobs' positions to find the perpendicular force vector, set the magnitude
    public:
        void init(sf::Vector2i knob2Pos); // fully initialize the fan with the second position
        Fan(sf::Vector2i knob1Pos);
        bool getShowMarker() const;
        // apply this fan's force vector to all the particles in its region. return how many particles were affected
        int applyForce(ParticleArray &array) const;
};

#endif