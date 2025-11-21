#include <iostream>
#include "Matrix.h"
using namespace std;

int** createMatrix(int rows, int cols) {
    int** matrix = new int*[rows];
    for (int i = 0; i < rows; ++i)
        matrix[i] = new int[cols];
    return matrix;
}

void fillMatrix(int** matrix, int rows, int cols) {
    cout << "Enter elements:\n";
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            cin >> matrix[i][j];
}

void printMatrix(int** matrix, int rows, int cols) {
    cout << "\nMatrix:\n";
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j)
            cout << matrix[i][j] << " ";
        cout << "\n";
    }
}

int sumMatrix(int** matrix, int rows, int cols) {
    int sum = 0;
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            sum += matrix[i][j];

    return sum;
}

void freeMatrix(int** matrix, int rows) {
    for (int i = 0; i < rows; ++i)
        delete[] matrix[i];
    delete[] matrix;
}
