/*
Написать программу, в которой объявить и присвоить начальные значения
целочисленным массивам a[10] и b[10]. Используя конструкцию parallel for и
reduction, вычислить средние арифметические значения элементов массивов a и b,
сравнить полученные значения, результат выдать на экран.
*/

#include <cstdio>
#include <omp.h>

double avg_parallel_for(int *arr, size_t n) {
  long int sum = 0;
#pragma omp parallel shared(sum)
  {
    long int subsum = 0;

#pragma omp for
    for (size_t i = 0; i < n; i++) {
      subsum += arr[i];
    }

#pragma omp atomic
    sum += subsum;
  }

  return double(sum) / n;
}

double avg_parallel_reduction(int *arr, size_t n) {
  long int sum = 0;

#pragma omp parallel for reduction(+ : sum)
  for (size_t i = 0; i < n; i++) {
    sum += arr[i];
  }

  return double(sum) / n;
}

int main() {
  int a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  int b[10] = {11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

  auto avg_a = avg_parallel_for(a, 10);
  printf("Avg. a (parallel for): %f\n", avg_a);

  auto avg_b = avg_parallel_reduction(b, 10);
  printf("Avg. b (parallel reduction): %f\n", avg_b);

  return 0;
}
