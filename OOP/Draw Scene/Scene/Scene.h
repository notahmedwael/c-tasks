#pragma once
#include <vector>
#include "../Shapes/BaseShape.h"
#include <SFML/Graphics.hpp>

class Scene {
private:
    std::vector<BaseShape*> shapes;
public:
    void add(BaseShape* s);
    void draw(sf::RenderWindow &window);
    ~Scene();
};
