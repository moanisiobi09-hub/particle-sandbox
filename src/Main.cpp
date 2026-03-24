#include <SFML/Graphics.hpp>
#include "Handlers/ParticleArray.hpp"
#include "Config/Constants.hpp"
#include <iostream>

int main()
{
    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode({SCREEN_WIDTH, SCREEN_HEIGHT}), "Particle Sandbox");
    window.setFramerateLimit(60);

    srand(time(0));

    ParticleArray particleArray;

    while (window.isOpen())
    {
        // get the mouse's position
        sf::Vector2i mPos = sf::Mouse::getPosition(window);

        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            // handle closing the window
            if (event->is<sf::Event::Closed>()) window.close();
            
            if (const sf::Event::MouseButtonPressed* mousePress = event->getIf<sf::Event::MouseButtonPressed>()) 
            {
                // when the user clicks on the screen, spawn a particle at the mouse position
                if (mousePress->button == sf::Mouse::Button::Left) particleArray.addParticle(mPos);
            }
        }

        window.clear(sf::Color::Black);

        particleArray.checkCollisions();

        particleArray.moveParticles();

        particleArray.drawParticles(window);

        window.display();
    }

    return 0;
}