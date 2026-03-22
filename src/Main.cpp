#include <SFML/Graphics.hpp>
#include "Collidable/Floor.hpp"
#include "ObjectList/ParticleList.hpp"
#include "Config/Constants.hpp"
#include <iostream>
#include <vector>

// check if a particle is colliding with the floor
bool hitFloor(const Particle &particle, const Floor &floor) {
    sf::FloatRect particleBox = particle.getBounds();
    sf::FloatRect floorBox = floor.getBounds();

    if (const std::optional<sf::FloatRect> intersect = particleBox.findIntersection(floorBox)) return true;
    return false;
}

int main()
{
    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode({SCREEN_WIDTH, SCREEN_HEIGHT}), "Particle Sandbox");
    window.setFramerateLimit(60);

    srand(time(0));

    ParticleList particleList;

    Floor floor = Floor();

    while (window.isOpen())
    {
        // get the position of the mouse
        sf::Vector2i mPos = sf::Mouse::getPosition(window);

        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            // handle closing the window
            if (event->is<sf::Event::Closed>()) window.close();
            
            if (const sf::Event::MouseButtonPressed* mousePress = event->getIf<sf::Event::MouseButtonPressed>()) 
            {
                // when the user clicks on the screen, spawn a particle at the mouse position
                if (mousePress->button == sf::Mouse::Button::Left) particleList.addParticleAt(mPos);
                // when the user right-clicks, clear the screen
                if (mousePress->button == sf::Mouse::Button::Right) particleList.clearList();
            }
        }

        window.clear(sf::Color::Black);

        auto &nakedList = particleList.getList();

        for (int i = 0; i < nakedList.size(); i++)
        {
            auto &particle = nakedList.at(i);
            if (hitFloor(particle, floor)) particle.fallOn(floor.getPosition().y);
            else 
            {
                for (int j = i + 1; j < nakedList.size(); j++) 
                {
                    // one particle must be stationary for there to be a collision
                    if (particle.getIsStationary() && particle.hitParticle(nakedList.at(j))) nakedList.at(j).fallOn(particle.getPosition().y - particle.getRadius());
                }
                if (!particle.getIsStationary()) particle.fall();
            }
        }

        particleList.drawParticles(window);

        window.draw(floor);

        window.display();
    }
}