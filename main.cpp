#include <iostream>
#include <cstdint>
#include <vector>
#include <bitset>
#include <ctime>

#include "MMB.h"

using namespace std;

uint32_t *P[4][1<<17], *PP[4][1<<17];
uint32_t *C[4][1<<17], *CC[4][1<<17];
vector<pair<uint32_t,uint32_t>> I[2];
uint32_t k[4][4];

void Dif(uint32_t a[], uint32_t b[]){
    for(int i = 0; i < 4; i++)
        cout << hex << (a[i] ^ b[i]) << " ";
    cout << endl;
}

void Init_keys()
{
    MMB bmm;
    srand((unsigned) time(0));
    for(uint8_t i = 0; i < bmm.BLOCK_SIZE; i++) {
        k[0][i] = rand();
        cout << hex << k[0][i] << " ";
    }
    cout << "\n__________________________________________________\n\n";
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

//    uint32_t test[4];
//    for(int i = 0; i < 4; i++)
//        test[i] = 0;
//    for(int i = 0; i < 4; i++)
//        cout<<test[i] << " ";
//    cout << endl;
//    bmm.Enc(test, k[0]);
//    for(int i = 0; i < 4; i++)
//        cout<<hex << test[i] << " ";
//    cout << endl;
//    bmm.Dec(test, k[0]);
//    for(int i = 0; i < 4; i++)
//        cout<<test[i] << " ";
//    cout << endl;

}

