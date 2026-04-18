#include "PureFunctions.hpp"

namespace pm
{
    float findXOnLine(std::pair<float, float> pointA, std::pair<float, float> pointB, float y)
    {
        // handle vertical lines by returning the A x coord
        if (pointA.first == pointB.first) return pointA.first;
        // find the slope of the line segment
        float slope = (pointA.second - pointB.second) / (pointA.first - pointB.first);
        // if the slope is 0 (horizontal line), use point A's x value
        if (slope == 0) return pointA.first;
        // use that slope to determine what the x must be
        float result = (y - pointA.second) / slope + pointA.first;
        return result;
    }
}