#include <stdio.h>

struct A {};

int main() {
  printf("%d\n", sizeof(struct A));  // 0
  struct A a, b;
  if (&a == &b) printf("hi, im ddory\n");
  printf("=====================\n");
  unsigned char* adr = &a;
  for (int i = -5; i < 20; ++i) {
    printf("%dth : [%d]\n", i, *(adr + i));
  }
  printf("%p, size = %lu\n", &a, sizeof(a));
  printf("%d\n", *(&a));
  *(int*)(&a) = 1;
  printf("=====================\n");
  unsigned char* addr = &a;
  for (int i = -5; i < 20; ++i) {
    printf("%dth : [%d]\n", i, *(addr + i));
  }

  printf("%d\n", *(&a));
  printf("%d\n", *(int*)(&a));
}
