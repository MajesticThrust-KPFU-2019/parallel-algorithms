/*
Написать программу, в которой объявить и присвоить начальные значения элементам
двумерного массива d[6][8], для инициализации значений использовать генератор
случайных чисел. Используя конструкцию директивы sections...section, определить
три секции для выполнения следующих операций:

- первая секция выполняет вычисление среднего арифметического значения элементов
двумерного массива,

- вторая секция выполняет вычисление минимального и максимального значений
элементов двумерного массива,

- третья секция выполняет вычисление количества элементов массива, числовые
значения которых кратны 3.

В каждой секции определить и выдать на экран номер исполняющей нити и результат
выполнения вычислений.
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

double avg(int **arr, size_t m, size_t n) {
  int sum = 0;
  for (size_t i = 0; i < m; i++) {
    for (size_t j = 0; j < n; j++) {
      sum += arr[i][j];
    }
  }

  return sum / double(m * n);
}

std::pair<int, int> minmax(int **arr, size_t m, size_t n) {
  int min = arr[0][0], max = arr[0][0];
  for (size_t i = 0; i < m; i++) {
    // skip the first element
    size_t j0 = (i == 0) ? 1 : 0;
    for (size_t j = j0; j < n; j++) {
      auto curr = arr[i][j];
      min = curr < min ? curr : min;
      max = curr > max ? curr : max;
    }
  }

  return std::pair<int, int>(min, max);
}

size_t count_mul3(int **arr, size_t m, size_t n) {
  size_t count;

  for (size_t i = 0; i < m; i++) {
    for (size_t j = 0; j < n; j++) {
      if (arr[i][j] % 3 == 0) {
        count++;
      }
    }
  }

  return count;
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

#pragma omp parallel
  {
    int tid = omp_get_thread_num();

#pragma omp sections
    {
      {
        auto d_avg = avg(d, m, n);
        printf("tid %d: avg = %f\n", tid, d_avg);
      }
#pragma omp section
      {
        auto d_minmax = minmax(d, m, n);
        printf("tid %d: min = %d, max = %d\n", tid, d_minmax.first,
               d_minmax.second);
      }
#pragma omp section
      {
        auto d_count_mul3 = count_mul3(d, m, n);
        printf("tid %d: count_mul3 = %ld\n", tid, d_count_mul3);
      }
    }
  }

  for (size_t i = 0; i < m; i++) {
    delete[] d[i];
  }
  delete[] d;

  return 0;
}
