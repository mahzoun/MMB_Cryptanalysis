#include <iostream>
#include <cstdint>
#include <bitset>
#include <ctime>

#include "BMM.h"

using namespace std;

uint32_t *P[4][1<<17];
uint32_t *C[4][1<<17];

int main() {
    BMM bmm;
    uint32_t k[4][bmm.BLOCK_SIZE];
    srand((unsigned) time(0));
    for(uint8_t i = 0; i < bmm.BLOCK_SIZE; i++)
        k[0][i] = rand();

    // k[1] = k[0] ^ (~0, 0, 0, ~0)
    k[1][0] = k[0][0] ^ 0xFFFFFFFF;
    k[1][1] = k[0][1];
    k[1][2] = k[0][2];
    k[1][3] = k[0][3] ^ 0xFFFFFFFF;

    // k[2] = k[0] ^ (0, 0, 0, ~0)
    k[2][0] = k[0][0];
    k[2][1] = k[0][1];
    k[2][2] = k[0][2];
    k[2][3] = k[0][3] ^ 0xFFFFFFFF;

    // k[3] = k[0] ^ (~0, 0, 0, 0)
    k[3][0] = k[0][0] ^ 0xFFFFFFFF;
    k[3][1] = k[0][1];
    k[3][2] = k[0][2];
    k[3][3] = k[0][3];

    for (int j = 0; j < 4; j++)
        for(int i = 0 ; i < (1<<17); i++){
            C[j][i] = new uint32_t[bmm.BLOCK_SIZE];
            P[j][i] = new uint32_t[bmm.BLOCK_SIZE];
        }

    for(uint32_t i = 0; i < (1<<17); i++){
        // Generate randome plaintexts
        P[0][i][0] = rand();
        P[1][i][0] = P[0][i][0];
        P[1][i][1] = 0xFFFFFFFF ^ P[0][i][1];
        P[1][i][2] = P[0][i][2];
        P[1][i][3] = 0xFFFFFFFF ^ P[0][i][3];
        // Generate encryption
        C[0][i] = bmm.Enc(P[0][i], k[0]);
        C[1][i] = bmm.Enc(P[1][i], k[0]);

        // C[2] = C[0] ^ (0, 0, ~0, ~0)
        C[2][i][0] = C[0][i][0];
        C[2][i][1] = C[0][i][1];
        C[2][i][2] = C[0][i][2] ^ 0xFFFFFFFF;
        C[2][i][3] = C[0][i][3] ^ 0xFFFFFFFF;

        // C[3] = C[1] ^ (0, 0, ~0, ~0)
        C[3][i][0] = C[1][i][0];
        C[3][i][1] = C[1][i][1];
        C[3][i][2] = C[1][i][2] ^ 0xFFFFFFFF;
        C[3][i][3] = C[1][i][3] ^ 0xFFFFFFFF;
    }

    for(uint32_t i = 0; i < (1<<17); i++){
        P[2][i] = bmm.Dec(C[2][i], k[2]);
        P[3][i] = bmm.Dec(C[3][i], k[3]);
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
