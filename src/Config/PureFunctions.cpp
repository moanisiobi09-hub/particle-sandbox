#include "PureFunctions.hpp"

namespace pm
{
    int findXOnLine(std::pair<float, float> pointA, std::pair<float, float> pointB, float y)
    {
        // handle vertical and horizontal lines by returning the pointA's x value
        if (pointA.first == pointB.first || pointA.second == pointB.second) return pointA.first;
        // find the slope of the line segment
        float slope = (pointA.second - pointB.second) / (pointA.first - pointB.first);
        // use that slope to determine what the x must be
        float result = (y - pointA.second) / slope + pointA.first;
        return (int)result;
    }
}