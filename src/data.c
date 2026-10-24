#include "data.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SPLIT_EPSILON 1e-9
//Max size of the line
#define CSV_LINE_SIZE 4096

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

static size_t parse_line(const char *line, double *out){
    size_t count = 0;
    const char *position = line;
    char *end;

    while(1){
        double value = strtod(position, &end);

        if(end == position){
            return 0;
        }

        if(out != NULL){
            out[count] = value;
        }

        count++;

        if(*end == ','){
            position = end + 1;
        }
        else if(*end == '\n' || *end == '\r' || *end == '\0'){
            return count;
        }
        else{
            return 0;
        }
    }
}

Matrix *data_load_csv(const char *path, int has_header){
    if(path == NULL){
        return NULL;
    }

    FILE *file = fopen(path, "r");

    if(file == NULL){
        return NULL;
    }

    char line[CSV_LINE_SIZE];
    size_t rows = 0;
    size_t cols = 0;
    int skip_line = has_header;

    while(fgets(line, CSV_LINE_SIZE, file) != NULL){
        if(strchr(line, '\n') == NULL && !feof(file)){
            fclose(file);
            return NULL;
        }

        if(skip_line){
            skip_line = 0;
            continue;
        }

        size_t fields = parse_line(line, NULL);

        if(fields == 0){
            fclose(file);
            return NULL;
        }

        if(rows == 0){
            cols = fields;
        }
        else if(fields != cols){
            fclose(file);
            return NULL;
        }

        rows++;
    }

    if(rows == 0){
        fclose(file);
        return NULL;
    }

    Matrix *result = matrix_create(rows, cols);

    if(result == NULL){
        fclose(file);
        return NULL;
    }

    rewind(file);

    skip_line = has_header;
    size_t row = 0;

    while(fgets(line, CSV_LINE_SIZE, file) != NULL){
        if(skip_line){
            skip_line = 0;
            continue;
        }

        parse_line(line, result->elements + row * cols);
        row++;
    }

    fclose(file);
    return result;
}

int data_separate_labels(const Matrix *matrix, size_t label_column, Matrix **features, Matrix **labels){
    if(features != NULL){
        *features = NULL;
    }

    if(labels != NULL){
        *labels = NULL;
    }

    if(features == NULL || labels == NULL){
        return -1;
    }

    if(matrix == NULL){
        return -1;
    }

    if(label_column > matrix->cols - 1){
        return -1;
    }

    if(matrix->cols == 1){
        return -1;
    }

    Matrix *new_features = matrix_create(matrix->rows, matrix->cols - 1);

    Matrix *new_labels = matrix_create(matrix->rows, 1);

    if(new_features == NULL || new_labels == NULL){
        matrix_free(new_features);
        matrix_free(new_labels);
        return -1;
    }

    for(size_t i = 0; i < matrix->rows; i++){

        size_t k = 0;

        for(size_t j = 0; j < matrix->cols; j++){
            double value = matrix->elements[i * matrix->cols + j];

            if(j == label_column){
                new_labels->elements[i] = value;
            }
            else{
                new_features->elements[i * new_features->cols + k] = value;
                k++;
            }
        }
    }

    *features = new_features;

    *labels = new_labels;

    return 0;
}


