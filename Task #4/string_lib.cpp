#include "string_lib.h"

int strLength(char arr[]){
    int counter = 0;
    while(arr[counter] != '\0'){
        counter ++;
    }
    return counter;
}

int strCompare(char str1[], char str2[]){
    // Get sizes
    int size1 = 0, size2 = 0;

    while(str1[size1] != '\0'){
        size1 ++;
    }

    while(str2[size2] != '\0'){
        size2 ++;
    }

    // Return by length differ
    if (size1 > size2){
        return 5;
    }
    if(size1 < size2){
        return -5;
    }

    // Compare by ASCII
    for (int i = 0; i < size1; i++)
    {
        if(str1[i] > str2[i]){
            return 1;
        }
        if (str1[i] < str2[i])
        {
            return -1;
        }
    }
    return 0;
}

void toLowercase(char str[]) {
    for (int i = 0; str[i]; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            str[i] += 32;
        }
    }
}

void toUppercase(char str[]) {
    for (int i = 0; str[i]; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] -= 32;
        }
    }
}

int strConcat(char str1[], char str2[], int sizeStr1){
    int len1 = strLength(str1);
    int len2 = strLength(str2);
    
    // Check if there's enough space
    if(len1 + len2 + 1 > sizeStr1){
        return -1; // Not enough space
    }
    
    // Copy str2 to end of str1
    int i = 0;
    while(str2[i] != '\0'){
        str1[len1 + i] = str2[i];
        i++;
    }
    str1[len1 + i] = '\0'; // Add null terminator
    
    return 0; // Success
}

int strCpy(char str1[], char str2[], int sizeStr1){
    int len2 = strLength(str2);
    
    // Check if there's enough space
    if(len2 + 1 > sizeStr1){
        return -1; // Not enough space
    }
    
    int i = 0;
    while(str2[i] != '\0'){
        str1[i] = str2[i];
        i++;
    }
    str1[i] = '\0'; // Add null terminator
    
    return 0; // Success
}