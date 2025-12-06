#pragma once
#include "BaseShape.h"
#include "Point.h"
#include <SFML/Graphics.hpp>

class Line : public BaseShape {
private:
    Point start, end;
    sf::Color color;
public:
    Line(Point s, Point e, sf::Color c = sf::Color::Black);
    void draw(sf::RenderWindow &window) override;
};
