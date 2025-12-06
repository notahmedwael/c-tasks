#pragma once
#include "BaseShape.h"
#include "Point.h"
#include <SFML/Graphics.hpp>

class Triangle : public BaseShape {
private:
    Point p1, p2, p3;
    sf::Color fillColor;
    sf::Color outlineColor;
public:
    Triangle(Point a, Point b, Point c, sf::Color fill = sf::Color::White, sf::Color outline = sf::Color::Black);
    void draw(sf::RenderWindow &window) override;
};
