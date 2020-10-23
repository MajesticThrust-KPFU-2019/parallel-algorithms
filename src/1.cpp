/*
Написать программу где каждый поток печатает свой идентификатор, количество потоков всего и
строчку «Hello World». Запустить программу с 8 потоками. Всегда ли вывод идентичен? Почему?
*/

#include <cstdio>
#include <omp.h>

int main() {
#pragma omp parallel num_threads(8)
  printf("Thread %d of %d: Hello world!\n", omp_get_thread_num(),
         omp_get_num_threads());

  return 0;
}
