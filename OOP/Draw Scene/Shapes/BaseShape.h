#pragma once
#include <SFML/Graphics.hpp>

class BaseShape {
public:
    virtual void draw(sf::RenderWindow &window) = 0;
    virtual ~BaseShape() {}
};
