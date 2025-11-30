#include <iostream>
#include "Complex.h"
#include "Driver.h"

void complexMenu() {
    Complex a, b;
    int choice;

    while (true) {
        std::cout << "\n--- Complex Number Menu ---\n";
        std::cout << "1. Input complex numbers a and b\n";
        std::cout << "2. Display a and b\n";

        std::cout << "3. a + b\n";
        std::cout << "4. a - b\n";
        std::cout << "5. a * b\n";
        std::cout << "6. a / b\n";

        std::cout << "7. a += b\n";
        std::cout << "8. a -= b\n";
        std::cout << "9. a *= b\n";
        std::cout << "10. a /= b\n";

        std::cout << "11. Unary -a\n";
        std::cout << "12. Unary +b\n";

        std::cout << "13. ++a (prefix)\n";
        std::cout << "14. a++ (postfix)\n";
        std::cout << "15. --b (prefix)\n";
        std::cout << "16. b-- (postfix)\n";

        std::cout << "17. Compare a and b (== and !=)\n";

        std::cout << "18. Access a[index] and b[index]\n";
        std::cout << "19. Magnitude: a() and b()\n";

        std::cout << "20. Copy assignment a = b\n";
        std::cout << "21. Copy assignment b = a\n";

        std::cout << "0. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {

        case 1:
            std::cout << "Enter a (real imag): ";
            std::cin >> a;
            std::cout << "Enter b (real imag): ";
            std::cin >> b;
            break;

        case 2:
            std::cout << "a = " << a << "\n";
            std::cout << "b = " << b << "\n";
            break;

        case 3:
            std::cout << "a + b = " << (a + b) << "\n";
            break;

        case 4:
            std::cout << "a - b = " << (a - b) << "\n";
            break;

        case 5:
            std::cout << "a * b = " << (a * b) << "\n";
            break;

        case 6:
            try {
                std::cout << "a / b = " << (a / b) << "\n";
            } catch (...) {
                std::cout << "Division error!\n";
            }
            break;

        case 7:
            a += b;
            std::cout << "a += b → " << a << "\n";
            break;

        case 8:
            a -= b;
            std::cout << "a -= b → " << a << "\n";
            break;

        case 9:
            a *= b;
            std::cout << "a *= b → " << a << "\n";
            break;

        case 10:
            try {
                a /= b;
                std::cout << "a /= b → " << a << "\n";
            } catch (...) {
                std::cout << "Division error!\n";
            }
            break;

        case 11:
            std::cout << "-a = " << (-a) << "\n";
            break;

        case 12:
            std::cout << "+b = " << (+b) << "\n";
            break;

        case 13:
            std::cout << "++a → " << (++a) << "\n";
            break;

        case 14:
            std::cout << "a++ → " << (a++) << "\n";
            std::cout << "a now = " << a << "\n";
            break;

        case 15:
            std::cout << "--b → " << (--b) << "\n";
            break;

        case 16:
            std::cout << "b-- → " << (b--) << "\n";
            std::cout << "b now = " << b << "\n";
            break;

        case 17:
            std::cout << "a == b ? " << (a == b ? "Yes" : "No") << "\n";
            std::cout << "a != b ? " << (a != b ? "Yes" : "No") << "\n";
            break;

        case 18: {
            int index;
            std::cout << "Enter index for a (0=real, 1=imag): ";
            std::cin >> index;
            try {
                std::cout << "a[" << index << "] = " << a[index] << "\n";
            } catch (std::out_of_range& e) {
                std::cout << e.what() << "\n";
            }

            std::cout << "Enter index for b (0=real, 1=imag): ";
            std::cin >> index;
            try {
                std::cout << "b[" << index << "] = " << b[index] << "\n";
            } catch (std::out_of_range& e) {
                std::cout << e.what() << "\n";
            }
            break;
        }

        case 19:
            std::cout << "|a| = " << a() << "\n";
            std::cout << "|b| = " << b() << "\n";
            break;

        case 20:
            a = b;
            std::cout << "a = b → a = " << a << "\n";
            break;

        case 21:
            b = a;
            std::cout << "b = a → b = " << b << "\n";
            break;

        case 0:
            std::cout << "Exiting...\n";
            return;

        default:
            std::cout << "Invalid choice!\n";
        }
    }
}
