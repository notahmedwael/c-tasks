#include <iostream>
#include "string_lib.h"

using namespace std;

int main(){

    cout << "Welcome To The String Library Program\n";
    cout << "=======================================\n\n";
    
    int option = 0;
    
    while(option != 7){
        cout << "\nEnter Your Choice:\n";
        cout << "1) Get String Length\n"
             << "2) Compare Two Strings\n"
             << "3) Lowercase a string\n"
             << "4) Uppercase a string\n"
             << "5) Concat Two Strings\n"
             << "6) Copy A String Into The Other\n"
             << "7) Exit\n";
        cout << "Choice: ";
        cin >> option;
        cin.ignore(); // Clear newline from buffer
    
        if(option < 1 || option > 7){
            cout << "Bad Option, Try again..\n";
            continue;
        }

        char str1[100], str2[100];
        int result;

        switch(option){
            case 1:
                cout << "Enter a string: ";
                cin.getline(str1, 100);
                cout << "Length: " << strLength(str1) << "\n";
                break;
            
            case 2:
                cout << "Enter first string: ";
                cin.getline(str1, 100);
                cout << "Enter second string: ";
                cin.getline(str2, 100);
                result = strCompare(str1, str2);
                if(result == 0)
                    cout << "Strings are equal\n";
                else if(result > 0)
                    cout << "First string is greater\n";
                else
                    cout << "Second string is greater\n";
                break;
            
            case 3:
                cout << "Enter a string: ";
                cin.getline(str1, 100);
                toLowercase(str1);
                cout << "Lowercase: " << str1 << "\n";
                break;
            
            case 4:
                cout << "Enter a string: ";
                cin.getline(str1, 100);
                toUppercase(str1);
                cout << "Uppercase: " << str1 << "\n";
                break;
            
            case 5:
                cout << "Enter first string: ";
                cin.getline(str1, 100);
                cout << "Enter second string: ";
                cin.getline(str2, 100);
                result = strConcat(str1, str2, 100);
                if(result == 0)
                    cout << "Concatenated: " << str1 << "\n";
                else
                    cout << "Error: Not enough space\n";
                break;
            
            case 6:
                cout << "Enter destination string: ";
                cin.getline(str1, 100);
                cout << "Enter source string: ";
                cin.getline(str2, 100);
                result = strCpy(str1, str2, 100);
                if(result == 0)
                    cout << "Result: " << str1 << "\n";
                else
                    cout << "Error: Not enough space\n";
                break;
            case 7:
                cout << "Goodbye!\n";
                break;
        }
    }

    return 0;
}