#include <iostream>
#include "Matrix.h"
using namespace std;

int main(){

    cout << "=============== Welcome to the 2D array program =============== \n" <<
    "Please specify the size of rows and cols of the array separated by a space: "
    << endl;

    int rows, cols;

    cin >> rows >> cols;

    int** matrix = createMatrix(rows, cols);

    fillMatrix(matrix, rows, cols);

    printMatrix(matrix, rows, cols);

    int total = sumMatrix(matrix, rows, cols);
    cout << "\nSum = " << total << endl;

    freeMatrix(matrix, rows);

    return 0;
}