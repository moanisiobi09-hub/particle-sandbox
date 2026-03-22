// A simple collidable rectangle to represent the floor
#ifndef FLOOR_HPP
#define FLOOR_HPP

#include <SFML/Graphics.hpp>
#include "../Config/Constants.hpp"

class Floor : public sf::Drawable, public sf::Transformable 
{
    private:
        const float WIDTH = (2.0 / 3) * SCREEN_WIDTH, HEIGHT = 40;
        sf::RectangleShape create() const; // create the figure that is drawn on screen
        void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    public:
        Floor();
        sf::FloatRect getBounds() const; // get the bounding box
};

#endif