#include "knn.h"

#include <math.h>

double euclidean_distance(const Matrix *matrix1, size_t row1, const Matrix *matrix2, size_t row2){
    if(matrix1 == NULL || matrix2 == NULL){
        return NAN;
    }

    if(matrix1->cols != matrix2->cols){
        return NAN;
    }

    if(row1 > matrix1->rows - 1){
        return NAN;
    }

    if(row2 > matrix2->rows - 1){
        return NAN;
    }

    double sum = 0.0;

    for(size_t c = 0; c < matrix1->cols; c++){
        double diff = matrix2->elements[row2 * matrix2->cols + c] - matrix1->elements[row1 * matrix1->cols + c];

        sum = sum + diff * diff;
    }

    return sqrt(sum);
}


