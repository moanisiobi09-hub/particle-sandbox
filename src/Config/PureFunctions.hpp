// The pure functions folder holds useful math that doesn't depend on any particular class. This is my favorite file to write
#ifndef PURE_FUNCTIONS_HPP
#define PURE_FUNCTIONS_HPP

#include <utility>
#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <format>

// pure math functions used for collision detection, force application, and other simulation purposes
namespace pm
{
    // given two points to form a line and a y-value on that line, find the corresponding x-value
    int findXOnLine(std::pair<float, float> pointA, std::pair<float, float> pointB, float y);

    // given a circle (defined as center and radius) and a line segment (defined as two points) return if the line overlaps the circle
    bool circleAndLineCollide(std::pair<float, float> center, float radius, std::pair<float, float> pointA, std::pair<float, float> pointB);
}

#endif