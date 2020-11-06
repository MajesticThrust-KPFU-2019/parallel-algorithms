/*
Написать программу, в которой объявить и присвоить начальные значения элементам
двумерного массива d[6][8], для инициализации значений использовать генератор
случайных чисел. Используя конструкцию директив omp parallel for и omp critical,
определить минимальное и максимальное значения элементов двумерного массива.
Количество нитей задать самостоятельно. Результат выдать на экран.
*/

#include <cstdio>
#include <omp.h>
#include <random>
#include <utility>

void print_arr(int **arr, size_t m, size_t n) {
#pragma omp single
  {
    for (size_t i = 0; i < m; i++) {
      for (size_t j = 0; j < n; j++) {
        printf("%d\t", arr[i][j]);
      }
      printf("\n");
    }
  }
}

std::pair<int, int> minmax(int **arr, size_t m, size_t n) {
  int min = arr[0][0], max = arr[0][0];
  
#pragma omp parallel for collapse(2)
  for (size_t i = 0; i < m; i++) {
    for (size_t j = 0; j < n; j++) {
      // omp critical section name is global, not a good choice for this case
#pragma omp critical(minmax_search)
      {
        // skip the first element
        if (!(i == 0 && j == 0)) {
          auto curr = arr[i][j];
          min = curr < min ? curr : min;
          max = curr > max ? curr : max;
        }
      }
    }
  }

  return std::pair<int, int>(min, max);
}

int main() {
  size_t m = 6, n = 8;
  int d_gen_min = -10, d_gen_max = 10;
  int **d = new int *[m];
  for (size_t i = 0; i < m; i++) {
    d[i] = new int[n];
  }

  std::default_random_engine gen;
  std::uniform_int_distribution<int> dist(d_gen_min, d_gen_max);

#pragma omp parallel for collapse(2) firstprivate(gen) shared(dist)
  for (size_t i = 0; i < 6; i++) {
    for (size_t j = 0; j < 8; j++) {
      d[i][j] = dist(gen);
    }
  }

  print_arr(d, m, n);

  auto d_minmax = minmax(d, m, n);
  printf("min = %d, max = %d\n", d_minmax.first, d_minmax.second);

  for (size_t i = 0; i < m; i++) {
    delete[] d[i];
  }
  delete[] d;

  return 0;
}
