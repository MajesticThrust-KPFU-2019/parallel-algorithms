/*
Написать программу, в которой объявить и присвоить начальные значения массиву
целых чисел a[30], для инициализации значений использовать генератор случайных
чисел. Используя конструкции omp parallel for и omp atomic, вычислить количество
элементов массива, числовые значения которых кратны 9. Количество нитей задать
самостоятельно. Результат выдать на экран.
*/

#include <cstdio>
#include <ctime>
#include <omp.h>
#include <random>

int main() {
  int a[30];

  std::uniform_int_distribution<int> dist(0, 100);

#pragma omp parallel
  {
    std::default_random_engine gen(omp_get_thread_num() + time(nullptr));

#pragma omp for
    for (size_t i = 0; i < 30; i++) {
      a[i] = dist(gen);
    }
  }
  printf("\n");

  size_t count_mul9 = 0;
#pragma omp parallel for
  for (size_t i = 0; i < 30; i++) {
    if (a[i] % 9 == 0) {
#pragma atomic write
      count_mul9 += 1;
    }
  }

  for (size_t i = 0; i < 30; i++) {
    printf("%d ", a[i]);
  }
  printf("\n");
  printf("count_mul9 = %ld\n", count_mul9);

  return 0;
}
