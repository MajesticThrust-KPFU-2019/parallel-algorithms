/*
Написать программу, в которой объявить и присвоить начальные значения
целочисленным массивам a[10] и b[10]. Используя конструкцию parallel for и
reduction, вычислить средние арифметические значения элементов массивов a и b,
сравнить полученные значения, результат выдать на экран.
*/

#include <cstdio>
#include <ctime>
#include <omp.h>
#include <random>

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
  int n = 10000000;
  int *a = new int[n];

  std::default_random_engine gen;
  std::uniform_int_distribution<int> dist(-10, 10);
  for (size_t i = 0; i < n; i++) {
    a[i] = dist(gen);
  }

  clock_t start, end;

  start = clock();
  auto avg_a = avg_parallel_for(a, n);
  end = clock();
  printf("Avg. a (parallel for): %f; %f ms\n", avg_a,
         double(end - start) / CLOCKS_PER_SEC * 1000);

  start = clock();
  auto avg_b = avg_parallel_reduction(a, n);
  end = clock();
  printf("Avg. b (parallel reduction): %f; %f ms\n", avg_b,
         double(end - start) / CLOCKS_PER_SEC * 1000);

  delete[] a;

  return 0;
}
