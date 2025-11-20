#include <iostream>
#include "Swap.h"
using namespace std;

int main(){
    cout << "Choose swap method:\n 1. Default Swap\n 2. XOR Swap\n 3. Addition/Subtraction Swap \n";

    int choice = 0;
    cin >> choice;

    cout << "Enter two integers: ";
    int a, b;
    cin >> a >> b;
    switch(choice){
        case 1:
            swapDefault(a, b);
            break;
        case 2:
            swapXOR(a, b);
            break;
        case 3:
            swapAddSub(a, b);
            break;
        default:
            cout << "Invalid choice!" << endl;
            return 1;
    }

    cout << "Swapped values: a = " << a << ", b = " << b << endl;
    return 0;
}