#include "Fan.hpp"

std::tuple<sf::RectangleShape, sf::ConvexShape> Arrow::create() const
{
    sf::RectangleShape line = sf::RectangleShape({5.f, magnitude});
    line.setFillColor(sf::Color({255, 255, 255, 125}));
    sf::ConvexShape tip = sf::ConvexShape(3);
    tip.setPoint(0, {3.f, magnitude + 10.f});
    tip.setPoint(1, {13.f, magnitude});
    tip.setPoint(2, {-7.f, magnitude});
    tip.setFillColor(sf::Color({255, 255, 255, 125}));
    return std::tuple<sf::RectangleShape, sf::ConvexShape> {line, tip};
}

void Arrow::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    auto figure = create();
    target.draw(get<0>(figure), states);
    target.draw(get<1>(figure), states);
}

Arrow::Arrow(float magnitude)
{
    this->magnitude = magnitude;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::tuple<sf::CircleShape, sf::CircleShape, sf::RectangleShape, std::vector<Arrow>> Fan::create() const
{
    sf::CircleShape knob1 = sf::CircleShape(KNOB_RADIUS);
    knob1.setOrigin(knob1.getGeometricCenter());
    knob1.setFillColor(sf::Color({232, 37, 120}));
    knob1.setOutlineColor({137, 17, 191});
    knob1.setOutlineThickness(2);

    sf::CircleShape knob2 = sf::CircleShape(KNOB_RADIUS);
    knob2.setOrigin(knob2.getGeometricCenter());
    knob2.move({length, 0});
    knob2.setFillColor(sf::Color({232, 37, 120}));
    knob2.setOutlineColor({137, 17, 191});
    knob2.setOutlineThickness(2);

    sf::RectangleShape bar = sf::RectangleShape({length, 5.f});
    bar.setFillColor(sf::Color({23, 209, 52}));

    std::vector<Arrow> arrows;

    const float SPACE = 30.f;
    float centeringOffset = (length - std::floor(length / SPACE) * SPACE) / 2; 
    for (float i = centeringOffset; i < length; i += SPACE)
    {
        Arrow arrow = Arrow(magnitude);
        arrow.move({i, 0});
        arrows.push_back(arrow);
    }

    return std::tuple<sf::CircleShape, sf::CircleShape, sf::RectangleShape, std::vector<Arrow>> {knob1, knob2, bar, arrows};
}

sf::VertexArray Fan::createMarker() const
{
    const float distance = 15.f;
    sf::VertexArray figure = sf::VertexArray(sf::PrimitiveType::Lines, 4);
    figure[0].position = sf::Vector2f({-distance, distance});
    figure[1].position = sf::Vector2f({distance, -distance});
    figure[2].position = sf::Vector2f({-distance, -distance});
    figure[3].position = sf::Vector2f({distance, distance});

    figure[0].color = sf::Color::Red;
    figure[1].color = sf::Color::Red;
    figure[2].color = sf::Color::Red;
    figure[3].color = sf::Color::Red;

    return figure;
}

void Fan::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform = getTransform();

    if (showMarker)
    {
        auto figure = createMarker();
        target.draw(figure, states);
    }
    else
    {
        auto figures = create();
        for (Arrow arrow : get<3>(figures)) target.draw(arrow, states);

        target.draw(get<2>(figures), states);
        target.draw(get<0>(figures), states);
        target.draw(get<1>(figures), states);
    }
}

float Fan::findLineX(sf::Vector2f pointA, sf::Vector2f pointB, float y) const
{
    return (pm::findXOnLine({pointA.x, pointA.y}, {pointB.x, pointB.y}, y));
}

sf::Vector2i Fan::findForceVector()
{
    // find the slope between both knobs (delta_X, delta_Y)
    sf::Vector2i slope = sf::Vector2i({(int)-(knob1Pos.x - knob2Pos.x), (int)-(knob1Pos.y - knob2Pos.y)});
    // make it perpendicular
    slope = slope.perpendicular();
    // set the magnitude to the length of this vector
    magnitude = std::sqrt((float)slope.x * slope.x + (float)slope.y * slope.y);
    // dampen the vector (same direction ~1/100 magnitude)
    slope.x /= 100;
    slope.y /= 100;
    return slope;
}

void Fan::init(sf::Vector2i knob2Pos)
{
    showMarker = false;
    this->knob2Pos = sf::Vector2f({(float)knob2Pos.x, (float)knob2Pos.y});

    // find the distance between the two knobs, this is the 'length' of the figure
    float dy = knob1Pos.y - this->knob2Pos.y, dx = knob1Pos.x - this->knob2Pos.x;
    float distSum = (dy * dy) + (dx * dx);

    this->length = std::sqrt(distSum);

    rotation = sf::radians(std::atan(dy / dx));
    if (dx > 0) rotation += sf::degrees(180);
    setRotation(rotation);

    // set the magnitude and force vector
    forceVector = findForceVector();

    // define the rotated rectangle corners in local fan space and transform into world space
    // local coords: fan is at (0,0), knob2 at (length,0), so the opposite edge is at y = magnitude
    sf::Vector2f localKnob1Ray = {0.f, magnitude};
    sf::Vector2f localKnob2Ray = {length, magnitude};
    sf::Transform fanTransform = getTransform();
    this->knob1Ray = fanTransform.transformPoint(localKnob1Ray);
    this->knob2Ray = fanTransform.transformPoint(localKnob2Ray);
}

Fan::Fan(sf::Vector2i knob1Pos)
{
    showMarker = true; // show the red x demarcating knob1's position
    this->knob1Pos = sf::Vector2f({(float)knob1Pos.x, (float)knob1Pos.y});

    sf::Vector2f precise = sf::Vector2f({(float)knob1Pos.x, (float)knob1Pos.y});
    setPosition(precise);
}

bool Fan::getShowMarker() const
{
    return showMarker;
}

int Fan::applyForce(ParticleArray &array) const
{
    int affectedCount = 0;
    // define lambda functions to tell where the x would be for each segment at a given y
    auto Ay = [this](float y) {return findLineX(knob1Pos, knob2Pos, y);}; // the green segment between the two knobs
    auto By = [this](float y) {return findLineX(knob1Pos, knob1Ray, y);}; // the implied segment between knob1 and the end of the arrow coming off of it
    auto Cy = [this](float y) {return findLineX(knob2Pos, knob2Ray, y);}; // the implied segment between knob2 and the end of the arrow coming off of it
    auto Dy = [this](float y) {return findLineX(knob1Ray, knob2Ray, y);}; // the implied segment between the end of the top and bottom arrow tips
    
    // iterate through a section of the screen. we go from our highest y (least y), to the lowest (most y)
    // addition considerations to avoid going out of bounds
    int highestY = std::min({knob1Pos.y, knob2Pos.y, knob1Ray.y, knob2Ray.y}), lowestY = std::max({knob1Pos.y, knob2Pos.y, knob1Ray.y, knob2Ray.y});
    for (int i = highestY; i <= std::min(lowestY, (int)array.screen.size() - 1); i++)
    {
        // iterate horizontally so that all checked points are in the box of effect for this Fan
        int start = std::max(Ay(i), By(i)), end = std::min(Cy(i), Dy(i));
        for (int j = start; j <= std::min(end, (int)array.screen.at(i).size() - 1); j++)
        {
            if (array.screen.at(i).at(j) != nullptr) 
            {
                array.screen.at(i).at(j)->push(forceVector);
                affectedCount++;
            }
        }
    }
    return affectedCount;
}