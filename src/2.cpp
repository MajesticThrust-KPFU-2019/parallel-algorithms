/*
Написать программу, в которой определить две параллельные области, выполнение
которых зависит от условного оператора #pragma omp parallel if(...), если
заданное значение числа нитей больше 1, параллельная область выполняется, иначе
не выполняется. Число нитей перед первой областью задать равным 3, перед второй
– равным 1. Внутри параллельных областей определить количество нитей и номер
каждой нити, результат выдать на экран. Убедиться в правильности работы
программы.
*/

#include <cstdio>
#include <omp.h>
#include <string>

int main(int argc, char *argv[]) {
  int thread_count = 1;
  if (argc > 1) {
    thread_count = std::stoi(argv[1]);
    if (thread_count < 1) {
      printf("Invalid thread count: %d\n", thread_count);
      return 1;
    } else {

      printf("Got %s as an argument, launching with %s threads\n", argv[1],
             argv[1]);
    }
  } else {
    printf("No arguments were given, launching with a single thread\n");
  }

#pragma omp parallel if (thread_count > 3), num_threads(thread_count)
  {
    printf("parallel if >3: thread %d of %d\n", omp_get_thread_num(),
           omp_get_num_threads());
  }

#pragma omp parallel if (thread_count > 1), num_threads(thread_count)
  {
    printf("parallel if >1: thread %d of %d\n", omp_get_thread_num(),
           omp_get_num_threads());
  }

  return 0;
}
