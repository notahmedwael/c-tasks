#include "Line.h"

Line::Line(Point s, Point e, sf::Color c) : start(s), end(e), color(c) {}

void Line::draw(sf::RenderWindow &window) {
    sf::Vertex line[] = {
        sf::Vertex(sf::Vector2f(start.x, start.y), color),
        sf::Vertex(sf::Vector2f(end.x, end.y), color)
    };
    window.draw(line, 2, sf::Lines);
}
