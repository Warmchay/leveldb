#include <iostream>
#include <ostream>

// `sizeof` return bytes as default unit 

int main () {
  char tmp[1] = {'c'};
  std::cout << sizeof(tmp[0]) << std::endl;
  std::cout << sizeof(unsigned long) << std::endl;
  return 0;
}