void Init_texts()
{
    MMB bmm;
    for (int j = 0; j < 4; j++)
        for(int i = 0 ; i < (1<<17); i++){
            C[j][i] = new uint32_t[bmm.BLOCK_SIZE];
            CC[j][i] = new uint32_t[bmm.BLOCK_SIZE];
            P[j][i] = new uint32_t[bmm.BLOCK_SIZE];
            PP[j][i] = new uint32_t[bmm.BLOCK_SIZE];
        }

    for(uint32_t j = 0; j < bmm.BLOCK_SIZE; j++) {
        P[0][0][j] = rand();
    }
    for(uint32_t i = 0; i < (1<<17); i++){
        // Generate randome plaintexts
        if(i > 0) {
            P[0][i][0] = rand();
            for (int j = 1; j < bmm.BLOCK_SIZE; j++)
                P[0][i][j] = P[0][0][j];
        }
        // P[1] = P[0] ^ (0, ~0, 0, ~0)
        P[1][i][0] = P[0][i][0];
        P[1][i][1] = 0xFFFFFFFF ^ P[0][i][1];
        P[1][i][2] = P[0][i][2];
        P[1][i][3] = 0xFFFFFFFF ^ P[0][i][3];

//        for(uint32_t j = 0; j < bmm.BLOCK_SIZE; j++) {
//            PP[0][i][j] = P[0][i][j];
//            PP[1][i][j] = P[1][i][j];
//        }

        // Generate encryption
        bmm.Enc(P[0][i], k[0], C[0][i]);
        bmm.Enc(P[1][i], k[1], C[1][i]);
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
//    for(int i = 0; i < 4; i++)
//        for(int j = 0; j < (1<<17); j++)
//            for(int k = 0; k < 4; k++)
//                CC[i][j][k] = C[i][j][k];
    for(uint32_t i = 0; i < (1<<17); i++){
        bmm.Dec(C[2][i], k[2], P[2][i]);
        bmm.Dec(C[3][i], k[3], P[3][i]);
//        for(uint32_t j = 0; j < bmm.BLOCK_SIZE; j++) {
//            CC[0][i][j] = C[0][i][j];
//            CC[1][i][j] = C[1][i][j];
//        }
    }
//    for(int i = 0; i < 4; i++)
//        for(int j = 0; j < (1<<17); j++)
//            for(int k = 0; k < 4; k++)
//                PP[i][j][k] = P[i][j][k];



}
int main() {
    MMB bmm;
    Init_keys();
    Init_texts();
//    uint32_t test[4];
//    for(int i = 0; i < 4; i++)
//        cout<<test[i] << " ";
//    cout << endl;
//    bmm.Enc(test, k[0]);
//    for(int i = 0; i < 4; i++)
//        cout<<test[i] << " ";
//    cout << endl;
//    bmm.Dec(test, k[0]);
//    for(int i = 0; i < 4; i++)
//        cout<<test[i] << " ";
//    cout << endl;


    // find I and J such that P[0][I][0] = P[1][J][0] after one round of rho.
//    uint32_t *rho_P0[1<<17];
//    uint32_t *rho_P1[1<<17];
//    for(uint32_t i = 0; i < (1<<17); i++) {
//        for(int j = 0; j < 4; j++)
//            PP[0][i][j] = P[0][i][j];
//        bmm.sigma(PP[0][i], k[0], 0);
//        bmm.gamma(PP[0][i]);
//        bmm.eta(PP[0][i]);
//        bmm.teta(PP[0][i]);
//    }
    for(uint32_t i = 0; i < (1<<17); i++){
        for(int ii = 0; ii < 4; ii++) {
            for (int j = 0; j < 4; j++)
                PP[ii][i][j] = P[ii][i][j];
            bmm.sigma(PP[ii][i], k[ii], 0);
            bmm.gamma(PP[ii][i]);
            bmm.eta(PP[ii][i]);
            bmm.teta(PP[ii][i]);
        }
    }



    for(uint32_t i = 0; i < (1 << 17); i++) {
        for(uint32_t j = 0; j < (1<<17); j++) {
            if(PP[0][i][0] == PP[1][j][0]){
                Dif(PP[0][i], PP[1][j]);
                I[0].push_back(make_pair(i, j));
                cout << "Values for P0 and P1 " << i << " " << j << " " << hex << PP[0][i][0] << " " << PP[1][j][0] << endl;
            }
        }
    }


    for(uint32_t i = 0; i < (1 << 17); i++) {
        for(uint32_t j = 0; j < (1<<17); j++) {
//            Dif(P[2][i], P[3][i]);
            if(((P[2][i][1] ^ P[3][j][1]) == 0xFFFFFFFF) & (P[2][i][2] == P[3][j][2]) & ((P[2][i][3] ^ P[3][j][3])== 0xFFFFFFFF)){
//            if(P[2][i][0] == P[3][j][0]){
                I[1].push_back(make_pair(i, j));
                Dif(P[2][i], P[3][j]);
                cout << "Values for P2 and P3 " << i << " " << j << endl;
            }
        }
    }


    //Brute force for the key value
//    cout << I[0].size() << " " << I[1].size() << endl;
//    for(int i = 0; i < (1 << 17); i++) {
//        for(int j = 0; j < 4; j++) {
//            PP[0][i][j] = P[0][i][j];
//            PP[1][i][j] = P[1][i][j];
//            PP[2][i][j] = P[2][i][j];
//            PP[3][i][j] = P[3][i][j];
//        }
//    }
    uint32_t key_test[4][4];
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            key_test[i][j] = 0;
    for(uint32_t i = 0; i < (1<<32); i++){
        key_test[0][0] = key_test[2][0] = i;
        key_test[1][0] = key_test[3][0] = i ^ 0xFFFFFFFF;
        for(int j = 0; j < I[0].size(); j++){
//            cout << "last loop   " << i << " " << j << endl;
//            bmm.rho(PP[0][I[0][j].first], key_test[0], 0);
//            bmm.rho(PP[1][I[0][j].second], key_test[1], 0);
//            bmm.rho(PP[2][I[1][j].first], key_test[2], 0);
//            bmm.rho(PP[3][I[1][j].second], key_test[3], 0);
//            Dif(PP[0][I[0][j].first], PP[1][I[0][j].second]);
//            Dif(PP[2][I[1][j].first], PP[3][I[1][j].second]);
            if( PP[0][I[0][j].first][0] == PP[1][I[0][j].second][0] and PP[2][I[1][j].first][0] == PP[3][I[1][j].second][0]){
                cout<<k[0][0] << " " << key_test[0][0]<<endl;
            }
        }
    }
    return 0;
}