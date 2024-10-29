#include "task.h"

#include <iostream>

int add(int a, int b)
{
  return a + b;
}

void print(int a)
{
  std::cerr << "Printing: " << a << '\n';
}

int main()
{
  Task<int, int, int> task1(add);
  Task<void, int> task2(print);

  task2.execute(task1.execute(645, 3245));
}
