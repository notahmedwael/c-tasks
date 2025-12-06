#include <SFML/Graphics.hpp>
#include "Scene/Scene.h"
#include "Shapes/BaseShape.h"
#include "Shapes/RectangleShape.h"
#include "Shapes/CircleShape.h"
#include "Shapes/Line.h"
#include "Shapes/Triangle.h"
#include "Shapes/Point.h"

// Colors
sf::Color SkyBlue(135,206,235);
sf::Color GrassGreen(106,190,69);
sf::Color SunYellow(255,221,0);
sf::Color HouseRed(200,50,50);
sf::Color RoofBrown(130,70,30);
sf::Color WindmillWhite(240,240,240);
sf::Color WindowBlue(120,180,255);
sf::Color DoorBrown(120,60,10);
sf::Color BladeColor(50,50,50);

int main() {
    sf::RenderWindow window(sf::VideoMode(800,600), "Cartoon Scene OOP");
    Scene scene;

    // Background
    scene.add(new RectangleShape(Point(0,0),800,600,SkyBlue));
    scene.add(new RectangleShape(Point(0,400),800,200,GrassGreen));

    // Sun
    scene.add(new CircleShapeCustom(Point(700,100),50,SunYellow));

    // House
    scene.add(new RectangleShape(Point(250,300),200,150,HouseRed));
    scene.add(new Triangle(Point(220,300),Point(480,300),Point(360,220),RoofBrown));
    scene.add(new RectangleShape(Point(330,350),40,100,DoorBrown));
    scene.add(new RectangleShape(Point(270,330),40,40,WindowBlue));
    scene.add(new RectangleShape(Point(390,330),40,40,WindowBlue));

    // Windmill
    scene.add(new RectangleShape(Point(100,300),30,150,WindmillWhite));
    scene.add(new Line(Point(115,300),Point(115,260),BladeColor));
    scene.add(new Line(Point(115,300),Point(75,300),BladeColor));
    scene.add(new Line(Point(115,300),Point(155,300),BladeColor));
    scene.add(new Line(Point(115,300),Point(115,340),BladeColor));

    while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event))
            if(event.type==sf::Event::Closed)
                window.close();

        window.clear();
        scene.draw(window);
        window.display();
    }

    return 0;
}
