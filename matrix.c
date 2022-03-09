#include "matrix.h"

matrix_t create_matrix(int rows, int columns) {
    matrix_t N;
    if (is_empty(rows, columns)) {
        N = create_incorrect_matrix();
    } else {
        N.matrix = (double**)malloc(rows * sizeof(double*));
        for (int i = 0; i < rows; i++) {
            N.matrix[i] = (double*)calloc(columns, sizeof(double));
        }
        N.matrix_type = ZERO_MATRIX;
    }
    N.rows = rows;
    N.columns = columns;
    return N;
}

void remove_matrix(matrix_t *A) {
    for (int i = 0; i < A->rows; i++)
        free(A->matrix[i]);
    if (A->rows)
        free(A->matrix);
    A->rows = 0;
    A->columns = 0;
    A->matrix_type = INCORRECT_MATRIX;
}

int eq_matrix(matrix_t *A, matrix_t *B) {
    int result = SUCCESS;
    if (check_matrixes(A, B)) {
        result = FAILURE;
    } else {
        for (int i = 0; i < A->rows; i++) {
            for (int j = 0; j < A->columns; j++) {
                double first = A->matrix[i][j];
                double second = B->matrix[i][j];
                if (fabs(first - second) >= E)
                    result = FAILURE;
            }
        }
    }
    return result;
}

matrix_t sum_matrix(matrix_t *A, matrix_t *B) {
    matrix_t C = sub_sum_mult_matrix(A, B, SUM, 0);
    return C;
}

matrix_t sub_matrix(matrix_t *A, matrix_t *B) {
    matrix_t C = sub_sum_mult_matrix(A, B, SUB, 0);
    return C;
}

matrix_t mult_number(matrix_t *A, double number) {
    matrix_t C = sub_sum_mult_matrix(A, A, MULT, number);
    return C;
}

matrix_t mult_matrix(matrix_t *A, matrix_t *B) {
    int is_incorrect = is_incorrect_matrix(A) || is_incorrect_matrix(B);
    matrix_t C = create_matrix(0, 0);
    if (!is_incorrect && (A->columns == B->rows)) {
        int rows = A->rows;
        int columns = B->columns;
        C = create_matrix(rows, columns);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                for (int k = 0; k < B->rows; k++) {
                    C.matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
                }
            }
        }
        C.matrix_type = get_matrix_type(C);
    }
    return C;
}

matrix_t transpose(matrix_t *A) {
    matrix_t B = create_matrix(0, 0);
    if (!is_incorrect_matrix(A)) {
        B = create_matrix(A->columns, A->rows);
        for (int i = 0; i < A->rows; i++) {
            for (int j = 0; j < A->columns; j++) {
                B.matrix[j][i] = A->matrix[i][j];
            }
        }
        B.matrix_type = get_matrix_type(B);
    }
    return B;
}

matrix_t calc_complements(matrix_t *A) {
    matrix_t R;
    if (is_incorrect_matrix(A) || (A->rows == 0 || A->columns == 0)) {
        R = create_matrix(0, 0);
    } else {
        if (A->rows == A->columns) {
            if (A->rows == 1) {
                R = create_matrix(1, 1);
                R.matrix[0][0] = 1;
                R.matrix_type = get_matrix_type(R);
            } else {
                R = create_matrix(A->rows, A->columns);
                for (int i = 0; i < A->rows; i++) {
                    for (int j = 0; j < A->columns; j++) {
                        matrix_t minor = get_minor_matrix(A, i, j);
                        R.matrix[i][j] = pow(-1, i+j+2) * determinant(&minor);
                        remove_matrix(&minor);
                    }
                }
                R.matrix_type = get_matrix_type(R);
            }
        } else {
            R = create_matrix(0, 0);
        }
    }
    return R;
}

double determinant(matrix_t *A) {
    double result = 0;
    if (check_one_matrix(A)) {
        result = NAN;
    } else if (A->rows == 1) {
        result = A->matrix[0][0];
    } else if (A->rows == 2) {
            result = (A->matrix[0][0] * A->matrix[1][1]) -  (A->matrix[1][0] * A->matrix[0][1]);
    } else {
        for (int i = 0; i < A->columns; i++) {
            matrix_t minor = get_minor_matrix(A, 0, i);
            result += pow(-1, i+2) * A->matrix[0][i] * determinant(&minor);
            remove_matrix(&minor);
        }
    }
    return result;
}

