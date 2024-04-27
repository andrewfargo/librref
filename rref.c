#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "rref.h"

/* Algorithm sourced from Purdue University:
   https://www.math.purdue.edu/~shao92/documents/Algorithm%20REF.pdf */

matrix_t new_matrix(uint_fast8_t rows, uint_fast8_t cols) {
  matrix_t ret = {.rows = rows, .cols = cols};
  ret.data = calloc(rows, sizeof(float_t *));
  for (int row = 0; row < rows; row++) {
    ret.data[row] = calloc(cols, sizeof(float_t));
  }
  return ret;
}

void free_matrix(matrix_t *matrix) {
  for (int row = 0; row < matrix->rows; row++) {
    free(matrix->data[row]);
  }
  free(matrix->data);
}


void matrix_print(matrix_t *matrix) {
  for (uint_fast8_t row = 0; row < matrix->rows; row++) {
    printf("[ ");
    for (uint_fast8_t col = 0; col < matrix->cols; col++) {
      printf("%6.3f\t", matrix->data[row][col]);
    }
    printf(" ]\n");
  }
  printf("\n");
}

/* Returns leading entry column of specified row, if it exists */
uint_fast8_t lentry_col(matrix_t *matrix, uint_fast8_t row) {
  uint_fast8_t none = UINT_FAST8_MAX;

  if (row >= matrix->rows)
    return none;

  for (int i = 0; i < matrix->cols; i++) {
    if (!(fabs(matrix->data[row][i]) < 0.00001)) {
      return i;
    }
  }

  return none;
}

/* Returns row of leading entry of specified column, if it exists */
uint_fast8_t lentry_row(matrix_t *matrix, uint_fast8_t col) {
  uint_fast8_t none = UINT_FAST8_MAX;
  if (col >= matrix->rows)
    return none;

  for (int i = 0; i < matrix->rows; i++) {
    if (lentry_col(matrix, i) == col) {
      return i;
    }
  }
  return none;
}

/* Finds the row with the leftmost leading entry */
uint_fast8_t min_lentry_row(matrix_t *matrix, uint_fast8_t pivot) {
  uint_fast8_t min = UINT_FAST8_MAX;

  for (uint_fast8_t row = pivot; row < matrix->rows; row++) {
    if (lentry_col(matrix, row) < min) {
      min = row;
    }
  }

  return min;
}

/* Finds the column with the rightmost leading entry */
uint_fast8_t max_lentry_col(matrix_t *matrix, uint_fast8_t pivot) {
  uint_fast8_t none = UINT_FAST8_MAX;
  uint_fast8_t max = none;

  for (uint_fast8_t col = 0; col < pivot; col++) {
    uint_fast8_t row = lentry_row(matrix, col);
    if (row != none) {
      max = col; // it's automatically the right-most
    }
  }

  return max;
}


/* Swap row1 and row2 */
void type_i(matrix_t *matrix, uint_fast8_t row1, uint_fast8_t row2) {
  float_t *temp = matrix->data[row1];
  matrix->data[row1] = matrix->data[row2];
  matrix->data[row2] = temp;
}

/* Multiply row by scalar */
void type_ii(matrix_t *matrix, uint_fast8_t row, float_t scalar) {
  for (uint_fast8_t col = 0; col < matrix->cols; col++) {
    matrix->data[row][col] *= scalar;
  }
}

/* row1 = row1 + scalar * row2 */
void type_iii(matrix_t *matrix, uint_fast8_t row1, uint_fast8_t row2,
              float_t scalar) {
  for (uint_fast8_t col = 0; col < matrix->cols; col++) {
    matrix->data[row1][col] += scalar * matrix->data[row2][col];
  }
}

void ref(matrix_t *matrix) {
  for (uint_fast8_t pivot = 0; pivot < matrix->rows; pivot++) {
    uint_fast8_t min_row = min_lentry_row(matrix, pivot);
    if (min_row >= matrix->rows)
      return;
    type_i(matrix, min_row, pivot);
    uint_fast8_t col = lentry_col(matrix, pivot);
    float_t pval = matrix->data[pivot][col];
    type_ii(matrix, pivot, 1.0 / pval);

    for (uint_fast8_t row = pivot + 1; row < matrix->rows; row++) {
      float_t rval = matrix->data[row][col];
      if (fabs(rval) < 0.00001)
        continue;
      type_iii(matrix, row, pivot, -1.0 * rval);
    }
  }
}

void rref(matrix_t *matrix) {
  ref(matrix);
  uint_fast8_t pivot = matrix->cols - 1;
  do {
    uint_fast8_t lrow = lentry_row(matrix, pivot);
    for (uint_fast8_t erow = 0; erow < pivot; erow++) {
      float_t rval = matrix->data[erow][pivot];
      if (fabs(rval) < 0.00001)
	continue;
      type_iii(matrix, erow, pivot, -1 * rval);
    }
  }  while ((pivot = max_lentry_col(matrix, pivot)) != UINT_FAST8_MAX);
}
