#include "CircleShape.h"

CircleShapeCustom::CircleShapeCustom(Point c, float r, sf::Color fill, sf::Color outline)
    : center(c), radius(r), fillColor(fill), outlineColor(outline) {}

void CircleShapeCustom::draw(sf::RenderWindow &window) {
    sf::CircleShape circle(radius);
    circle.setPosition(center.x - radius, center.y - radius);
    circle.setFillColor(fillColor);
    circle.setOutlineColor(outlineColor);
    circle.setOutlineThickness(3);
    window.draw(circle);
}
