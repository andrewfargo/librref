#ifndef RREF_H
#define RREF_H

#include <math.h>
#include <stdint.h>

typedef struct {
  uint_fast8_t rows;
  uint_fast8_t cols;
  float_t **data;
} matrix_t;

void print_matrix(matrix_t *matrix);

matrix_t new_matrix(uint_fast8_t rows, uint_fast8_t cols);
void free_matrix(matrix_t *matrix);

void ref(matrix_t *matrix);
void rref(matrix_t *matrix);

#endif // RREF_H
