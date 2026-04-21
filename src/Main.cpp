#include <SFML/Graphics.hpp>
#include "Handlers/ParticleArray.hpp"
#include "Handlers/FanList.hpp"
#include "Config/FrameCounter.hpp"
#include "Config/Constants.hpp"
#include <iostream>

int main()
{
    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode({SCREEN_WIDTH, SCREEN_HEIGHT}), "Particle Sandbox");
    window.setFramerateLimit(70);

    srand(time(0));

    FrameCounter fpsTracker;

    FanList fanList;

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
                if (mousePress->button == sf::Mouse::Button::Right) fanList.addFan(mPos);
                // when the user left-clicks on the screen, spawn a particle
                if (mousePress->button == sf::Mouse::Button::Left) particleArray.addParticle(mPos); 
            }
            // detect key presses
            if (const sf::Event::KeyPressed* keyPress = event->getIf<sf::Event::KeyPressed>())
            {
                // when the user presses ctrl + z remove the last placed wind emitter
                if (keyPress->code == sf::Keyboard::Key::Z && keyPress->control) fanList.popFan();
            }
        }

        window.clear(sf::Color::Black);

        //particleArray.checkCollisions();

        if (!fanList.empty()) fanList.listOfFans.at(0).DEBUG_stuff(window, particleArray);

        particleArray.moveParticles();

        particleArray.drawParticles(window);

        fanList.applyForce(particleArray);

        fanList.drawFans(window);

        fpsTracker.addFrame();
        window.draw(fpsTracker);

        window.display();
    }
    /* std::pair<float, float> pointA = {400 + 10, 300};
    std::pair<float, float> pointB = {400 - 10, 300};
    for (int i = 0; i <= 300; i++) {
        pointA.second ++;
        pointB.second --;
        for (int y = 0; y <= SCREEN_HEIGHT; y++) {
            if (pm::findXOnLine(pointA, pointB, y) >= 800 || pm::findXOnLine(pointA, pointB, y) < 0) std::cerr << std::format("({}, {}) + ({}, {}) + {} -> {}", pointA.first, pointA.second, pointB.first, pointB.second, y, pm::findXOnLine(pointA, pointB, y)) << "\n";
        }
    } */

    return 0;
}