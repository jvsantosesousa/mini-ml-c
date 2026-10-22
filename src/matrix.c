#include "matrix.h"
#include <stdlib.h>
#include <stdint.h>

Matrix *matrix_create(size_t rows, size_t cols){
    if(rows == 0 || cols == 0){
        return NULL;
    }

    if(cols > SIZE_MAX / rows){
        return NULL;
    }

    Matrix *matrix = malloc(sizeof *matrix);

    if(matrix == NULL){
        return NULL;
    }

    matrix->elements = calloc(rows * cols, sizeof(double));

    if(matrix->elements == NULL){
        free(matrix);
        return NULL;
    }

    matrix->rows = rows;
    matrix->cols = cols;

    return matrix;
}

double matrix_get(const Matrix *matrix, size_t row, size_t col){
    return matrix->elements[row * matrix->cols + col];
}

void matrix_set(Matrix *matrix, size_t row, size_t col, double value){
    matrix->elements[row * matrix->cols + col] = value;
}

void matrix_free(Matrix *matrix){
    if(matrix == NULL){
        return;
    }

    free(matrix->elements);
    free(matrix);
}

