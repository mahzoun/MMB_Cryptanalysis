#include <iostream>
#include <cstdint>
#include <bitset>

int main() {
    uint32_t x = 0x0;
    std::bitset<32> y(~x);
    std::cout << y << std::endl;
    return 0;
}
