#include "data.h"

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
