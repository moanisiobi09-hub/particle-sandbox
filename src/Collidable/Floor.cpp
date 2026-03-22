#include "Floor.hpp"

sf::RectangleShape Floor::create() const
{
    sf::RectangleShape figure = sf::RectangleShape({WIDTH, HEIGHT});
    figure.setFillColor(sf::Color::Magenta);
    return figure;
}

Floor::Floor()
{
    setPosition({(1.0 / 6) * SCREEN_WIDTH, SCREEN_HEIGHT - HEIGHT});
}

void Floor::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::RectangleShape figure = create();
    states.transform = getTransform();
    target.draw(figure, states);
}

sf::FloatRect Floor::getBounds() const
{
    return sf::FloatRect({(1.0 / 6) * SCREEN_WIDTH, SCREEN_HEIGHT - HEIGHT}, {WIDTH, HEIGHT});
}