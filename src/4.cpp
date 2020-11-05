/*
Написать программу, в которой объявить и присвоить начальные значения
целочисленным массивам a[10] и b[10], определить параллельную область,
количество нитей задать равным 2, выделить код для основной нити (номер 0) и
нити с номером 1. Основная нить должна выполнять поиск min значения элементов
массива a, нить с номером 1 - поиск max значения элементов массива b. Результат
выдать на экран.
*/

#include <cstdio>
#include <omp.h>
#include <stdexcept>

int min(int *arr, size_t arr_len) {
  if (arr_len == 0) {
    throw std::runtime_error("Invalid array length!");
  }

  int m = arr[0];
  for (size_t i = 1; i < arr_len; i++) {
    if (arr[i] < m) {
      m = arr[i];
    }
  }

  return m;
}

int max(int *arr, size_t arr_len) {
  if (arr_len == 0) {
    throw std::runtime_error("Invalid array length!");
  }

  int m = arr[0];
  for (size_t i = 1; i < arr_len; i++) {
    if (arr[i] > m) {
      m = arr[i];
    }
  }

  return m;
}

int main() {
  int a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  int b[10] = {11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

  int amin, bmax;
#pragma omp parallel num_threads(2)
  {
    auto tid = omp_get_thread_num();
    switch (tid) {
    case 0:
      amin = min(a, 10);
      break;
    case 1:
      bmax = max(b, 10);
      break;
    }
  }

  printf("amin = %d, bmax = %d\n", amin, bmax);

  return 0;
}
