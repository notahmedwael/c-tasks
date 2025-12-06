#include "Scene.h"

void Scene::add(BaseShape* s) {
    shapes.push_back(s);
}

void Scene::draw(sf::RenderWindow &window) {
    for(auto s : shapes)
        s->draw(window);
}

Scene::~Scene() {
    for(auto s : shapes)
        delete s;
}
