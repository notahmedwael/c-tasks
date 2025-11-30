#include <iostream>
#include "DynamicString.h"
#include "driver.h"

void showMenu() {
    std::cout << "\n===== DynamicString Menu =====\n";
    std::cout << "1. Create string\n";
    std::cout << "2. Display string\n";
    std::cout << "3. Copy construct new string (t = s)\n";
    std::cout << "4. Copy assign another string (t = s)\n";
    std::cout << "5. Test equality (==, !=)\n";
    std::cout << "6. Lexicographical compare (<, >)\n";
    std::cout << "7. Append string\n";
    std::cout << "8. Insert string\n";
    std::cout << "9. Erase substring\n";
    std::cout << "10. Reverse\n";
    std::cout << "11. To lowercase\n";
    std::cout << "12. To uppercase\n";
    std::cout << "13. Capitalize\n";
    std::cout << "14. Push back character\n";
    std::cout << "15. Pop back character\n";
    std::cout << "16. Unary + (copy)\n";
    std::cout << "17. Unary - (lowercase copy)\n";
    std::cout << "18. Concatenation (+, +=)\n";
    std::cout << "19. Indexing []\n";
    std::cout << "20. compare(const char*)\n";
    std::cout << "21. Clear string\n";
    std::cout << "0. Exit\n";
    std::cout << "Choose: ";
}

void runStringMenu() {
    DynamicString s;
    DynamicString t;

    int choice;

    while (true) {
        showMenu();
        std::cin >> choice;
        std::cin.ignore();

        if (choice == 0) break;

        switch (choice) {

        case 1: {
            std::cout << "Enter new string: ";
            std::string tmp;
            std::getline(std::cin, tmp);
            s = DynamicString(tmp.c_str());
            std::cout << "String updated.\n";
            break;
        }

        case 2:
            std::cout << "s = \"" << s << "\"  (length: " << s.size() << ")\n";
            break;

        case 3:
            t = DynamicString(s);
            std::cout << "t copy-constructed from s.\n";
            std::cout << "t = \"" << t << "\"\n";
            break;

        case 4:
            t = s;
            std::cout << "t assigned from s.\n";
            std::cout << "t = \"" << t << "\"\n";
            break;

        case 5: {
            std::cout << "Comparing s and t:\n";
            if (s == t) std::cout << "They are equal.\n";
            else std::cout << "They are NOT equal.\n";

            if (s != t) std::cout << "They are different.\n";
            else std::cout << "They are NOT different.\n";
            break;
        }

        case 6: {
            std::cout << "Lexicographical comparison:\n";
            if (s < t) std::cout << "\"s\" comes BEFORE \"t\" alphabetically.\n";
            else if (s > t) std::cout << "\"s\" comes AFTER \"t\" alphabetically.\n";
            else std::cout << "\"s\" and \"t\" are exactly equal.\n";
            break;
        }

        case 7: {
            std::cout << "Enter text to append: ";
            std::string tmp;
            std::getline(std::cin, tmp);
            s.append(tmp.c_str());
            std::cout << "New string: " << s << "\n";
            break;
        }

        case 8: {
            int idx;
            std::cout << "Insert position: ";
            std::cin >> idx;
            std::cin.ignore();

            std::cout << "Enter text to insert: ";
            std::string tmp;
            std::getline(std::cin, tmp);

            s.insert(idx, tmp.c_str());
            std::cout << "Updated: " << s << "\n";
            break;
        }

        case 9: {
            int pos, len;
            std::cout << "Erase start: ";
            std::cin >> pos;
            std::cout << "Length: ";
            std::cin >> len;
            std::cin.ignore();

            s.erase(pos, len);
            std::cout << "Updated: " << s << "\n";
            break;
        }

        case 10:
            s.reverse();
            std::cout << "Reversed: " << s << "\n";
            break;

        case 11:
            s.toLower();
            std::cout << "Lowercase: " << s << "\n";
            break;

        case 12:
            s.toUpper();
            std::cout << "Uppercase: " << s << "\n";
            break;

        case 13:
            s.capitalize();
            std::cout << "Capitalized: " << s << "\n";
            break;

        case 14: {
            char c;
            std::cout << "Enter a character to push: ";
            std::cin >> c;
            std::cin.ignore();

            s.push_back(c);
            std::cout << "Updated: " << s << "\n";
            break;
        }

        case 15:
            s.pop_back();
            std::cout << "After pop: " << s << "\n";
            break;

        case 16: {
            DynamicString copy = +s;
            std::cout << "Unary + gives an identical copy: \"" << copy << "\"\n";
            break;
        }

        case 17: {
            DynamicString low = -s;
            std::cout << "Unary - gives lowercase copy: \"" << low << "\"\n";
            break;
        }

        case 18: {
            std::cout << "Enter another string: ";
            std::string tmp;
            std::getline(std::cin, tmp);
            DynamicString u(tmp.c_str());

            DynamicString result = s + u;
            std::cout << "Result of s + u = \"" << result << "\"\n";

            s += u;
            std::cout << "After s += u : \"" << s << "\"\n";
            break;
        }

        case 19: {
            int idx;
            std::cout << "Enter index: ";
            std::cin >> idx;
            std::cin.ignore();

            if (idx >= 0 && idx < (int)s.size())
                std::cout << "s[" << idx << "] = '" << s[idx] << "'\n";
            else
                std::cout << "Index out of range.\n";
            break;
        }

        case 20: {
            char buf[256];
            std::cout << "Enter C-string to compare against s: ";
            std::cin.getline(buf, 256);

            int r = s.compare(buf);
            if (r == 0)
                std::cout << "\"s\" and input text are EXACTLY equal.\n";
            else if (r < 0)
                std::cout << "\"s\" comes BEFORE the given text.\n";
            else
                std::cout << "\"s\" comes AFTER the given text.\n";

            break;
        }

        case 21:
            s.clear();
            std::cout << "String cleared.\n";
            break;

        default:
            std::cout << "Invalid choice.\n";
        }
    }

    std::cout << "Goodbye!\n";
}
