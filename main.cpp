#include <iostream>
#include <cstdint>
#include <vector>
#include <bitset>
#include <ctime>

#include "BMM.h"

using namespace std;

uint32_t *P[4][1<<17];
uint32_t *C[4][1<<17];
vector<pair<uint32_t,uint32_t>> I[2];

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
        for(uint32_t j = 0; j < bmm.BLOCK_SIZE; j++)
            P[0][i][j] = rand();

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

    // find I and J such that P[0][I][0] = P[1][J][0] after one round of rho.
    uint32_t *rho_P0[1<<17];
    uint32_t *rho_P1[1<<17];
    for(uint32_t i = 0; i < (1<<17); i++) {
        rho_P0[i] = bmm.rho(P[0][i], k[0], 0);
    }
    for(uint32_t j = 0; j < (1<<17); j++){
        rho_P1[j] = bmm.rho(P[1][j], k[0], 0);
    }
    for(uint32_t i = 0; i < (1 << 17); i++) {
        for(uint32_t j = 0; j < (1<<17); j++) {
            if(i != j & rho_P0[i][0] == rho_P1[j][0]){
                I[0].push_back(make_pair(i, j));
                cout << i << " " << j << " " << rho_P0[i][0] << " " << rho_P1[j][0] << endl;
            }
        }
    }


    for(uint32_t i = 0; i < (1 << 17); i++) {
        for(uint32_t j = 0; j < (1<<17); j++) {
            I[1].push_back(make_pair(i, j));
            if(i != j & P[2][i][1] == P[3][j][1] & P[2][i][2] == P[3][j][2] & P[2][i][3] == P[3][j][3]){
                cout << i << " " << j << endl;
            }
        }
    }

    return 0;
}
