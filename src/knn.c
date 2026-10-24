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

double knn_frequent_label(const Matrix *labels, const size_t *indices, size_t neighbor_count){
    if(labels == NULL || indices == NULL){
        return NAN;
    }

    if(labels->cols != 1 || neighbor_count == 0){
        return NAN;
    }

    for(size_t i = 0; i < neighbor_count; i++){
        if(indices[i] >= labels->rows){
            return NAN;
        }
    }

    double best_label = 0.0;
    size_t best_votes = 0;

    for(size_t i = 0; i < neighbor_count; i++){
        double label = labels->elements[indices[i]];

        size_t votes = 0;

        for(size_t j = i; j < neighbor_count; j++){
            if(labels->elements[indices[j]] == label){
                votes++;
            }
        }

        if(votes > best_votes){
            best_label = label;
            best_votes = votes;
        }
    }

    return best_label;
}

Matrix *knn_predict(const Matrix *train, const Matrix *train_labels, const Matrix *test, size_t neighbor_count){
    if(train == NULL || train_labels == NULL || test == NULL){
        return NULL;
    }

    if(train_labels->cols != 1 || train_labels->rows != train->rows){
        return NULL;
    }

    if(train->cols != test->cols){
        return NULL;
    }

    if(neighbor_count == 0 || neighbor_count > train->rows){
        return NULL;
    }

    size_t *indices = malloc(neighbor_count * sizeof(size_t));

    if(indices == NULL){
        return NULL;
    }

    Matrix *predicted = matrix_create(test->rows, 1);

    if(predicted == NULL){
        free(indices);

        return NULL;
    }

    for(size_t i = 0; i < test->rows; i++){
        if(knn_find_neighbors(train, test, i, neighbor_count, indices) == -1){
            free(indices);
            matrix_free(predicted);

            return NULL;
        }

        double label = knn_frequent_label(train_labels, indices, neighbor_count);

        if(isnan(label)){
            free(indices);
            matrix_free(predicted);

            return NULL;
        }

        predicted->elements[i] = label;
    }

    free(indices);
    return predicted;
}
