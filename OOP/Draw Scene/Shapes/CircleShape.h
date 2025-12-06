#pragma once
#include "BaseShape.h"
#include "Point.h"
#include <SFML/Graphics.hpp>

class CircleShapeCustom : public BaseShape {
private:
    Point center;
    float radius;
    sf::Color fillColor;
    sf::Color outlineColor;
public:
    CircleShapeCustom(Point c, float r, sf::Color fill, sf::Color outline = sf::Color::Black);
    void draw(sf::RenderWindow &window) override;
};
