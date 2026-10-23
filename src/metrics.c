#include "metrics.h"
#include <math.h>

double accuracy(const Matrix *predicted, const Matrix *actual){
    if(predicted == NULL || actual == NULL){
        return NAN;
    }

    if(predicted->rows != actual->rows || predicted->cols != actual->cols){
        return NAN;
    }

    size_t total = predicted->rows * predicted->cols;

    size_t hits = 0;

    for(size_t i = 0; i < total; i++){
        if(predicted->elements[i] == actual->elements[i]){
            hits++;
        }
    }

    return (double)hits / total;
}