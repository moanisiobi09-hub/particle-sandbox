#include "FanList.hpp"

void FanList::addFan(sf::Vector2i mPos)
{
    // if the last fan to be added has been initialized (or there was no
    // previous fan) add a new one at the mouse position
    if (listOfFans.empty() || !listOfFans.back().getShowMarker()) listOfFans.push_back(Fan(mPos));
    // otherwise, use the mouse position to initialize the last fan added
    else listOfFans.back().init(mPos);
}

void FanList::popFan()
{
    // if there are fans, remove the last one added
    if (!listOfFans.empty()) listOfFans.pop_back();
}

void FanList::drawFans(sf::RenderTarget &target) const
{
    for (const Fan &fan : listOfFans) target.draw(fan);
}

int FanList::applyForce(ParticleArray &particleArray) const
{
    int affectedCount = 0; // the total particles pushed by all fans this frame
    for (const Fan &fan : listOfFans) 
    {
        affectedCount += fan.applyForce(particleArray);
    }
    return affectedCount;
}

bool FanList::empty() const
{
    return listOfFans.empty();
}