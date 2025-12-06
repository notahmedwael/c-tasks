#pragma once
#include "BaseShape.h"
#include "Point.h"
#include <SFML/Graphics.hpp>

class RectangleShape : public BaseShape {
private:
    Point topLeft;
    float width, height;
    sf::Color fillColor;
    sf::Color outlineColor;
public:
    RectangleShape(Point p, float w, float h, sf::Color fill, sf::Color outline = sf::Color::Black);
    void draw(sf::RenderWindow &window) override;
};
