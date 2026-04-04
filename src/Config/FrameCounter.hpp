// The frame counter is a simple object that keeps track of how long each frame takes to complete in the game loop
#ifndef FRAME_COUNTER_HPP
#define FRAME_COUNTER_HPP

#include <SFML\System.hpp>
#include <SFML\Graphics.hpp>

class FrameCounter : public sf::Drawable, public sf::Transformable
{
    private:
        sf::Clock clock;
        sf::Font liberationSans;
        int frameCount;
        float fps;
        sf::Text create() const;
        void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    public:
        FrameCounter();
        void addFrame(); // take the time elapsed in this frame, add to the number of frames
};

#endif