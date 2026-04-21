// The FanList class is in charge of handling the addition, initialization and deletion of Fan object during runtime
#ifndef FAN_LIST_HPP
#define FAN_LIST_HPP

#include "../ForceSources/Fan.hpp"
#include <vector>

class FanList
{
    public:
        std::vector<Fan> listOfFans;
        void addFan(sf::Vector2i mPos); // add a fan at the mouse position or complete the last one added
        void popFan(); // remove the last fan added
        void drawFans(sf::RenderTarget &target) const;
        int applyForce(ParticleArray &particleArray) const; // apply the forces from all of the fans. return the number of particles affected
        bool empty() const; // return whether or not there are no fans
};

#endif