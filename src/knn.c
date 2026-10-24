#include "knn.h"

#include <math.h>
#include <stdlib.h>

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

int knn_find_neighbors(const Matrix *train, const Matrix *query, size_t query_row, size_t neighbor_count, size_t *indices){
    if(train == NULL || query == NULL || indices == NULL){
        return -1;
    }

    if(query_row > query->rows - 1){
        return -1;
    }

    if(train->cols != query->cols){
        return -1;
    }

    if(neighbor_count == 0 || neighbor_count > train->rows){
        return -1;
    }

    double *best_distances = malloc(neighbor_count * sizeof(double));

    if(best_distances == NULL){
        return -1;
    }

    size_t count = 0;

    for(size_t i = 0; i < train->rows; i++){
        double distance = 0.0;

        distance = euclidean_distance(train, i, query, query_row);

        if(count < neighbor_count || distance < best_distances[neighbor_count - 1]){
            size_t position;

            if(count < neighbor_count){
                position = count;
            }
            else{
                position = neighbor_count - 1;
            } 

            while(position > 0 && distance < best_distances[position - 1]){
                best_distances[position] = best_distances[position - 1];
                indices[position] = indices[position - 1];
                position--;
            }

            best_distances[position] = distance;
            indices[position] = i;

            if(count < neighbor_count){
                count++;
            }
        }
    }

    free(best_distances);
    
    return 0;
}