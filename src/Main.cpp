#include <SFML/Graphics.hpp>
#include "Handlers/ParticleArray.hpp"
#include "Collidable/Fan.hpp"
#include "Config/Constants.hpp"
#include <iostream>

int main()
{
    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode({SCREEN_WIDTH, SCREEN_HEIGHT}), "Particle Sandbox");
    window.setFramerateLimit(60);

    srand(time(0));

    std::vector<Fan> allFans;

    ParticleArray particleArray;

    while (window.isOpen())
    {
        // get the mouse's position
        sf::Vector2i mPos = sf::Mouse::getPosition(window);

        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            // handle closing the window
            if (event->is<sf::Event::Closed>()) window.close();
            // detect mouse presses
            if (const sf::Event::MouseButtonPressed* mousePress = event->getIf<sf::Event::MouseButtonPressed>()) 
            {
                // when the user right-clicks on the screen, spawn a wind emitter
                if (mousePress->button == sf::Mouse::Button::Right)
                {
                    if (allFans.size() == 0 || !allFans.back().getShowMarker()) allFans.push_back(Fan(mPos)); // if the last fan is fully initialized, add another one
                    else allFans.back().init(mPos); // otherwise, use the mouse position to initialize the last one added
                }
                // when the user left-clicks on the screen, spawn a particle
                if (mousePress->button == sf::Mouse::Button::Left) particleArray.addParticle(mPos); 
            }
            // detect key presses
            if (const sf::Event::KeyPressed* keyPress = event->getIf<sf::Event::KeyPressed>())
            {
                // when the user presses ctrl + z remove the last placed wind emitter
                if (keyPress->code == sf::Keyboard::Key::Z && keyPress->control && !allFans.empty()) allFans.pop_back(); 
            }
        }

        window.clear(sf::Color::Black);

        //particleArray.checkCollisions();

        particleArray.moveParticles();

        particleArray.drawParticles(window);

        for (Fan& fan : allFans) 
        {
            fan.applyForce(particleArray);
            window.draw(fan);
        }

        window.display();
    }

    return 0;
}