matrix_t inverse_matrix(matrix_t *A) {
    matrix_t R;
    if (is_incorrect_matrix(A) || (A->rows == 0 || A->columns == 0)) {
        R = create_matrix(0, 0);
    } else {
        if (A->rows == A->columns) {
            double det = determinant(A);
            if (det == 0.0) {
                R = create_matrix(0, 0);
            } else {
                matrix_t algebraic = calc_complements(A);
                matrix_t transposed_matr = transpose(&algebraic);
                R = mult_number(&transposed_matr, 1 / det);
                R.matrix_type = get_matrix_type(R);
                remove_matrix(&algebraic);
                remove_matrix(&transposed_matr);
            }
        } else {
            R = create_matrix(0, 0);
        }
    }
    return R;
}

// Help functions
matrix_t create_identity_matrix(int rows, int columns) {
    matrix_t I = create_matrix(rows, columns);
    for (int i = 0; i < I.rows; i++) {
        for (int j = 0; j < I.columns; j++) {
            if (i == j)
                I.matrix[i][j] = 1;
        }
    }
    I.matrix_type = IDENTITY_MATRIX;
    return I;
}

matrix_t create_incorrect_matrix() {
    matrix_t R;
    R.matrix = NULL;
    R.matrix_type = INCORRECT_MATRIX;
    return R;
}

int is_zero_matrix(matrix_t M) {
    int result = SUCCESS;
    for (int i = 0; i < M.rows; i++) {
        for (int j = 0; j < M.columns; j++) {
                if (fabs(M.matrix[i][j]) > E) {
                result = FAILURE;
            }
        }
    }
    return result;
}

int is_identity_matrix(matrix_t M) {
    int result = SUCCESS;
    for (int i = 0; i < M.rows; i++) {
        for (int j = 0; j < M.columns; j++) {
            if (i == j) {
                if ((fabs(M.matrix[i][j]) - 1.0) > E) {
                    result = FAILURE;
                }
            } else {
                if (fabs(M.matrix[i][j]) > E) {
                    result = FAILURE;
                }
            }
        }
    }
    return result;
}

int get_matrix_type(matrix_t M) {
    int result = CORRECT_MATRIX;
    if (M.matrix_type == INCORRECT_MATRIX)
        result = INCORRECT_MATRIX;
    else if (is_zero_matrix(M))
        result = ZERO_MATRIX;
    else if (is_identity_matrix(M))
        result = IDENTITY_MATRIX;
    return result;
}

int is_incorrect_matrix(matrix_t *M) {
    return M->matrix_type == INCORRECT_MATRIX;
}

int is_not_square(matrix_t *M) {
    return M->rows != M->columns;
}

int is_empty(int rows, int columns) {
    return rows == 0 && columns == 0;
}

int is_size_equal(matrix_t *A, matrix_t *B) {
    return A->rows == B->rows && A->columns == B->columns;
}

int check_matrixes(matrix_t *A, matrix_t *B) {
    return (is_incorrect_matrix(A) || is_incorrect_matrix(B)) || !is_size_equal(A, B);
}

int check_one_matrix(matrix_t *A) {
    return is_incorrect_matrix(A) || is_not_square(A);
}

matrix_t sub_sum_mult_matrix(matrix_t *A, matrix_t *B, int expr, double number) {
    matrix_t C = create_matrix(0, 0);
    if (!check_matrixes(A, B) && !isnan(number)) {
        C = create_matrix(A->rows, A->columns);
        for (int i = 0; i < A->rows; i++) {
            for (int j = 0; j < A->columns; j++) {
                double first = A->matrix[i][j];
                if (expr != MULT) {
                    double second = B->matrix[i][j];
                    if (expr == SUM)
                        C.matrix[i][j] = first + second;
                    else
                        C.matrix[i][j] = first - second;
                } else {
                    C.matrix[i][j] = first * number;
                }
            }
        }
        C.matrix_type = get_matrix_type(C);
    }
    return C;
}

matrix_t get_minor_matrix(matrix_t *A, int a_i, int a_j) {
    matrix_t M = create_matrix(A->rows - 1, A->columns - 1);
    for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
            if (i != a_i && j != a_j) {
                if (i > a_i && j > a_j)
                    M.matrix[i - 1][j - 1] = A->matrix[i][j];
                else if (i > a_i)
                    M.matrix[i - 1][j] = A->matrix[i][j];
                else if (j > a_j)
                    M.matrix[i][j - 1] = A->matrix[i][j];
                else
                    M.matrix[i][j] = A->matrix[i][j];
            }
        }
    }
    return M;
}
