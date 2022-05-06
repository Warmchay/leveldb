#include <iostream>
#include <ostream>

// `sizeof` return bytes as default unit 

int main () {
  char tmp[1] = {'c'};
  int tmp1[1] = {0};
  std::cout << sizeof(tmp[0]) << std::endl;
  std::cout << sizeof(tmp1[0]) << std::endl;
  std::cout << sizeof(unsigned long) << std::endl;
  return 0;
}