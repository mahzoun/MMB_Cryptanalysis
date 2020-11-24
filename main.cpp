#include <iostream>
#include <cstdint>
#include <bitset>
#include <ctime>

#include "BMM.h"

using namespace std;

uint32_t P1[1<<17][4];
uint32_t P2[1<<17][4];
uint32_t *C1[1<<17];
uint32_t *C2[1<<17];
uint32_t *C3[1<<17];
uint32_t *C4[1<<17];

int main() {
    BMM bmm;
    uint32_t k[bmm.BLOCK_SIZE];
    srand((unsigned) time(0));
    for(uint8_t i = 0; i < bmm.BLOCK_SIZE; i++)
        k[i] = rand();

    for(uint32_t i = 0; i < (1<<17); i++){
        P1[i][0] = rand();
        P2[i][0] = P1[i][0];
        P2[i][1] = ~0x0 ^ P1[i][1];
        P2[i][2] = P1[i][2];
        P2[i][3] = ~0x0 ^ P1[i][3];
        C1[i] = bmm.Enc(P1[i], k);
        C2[i] = bmm.Enc(P2[i], k);
    }

//    for(uint8_t i = 0; i < bmm.BLOCK_SIZE; i++) {
//        std::bitset<32> y(k[i]);
//        std::cout << k[i] << " " << y << std::endl;
//    }
    return 0;
}
