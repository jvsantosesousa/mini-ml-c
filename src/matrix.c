#include "matrix.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

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

void matrix_print(const Matrix *matrix){
    if(matrix == NULL){
        printf("(null matrix)\n");
        return;
    }

    for(size_t i = 0; i < matrix->rows; i++){
        for(size_t j = 0; j < matrix->cols; j++){
            printf("%8.3f", matrix_get(matrix, i, j));
        }
        printf("\n");
    }
}

Matrix *matrix_add(const Matrix *a, const Matrix *b){
    if(a == NULL || b == NULL){
        return NULL;
    }

    if(a->rows != b->rows || a->cols != b->cols){
        return NULL;
    }

    Matrix *result = matrix_create(a->rows, a->cols);

    if(result == NULL){
        return NULL;
    }

    for(size_t i = 0; i < a->rows * a->cols; i++){
        result->elements[i] = a->elements[i] + b->elements[i];
    }

    return result;
}

Matrix *matrix_subtract(const Matrix *a, const Matrix *b){
    if(a == NULL || b == NULL){
        return NULL;
    }

    if(a->rows != b->rows || a->cols != b->cols){
        return NULL;
    }

    Matrix *result = matrix_create(a->rows, a->cols);

    if(result == NULL){
        return NULL;
    }

    for(size_t i = 0; i < a->rows * a->cols; i++){
        result->elements[i] = a->elements[i] - b->elements[i];
    }

    return result;
}

Matrix *matrix_multiply(const Matrix *a, const Matrix *b){
    if(a == NULL || b == NULL){
        return NULL;
    }

    if(a->cols != b->rows){
        return NULL;
    }

    Matrix *result = matrix_create(a->rows, b->cols);

    if(result == NULL){
        return NULL;
    }

    for(size_t i = 0; i < a->rows; i++){
        for(size_t j = 0; j < b->cols; j++){

            double sum = 0.0;

            for(size_t k = 0; k < a->cols; k++){
                sum = sum + matrix_get(a, i, k) * matrix_get(b, k, j);
            }

            matrix_set(result, i, j, sum);
        }
    }

    return result;
}

Matrix *matrix_transpose(const Matrix *matrix){
    if(matrix == NULL){
        return NULL;
    }

    Matrix *result = matrix_create(matrix->cols, matrix->rows);

    if(result == NULL){
        return NULL;
    }

    for(size_t i = 0; i < result->cols; i++){
        for(size_t j = 0; j < result->rows; j++){
            double new_value = matrix_get(matrix, i, j);

            matrix_set(result, j, i, new_value);
        }
    }

    return result;
}

Matrix *matrix_hadamard(const Matrix *a, const Matrix *b){
    if(a == NULL || b == NULL){
        return NULL;
    }

    if(a->rows != b->rows || a->cols != b->cols){
        return NULL;
    }

    Matrix *result = matrix_create(a->rows, a->cols);

    if(result == NULL){
        return NULL;
    }

    for(size_t i = 0; i < a->rows * a->cols; i++){
        result->elements[i] = a->elements[i] * b->elements[i];
    }

    return result;
}

void matrix_free(Matrix *matrix){
    if(matrix == NULL){
        return;
    }

    free(matrix->elements);
    free(matrix);
}

