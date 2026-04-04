#include "FrameCounter.hpp"

sf::Text FrameCounter::create() const
{
    // create a text label with the time
    sf::Text figure = sf::Text(liberationSans);
    figure.setString(std::format("FPS: {:.2f}", fps));
    figure.setCharacterSize(20);
    // return it
    return figure;
}

void FrameCounter::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform = getTransform();
    auto figure = create();

    target.draw(figure, states);
}

FrameCounter::FrameCounter() : liberationSans()
{
    frameCount = 0;
    fps = 60.f;
    // load the font
    if (!liberationSans.openFromFile("external/liberation-sans/LiberationSans-Regular.ttf")) std::exit(-67);
}

void FrameCounter::addFrame()
{
    frameCount++;
    // after 10 frames have elapsed, update the fps
    if (frameCount > 10)
    {
        frameCount = 0; // reset frames
        sf::Time elapsedTime = clock.restart();
        fps = 10.f / elapsedTime.asSeconds();
    }
}