#pragma once

#include <iostream>
#include <stdexcept>

class Matrix {
private:
    int **matrix;
    uint rows;
    uint columns;
public:
    Matrix(const uint r, const uint c) : rows(r), columns(c) {
        matrix = new int*[r];
        if (matrix == NULL) throw std::runtime_error("Unable to allocate matrix");

        for (int i = 0; i < r; i++) {
            matrix[i] = new int[c];
            for (int j = 0; j < c; j++) matrix[i][j] = 0;
        }
    }
};