#include "data.h"
#include <stdlib.h>
//
#define SPLIT_EPSILON 1e-9

Matrix *data_normalize(const Matrix *matrix){
    if(matrix == NULL){
        return NULL;
    }

    Matrix *result = matrix_create(matrix->rows, matrix->cols);

    if(result == NULL){
        return NULL;
    }

    for(size_t j = 0; j < matrix->cols; j++){
        double min = matrix_get(matrix, 0, j);
        double max = min;

        for(size_t i = 0; i < matrix->rows; i++){
            double element = matrix->elements[i * matrix->cols + j];

            if(element < min){
                min = element;
            }

            if(element > max){
                max = element;
            }
        }

        double range = max - min;

        //A constant column has no range: dividing by 1.0 makes it 0.0.
        if(range == 0.0){
            range = 1.0;
        }

        for(size_t i = 0; i < matrix->rows; i++){
            double value = (matrix->elements[i * matrix->cols + j] - min) / range;

            matrix_set(result, i, j, value);
        }
    }

    return result;
}

Matrix *data_shuffle(const Matrix *matrix, unsigned int seed){
    if(matrix == NULL){
        return NULL;
    }

    Matrix *result = matrix_copy(matrix);

    if(result == NULL){
        return NULL;
    }

    //Nothing to shuffle so retrun the copy as it is
    if(result->rows < 2){
        return result;
    }

    srand(seed);

    for(size_t i = result->rows - 1; i > 0; i--){
        size_t j = (size_t)rand() % (i + 1);

        for(size_t c = 0; c < result->cols; c++){
            size_t a = i * result->cols + c;
            size_t b = j * result->cols + c;

            double temp = result->elements[a];
            result->elements[a] = result->elements[b];
            result->elements[b] = temp;
        }
    }

    return result;
}

//Copy count rows of matrix, starting at row first_row, into a new matrix
//The caller must make sure the rows first_row to first_row + count - 1 exist in matrix
//Returns the new matrix, or NULL if count is 0 or memory allocation fails
static Matrix *copy_rows(const Matrix *matrix, size_t first_row, size_t count){

    size_t rows = count;
    size_t cols = matrix->cols;

    Matrix *result = matrix_create(rows, cols);

    if(result == NULL){
        return NULL;
    }

    for(size_t i = 0; i < result->rows * result->cols; i++){
        result->elements[i] = matrix->elements[first_row * cols + i];
    }

    return result;
}

int data_split(const Matrix *matrix, double train_ratio, Matrix **train, Matrix **test){
    if(train != NULL){
        *train = NULL;
    }

    if(test != NULL){
        *test = NULL;
    }

    if(train == NULL || test == NULL){
        return -1;
    }

    if(matrix == NULL){
        return -1;
    }

    if(!(train_ratio > 0 && train_ratio < 1)){
        return -1;
    }

    size_t train_rows;

    train_rows = (size_t)(matrix->rows * train_ratio + SPLIT_EPSILON);

    if(train_rows == 0 || matrix->rows - train_rows == 0){
        return -1;
    }

    Matrix *new_train = copy_rows(matrix, 0, train_rows);
    Matrix *new_test = copy_rows(matrix, train_rows, matrix->rows - train_rows);

    if(new_train == NULL || new_test == NULL){
        matrix_free(new_train);
        matrix_free(new_test);
        return -1;
    }

    *train = new_train;
    *test = new_test;

    return 0;

}