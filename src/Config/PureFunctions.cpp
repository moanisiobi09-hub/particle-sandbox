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

    bool circleAndLineCollide(std::pair<float, float> center, float radius, std::pair<float, float> pointA, std::pair<float, float> pointB)
    {
        // if the distance between the center and both endpoints is greater than
        // the sum of the radius and the length of the line segment, the two figures
        // are definitely not colliding
        float sum = radius + std::hypot(pointA.first - pointB.first, pointA.second - pointB.second);
        float ADistSquared = std::pow(center.first - pointA.first, 2) + std::pow(center.second - pointA.second, 2);
        float BDistSquared = std::pow(center.first - pointB.first, 2) + std::pow(center.second - pointB.second, 2);

        if (std::sqrt(ADistSquared) > sum && std::sqrt(BDistSquared) > sum) return false;
        
        // if the distance between either of the line segment's endpoints and
        // the center is less than the radius there is a collision
        bool AisTooClose = ADistSquared < radius * radius;
        bool BisTooClose = BDistSquared < radius * radius;

        if (AisTooClose || BisTooClose) return true;

        // find the slope while handling horizontal and vertical lines
        float m;
        if (pointA.second == pointB.second) m = 1e-5; // horizontal
        else if (pointA.first == pointB.first) m = 1e5; // vertical
        else m = (pointA.second - pointB.second) / (pointA.first - pointB.first); // the slope of the line

        // do a high level check to see if the circle and line collide somewhere in the middle
        std::pair<float, float> H; // important point on the line
        float Hx = (pointA.second - center.second - (m * pointA.first) - (center.first / m)) / (-m - (1 / m));
        float Hy = center.second - (1 / m) * (Hx - center.first);

        H = std::pair<float, float>{(float)Hx, Hy};

        std::pair<float, float> posN, negN; // an important point on the circle
        float perpM = -1 / m; // slope of perpendicular line

        float posNx = std::sqrt((radius * radius) / (perpM * perpM + 1));;
        float negNx = -posNx;

        float posNy = posNx * perpM;
        float negNy = negNx * perpM;

        posN = std::pair<float, float>{center.first + posNx, center.second + posNy};
        negN = std::pair<float, float>{center.first + negNx, center.second + negNy};
   
        float HtoCenterDist = std::hypot(H.first - center.first, H.second - center.second);
        if (HtoCenterDist < std::hypot(posN.first - center.first, posN.second - center.second) || HtoCenterDist < std::hypot(negN.first - center.first, negN.second - center.second)) return true;
        return false;
    }
    
    float dotProductOf(std::pair<float, float> v1, std::pair<float, float> v2)
    {
        return (v1.first * v2.first) + (v1.second * v2.second);
    }
}