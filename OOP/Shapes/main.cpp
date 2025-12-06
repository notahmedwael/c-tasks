#include "Shapes.h"
#include <memory>
#include <limits>

int main() {
    std::vector<std::unique_ptr<Shape>> shapes;

    while (true) {
        std::cout << "\n--- Shape Manager Menu ---\n";
        std::cout << "1. Add Triangle\n";
        std::cout << "2. Add Rectangle\n";
        std::cout << "3. Add Square\n";
        std::cout << "4. Add Circle\n";
        std::cout << "5. Display all shapes\n";
        std::cout << "6. Calculate area of a shape by index\n";
        std::cout << "7. Calculate and display area of all shapes\n";
        std::cout << "8. Calculate total area of all shapes\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter your choice: ";

        int choice;
        std::cin >> choice;

        if (!std::cin) { // handle non-integer input
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        if (choice == 0) break;

        try {
            switch(choice) {
                case 1: {
                    double b, h;
                    std::cout << "Enter base and height: ";
                    std::cin >> b >> h;
                    shapes.push_back(std::make_unique<Triangle>(b, h));
                    break;
                }
                case 2: {
                    double w, h;
                    std::cout << "Enter width and height: ";
                    std::cin >> w >> h;
                    shapes.push_back(std::make_unique<Rectangle>(w, h));
                    break;
                }
                case 3: {
                    double s;
                    std::cout << "Enter side: ";
                    std::cin >> s;
                    shapes.push_back(std::make_unique<Square>(s));
                    break;
                }
                case 4: {
                    double r;
                    std::cout << "Enter radius: ";
                    std::cin >> r;
                    shapes.push_back(std::make_unique<Circle>(r));
                    break;
                }
                case 5: {
                    if (shapes.empty()) {
                        std::cout << "No shapes added yet.\n";
                    } else {
                        for (size_t i = 0; i < shapes.size(); ++i) {
                            std::cout << i << ": ";
                            shapes[i]->display();
                        }
                    }
                    break;
                }
                case 6: {
                    if (shapes.empty()) {
                        std::cout << "No shapes added yet.\n";
                        break;
                    }
                    size_t idx;
                    std::cout << "Enter shape index: ";
                    std::cin >> idx;
                    if (idx >= shapes.size()) {
                        std::cout << "Invalid index.\n";
                    } else {
                        std::cout << "Area of shape " << idx << " (" 
                                  << shapes[idx]->typeOfShape() << ") = "
                                  << shapes[idx]->area() << "\n";
                    }
                    break;
                }
                case 7: {
                    if (shapes.empty()) {
                        std::cout << "No shapes added yet.\n";
                    } else {
                        std::cout << "--- Areas of all shapes ---\n";
                        for (auto& s : shapes) {
                            std::cout << s->typeOfShape() << " area = " << s->area() << "\n";
                        }
                    }
                    break;
                }
                case 8: {
                    double total = 0;
                    for (auto& s : shapes) total += s->area();
                    std::cout << "Total area of all shapes = " << total << "\n";
                    break;
                }
                default:
                    std::cout << "Invalid choice. Try again.\n";
            }
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }

    std::cout << "Exiting Shape Manager. Goodbye!\n";
    return 0;
}