#include "RectangleShape.h"

RectangleShape::RectangleShape(Point p, float w, float h, sf::Color fill, sf::Color outline)
    : topLeft(p), width(w), height(h), fillColor(fill), outlineColor(outline) {}

void RectangleShape::draw(sf::RenderWindow &window) {
    sf::RectangleShape rect(sf::Vector2f(width, height));
    rect.setPosition(topLeft.x, topLeft.y);
    rect.setFillColor(fillColor);
    rect.setOutlineColor(outlineColor);
    rect.setOutlineThickness(3);
    window.draw(rect);
}
