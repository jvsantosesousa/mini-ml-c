#include "data.h"
#include <stdlib.h>

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