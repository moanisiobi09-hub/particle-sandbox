// The pure functions folder holds useful math that doesn't depend on any particular class. This is my favorite file to write
#ifndef PURE_FUNCTIONS_HPP
#define PURE_FUNCTIONS_HPP

#include <utility>
#include <algorithm>
#include <cmath>
#include <functional>

// pure math functions used for collision detection, force application, and other simulation purposes
namespace pm
{
    // given two points to form a line and a y-value on that line, find the corresponding x-value
    int findXOnLine(std::pair<float, float> pointA, std::pair<float, float> pointB, float y);
}

#endif