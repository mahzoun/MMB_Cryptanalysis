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

    for(int i = 0 ; i < (1<<17); i++){
        C1[i] = new uint32_t[bmm.BLOCK_SIZE];
        C2[i] = new uint32_t[bmm.BLOCK_SIZE];
        C3[i] = new uint32_t[bmm.BLOCK_SIZE];
        C4[i] = new uint32_t[bmm.BLOCK_SIZE];
    }

    for(uint32_t i = 0; i < (1<<17); i++){
        // Generate randome plaintexts
        P1[i][0] = rand();
        P2[i][0] = P1[i][0];
        P2[i][1] = ~0x0 ^ P1[i][1];
        P2[i][2] = P1[i][2];
        P2[i][3] = ~0x0 ^ P1[i][3];
        // Generate encryption
        C1[i] = bmm.Enc(P1[i], k);
        C2[i] = bmm.Enc(P2[i], k);
        C3[i][0] = C1[i][0];
        C3[i][1] = C1[i][1];
        C3[i][2] = C1[i][2] ^ ~0x0;
        C3[i][3] = C1[i][3] ^ ~0x0;
        C4[i][0] = C2[i][0];
        C4[i][1] = C2[i][1];
        C4[i][2] = C2[i][2] ^ ~0x0;
        C4[i][3] = C2[i][3] ^ ~0x0;
    }

    //Test Enc and Dec
//    uint32_t test[4];
//    uint32_t *enc = new uint32_t[4];
//    uint32_t *dec = new uint32_t[4];
//
//    for(int i = 0; i < 4; i++)
//        cout << hex << k[i] << "\t";
//    cout << endl;
//
//    for(int i = 0; i < 4; i++)
//        cout << hex << test[i] << "\t";
//    cout << endl;
//
////    enc = bmm.rho(test, k, 6);
//    enc = bmm.sigma(bmm.gamma(bmm.eta(bmm.teta(test))), k, 6);
//    enc = bmm.Enc(test, k);
//    for(int i = 0; i < 4; i++)
//        cout << hex << enc[i] << "\t";
//    cout << endl;
//
////    dec = bmm.rho_inv(enc, k, 6);
//    dec = bmm.teta(bmm.eta(bmm.gamma_inv(bmm.sigma((enc), k, 6))));
//    dec = bmm.Dec(enc, k);
//    for(int i = 0; i < 4; i++)
//        cout << hex << dec[i] << "\t";
//    cout << endl;



    return 0;
}
