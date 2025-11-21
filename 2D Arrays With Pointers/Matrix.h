#ifndef MATRIX_H
#define MATRIX_H

// Allocate matrix
int** createMatrix(int rows, int cols);

// Fill matrix from user input
void fillMatrix(int** matrix, int rows, int cols);

// Print matrix
void printMatrix(int** matrix, int rows, int cols);

// Sum elements (pure function)
int sumMatrix(int** matrix, int rows, int cols);

// Free matrix
void freeMatrix(int** matrix, int rows);

#endif