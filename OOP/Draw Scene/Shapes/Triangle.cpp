#include "Triangle.h"

Triangle::Triangle(Point a, Point b, Point c, sf::Color fill, sf::Color outline)
    : p1(a), p2(b), p3(c), fillColor(fill), outlineColor(outline) {}

void Triangle::draw(sf::RenderWindow &window) {
    sf::ConvexShape shape;
    shape.setPointCount(3);
    shape.setPoint(0, sf::Vector2f(p1.x, p1.y));
    shape.setPoint(1, sf::Vector2f(p2.x, p2.y));
    shape.setPoint(2, sf::Vector2f(p3.x, p3.y));

    shape.setFillColor(fillColor);
    shape.setOutlineColor(outlineColor);
    shape.setOutlineThickness(3);

    window.draw(shape);
}
