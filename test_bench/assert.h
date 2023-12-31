#include <stdint.h> 
#include <string>


void assert_u32(std::string tname, uint32_t in, uint32_t expected) {
    std::cout << tname;
    if (in != expected) {
        printf("... fail\n");
        printf("in:       %d\n", in);
        printf("expected: %d\n", expected);
    }
    printf("... ok\n");
}