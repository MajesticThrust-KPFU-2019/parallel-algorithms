/*
Написать программу, в которой объявить и присвоить начальные значения
целочисленным переменным: a и b, определить две параллельные области, первая – с
числом нитей 2, вторая – с числом нитей 4. Для первой области переменные a и b
объявить защищенными, с режимом доступа private и firstprivate соответственно,
внутри области определить номер нити и выполнить увеличение значения переменных
на эту величину. Выдать на экран значения переменных до входа в параллельную
область, внутри параллельной области и после выхода из параллельной области. Для
второй области переменную a объявить общей, переменную b объявить защищенной, с
режимом доступа private, определить номер нити и выполнить уменьшение значения
переменных на эту величину. Выдать на экран значения переменных до входа в
параллельную область, внутри параллельной области и после выхода из параллельной
области.
*/

#include <cstdio>
#include <omp.h>

int main() {
  int a = 1, b = 2;

  printf("Before first parallel: a = %d, b = %d\n", a, b);
#pragma omp parallel num_threads(2) private(a) firstprivate(b)
  {
    auto tid = omp_get_thread_num();
    printf("First parallel: thread %d, a = %d, b = %d\n", tid, a, b);
    a++;
    b++;
    printf("First parallel: after increment; thread %d, a = %d, b = %d\n", tid,
           a, b);
  }
  printf("After first parallel: a = %d, b = %d\n", a, b);

  printf("Before second parallel: a = %d, b = %d\n", a, b);
#pragma omp parallel num_threads(4) shared(a) private(b)
  {
    auto tid = omp_get_thread_num();
    printf("Second parallel: thread %d, a = %d, b = %d\n", tid, a, b);
    a--;
    b--;
    printf("Second parallel: after decrement; thread %d, a = %d, b = %d\n", tid,
           a, b);
  }
  printf("After second parallel: a = %d, b = %d\n", a, b);

  return 0;
}
