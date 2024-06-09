#include "test.h"

int main() {
  Test *arr[MAX_SIZE] = {nullptr};
  for (int i = 0; i < MAX_SIZE; ++i) {
    Test *test = new Test(i);
    arr[i] = test;
  }
  for (int i = 0; i < MAX_SIZE; ++i) {
    delete arr[i];
  }

  return 0;
}