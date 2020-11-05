/*
Написать программу, в которой определить две параллельные области, каждая из
которых содержит итерационную конструкцию for выполняющую инициализацию
элементов одномерных массивов целых чисел a[12], b[12] и c[12]. Число нитей
перед первой областью задать равным 3, перед второй – равным 4. Первая
параллельная область выполняет инициализацию элементов массивов a и b с
использованием статического распределения итераций, размер порции итераций
выбрать самостоятельно, вторая параллельная область выполняет инициализацию
элементов массива c по следующему правилу c[i] = a[i] + b[i], с использованием
динамического распределения итераций, размер порции итераций выбрать
самостоятельно. В каждой области определить и выдать на экран количество нитей,
номер нити, номер итерации, которые она выполняет, и результат выполнения цикла.
Убедиться в правильности работы программы.
*/

// https://bisqwit.iki.fi/story/howto/openmp/#Scheduling

#include <cstdio>
#include <omp.h>

int main() {
  int a[12], b[12], c[12];

  omp_set_nested(2);

#pragma omp parallel sections
  {
    {
#pragma omp parallel for num_threads(3) schedule(static)
      for (size_t i = 0; i < 12; i++) {
        a[i] = i;
        b[i] = i;
        c[i] = i;

        auto tid = omp_get_thread_num();
        auto tnum = omp_get_num_threads();
        printf("S-static t(%d/%d): i = %ld, a = %d, b = %d, c = %d\n", tid,
               tnum, i, a[i], b[i], c[i]);
      }
    }
#pragma omp section
    {
#pragma omp parallel for num_threads(4) schedule(dynamic, 3)
      for (size_t i = 0; i < 12; i++) {
        // isn't this a race condition?
        c[i] = a[i] + b[i];

        auto tid = omp_get_thread_num();
        auto tnum = omp_get_num_threads();
        printf("S-dynamic t(%d/%d): i = %ld, a = %d, b = %d, c = %d\n", tid,
               tnum, i, a[i], b[i], c[i]);
      }
    }
  }

  return 0;
}
