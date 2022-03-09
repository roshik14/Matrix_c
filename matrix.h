#ifndef SRC_S21_MATRIX_H_
#define SRC_S21_MATRIX_H_
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#define SUCCESS 1
#define FAILURE 0
#define SUB 0
#define SUM 1
#define MULT 2
#define MAX_BUF 100
#define E 1e-7

typedef enum {
    CORRECT_MATRIX = 0,
    INCORRECT_MATRIX = 1,
    IDENTITY_MATRIX = 2,
    ZERO_MATRIX = 3
} matrix_type_t;

typedef struct matrix_struct {
    double** matrix;
    int rows;
    int columns;
    matrix_type_t matrix_type;
} matrix_t;

matrix_t create_matrix(int rows, int columns);
void remove_matrix(matrix_t *A);
int eq_matrix(matrix_t *A, matrix_t *B);
matrix_t sum_matrix(matrix_t *A, matrix_t *B);
matrix_t sub_matrix(matrix_t *A, matrix_t *B);
matrix_t mult_number(matrix_t *A, double number);
matrix_t mult_matrix(matrix_t *A, matrix_t *B);
matrix_t transpose(matrix_t *A);
matrix_t calc_complements(matrix_t *A);
double determinant(matrix_t *A);
matrix_t inverse_matrix(matrix_t *A);

// Help functions
matrix_t create_identity_matrix(int rows, int columns);
matrix_t create_incorrect_matrix();
int check_matrixes(matrix_t *A, matrix_t *B);
int check_one_matrix(matrix_t *A);
int is_zero_matrix(matrix_t M);
int is_identity_matrix(matrix_t M);
int is_not_square(matrix_t *M);
int is_empty(int rows, int columns);
int get_matrix_type(matrix_t M);
int is_incorrect_matrix(matrix_t *M);
int is_size_equal(matrix_t *A, matrix_t *B);
matrix_t sub_sum_mult_matrix(matrix_t *A, matrix_t *B, int expr, double number);
matrix_t get_minor_matrix(matrix_t *A, int a_i, int a_j);
double get_minor_value(matrix_t *A, int a_i, int a_j);
double get_det(matrix_t *A);

#endif  // SRC_S21_MATRIX_H_
