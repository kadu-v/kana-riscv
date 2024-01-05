#include <stdint.h>

#include <string>

void assert_eq(std::string test_name, uint32_t in, uint32_t expected) {
  std::cout << test_name;
  if (in != expected) {
    printf(" ...failed\n");
    printf("    actual:   %u\n", in);
    printf("    expected: %u\n", expected);
    return;
  }
  printf(" ...ok\n");
}