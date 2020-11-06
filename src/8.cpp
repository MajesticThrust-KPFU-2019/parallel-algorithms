/*
Используя возможности OpenMP, написать программу умножения матрицы на вектор.
Память для матрицы и векторов выделять динамически. Сравнить результат
выполнения последовательной и параллельной программ. Сравнить время работы
последовательной и параллельных программ.
*/

#include <chrono>
#include <cstdio>
#include <ctime>
#include <omp.h>
#include <random>

void fill_arr(double *arr, size_t n, double min, double max) {
  std::uniform_real_distribution<double> dist(min, max);
#pragma omp parallel shared(dist)
  {
    std::default_random_engine gen(std::time(nullptr) + omp_get_thread_num());

#pragma omp for
    for (size_t i = 0; i < n; i++) {
      arr[i] = dist(gen);
    }
  }
}

/**
 * Convert 2d index to 1d index.
 *
 * `i`, `j` - 2d indices;
 * `min_size` - the size of the minor dimension (e.g. if the matrix is
 * row-major, then `min_size` is the column count)
 */
inline size_t idx_2to1(size_t i, size_t j, size_t min_size) {
  return (i * min_size) + j;
}

void print_mat(double *mat, size_t m, size_t n) {
#pragma omp single
  {
    for (size_t i = 0; i < m; i++) {
      for (size_t j = 0; j < n; j++) {
        auto m_i = idx_2to1(i, j, n);
        // printf("%ld:%.3f\t", m_i, mat[m_i]);
        printf("%.3f\t", mat[m_i]);
      }
      printf("\n");
    }
  }
}

/**
 * Multiply m*n matrix `mat` and n-len vector `v` and write the result into
 * `out`.
 *
 * Expects `out` to fit a m-len vector.
 *
 * Matrix `mat` is an 1-dim array with row-major indexing.
 */
void mvmul(double *mat, size_t m, size_t n, double *v, double *out) {
  // for each elem in result vector
  for (size_t i = 0; i < m; i++) {
    out[i] = 0;
    // for each sum component
    for (size_t j = 0; j < n; j++) {
      auto m_i = idx_2to1(i, j, n);
      out[i] += mat[m_i] * v[j];
    }
  }
}

/**
 * Multiply m*n matrix `mat` and n-len vector `v` and write the result into
 * `out`. Parallelized version.
 *
 * Expects `out` to fit a m-len vector.
 *
 * Matrix `mat` is an 1-dim array with row-major indexing.
 */
void mvmul_parallel(double *mat, size_t m, size_t n, double *v, double *out) {
// for each elem in result vector
#pragma omp parallel for
  for (size_t i = 0; i < m; i++) {
    double out_sum = 0;
    // for each sum component
#pragma omp parallel for reduction(+ : out_sum)
    for (size_t j = 0; j < n; j++) {
      auto m_i = idx_2to1(i, j, n);
      out_sum += mat[m_i] * v[j];
    }

    out[i] = out_sum;
  }
}

int main() {
  size_t m = 30, n = 20;

  double *mat = new double[m * n];
  double *v = new double[n];
  double *out = new double[m];

  fill_arr(mat, m * n, -1, 1);
  fill_arr(v, n, -1, 1);

  printf("Matrix:\n");
  print_mat(mat, m, n);
  printf("Vector (as row):\n");
  print_mat(v, 1, n);

  clock_t t_start, t_end;

  printf("Performing non-parallelized matrix by vector multiplication\n");
  t_start = std::clock();
  mvmul(mat, m, n, v, out);
  t_end = std::clock();
  printf("Result (as row) (%f s):\n", double(t_end - t_start) / CLOCKS_PER_SEC);
  print_mat(out, 1, m);

  printf("Performing parallelized matrix by vector multiplication\n");
  t_start = std::clock();
  mvmul_parallel(mat, m, n, v, out);
  t_end = std::clock();
  printf("Result (as row) (%f s):\n", double(t_end - t_start) / CLOCKS_PER_SEC);
  print_mat(out, 1, m);

  delete[] mat;

  return 0;
}
