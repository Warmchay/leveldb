#include <iostream>

int main () {
  // union's member share the same memory
  union {
    short s;
    char c[sizeof(short)]; 
    // 共享一块内存时 char 是 1B 不需要做额外除法
    // 如果是 int 为 4B，需要 sizeof(int) / 4，内存大小要对齐
  } tmp;

  tmp.s = 0x0102;
  if (tmp.c[0] == 1 && tmp.c[1] == 2) {
    std::cout << "Big Endian" << std::endl;
  } else if (tmp.c[0] == 2 && tmp.c[1] == 1) {
    std::cout << "Little Endian" << std::endl;
  }

  return 0;
}