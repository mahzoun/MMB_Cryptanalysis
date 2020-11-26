//
// Created by sauron on 11/24/20.
//

#ifndef MMB_CRYPTANALYSIS_BMM_H
#define MMB_CRYPTANALYSIS_BMM_H
#include <iostream>
#include <cstdint>
#include <cmath>

class BMM {
public:
    const static uint8_t BLOCK_SIZE = 4, ROUNDS = 6;
    const uint32_t delta = 0x2AAAAAAA;
    const uint32_t B = 0xDAE;
    const uint32_t G[BLOCK_SIZE] = {0x25F1CDB, 0x4BE39B6, 0x12F8E6D8, 0x2F8E6D81};
    const uint32_t G_inv[BLOCK_SIZE] = {0xDAD4694, 0x06D6A34A, 0x81B5A8D2, 0x281B5A8D};
    uint32_t* Enc(uint32_t[], uint32_t[]);
    uint32_t* Dec(uint32_t[], uint32_t[]);
    void sigma(uint32_t[], uint32_t[], uint32_t);
    void gamma(uint32_t[]);
    void gamma_inv(uint32_t[]);
    void eta(uint32_t[]);
    void teta(uint32_t[]);
    void rho(uint32_t[], uint32_t[], uint32_t);
    uint32_t* rho_inv(uint32_t[], uint32_t[], uint32_t);
};


#endif //MMB_CRYPTANALYSIS_BMM_H
