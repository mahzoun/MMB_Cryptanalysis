//
// Created by sauron on 11/24/20.
//

#include "MMB.h"

void MMB::teta(uint32_t x[BLOCK_SIZE])
{
    uint32_t y[BLOCK_SIZE];
    y[0] = x[0] ^ x[1] ^ x[3];
    y[1] = x[0] ^ x[1] ^ x[2];
    y[2] = x[1] ^ x[2] ^ x[3];
    y[3] = x[0] ^ x[2] ^ x[3];
    for(int i = 0; i < BLOCK_SIZE; i++)
        x[i] = y[i];
}

void MMB::eta(uint32_t x[BLOCK_SIZE])
{
    x[0] = x[0] ^ ((1&x[0])*delta);
    x[3] = x[3] ^ ((1&x[3])*delta);
}

void MMB::gamma(uint32_t x[BLOCK_SIZE])
{
    uint32_t y[BLOCK_SIZE];
    for (uint8_t i = 0; i < BLOCK_SIZE; i++){
        if (x[i] == 0xFFFFFFFF)
            y[i] = x[i];
        else {
            uint64_t temp = (uint64_t)x[i] * (uint64_t)G[i];
            y[i] = temp % 0xFFFFFFFF;
        }
    }
    for(int i = 0; i < BLOCK_SIZE; i++)
        x[i] = y[i];
}

void MMB::gamma_inv(uint32_t x[BLOCK_SIZE])
{
    uint32_t y[BLOCK_SIZE];
    for (uint8_t i = 0; i < BLOCK_SIZE; i++){
        if (x[i] == 0xFFFFFFFF)
            y[i] = x[i];
        else {
            uint64_t temp = (uint64_t)x[i] * (uint64_t)G_inv[i];
            y[i] = temp % 0xFFFFFFFF;
        }
    }
    for(int i = 0; i < BLOCK_SIZE; i++)
        x[i] = y[i];
}

void MMB::sigma(uint32_t x[BLOCK_SIZE], uint32_t k[BLOCK_SIZE], uint32_t J)
{
    uint32_t y[BLOCK_SIZE];
    for (int i = 0; i < BLOCK_SIZE; i++){
//TODO fix k_iJ
        uint32_t k_iJ = k[(i+J)%4];// ^ ((1<<J) * B);
        y[i] = x[i] ^ k_iJ;
    }
    for(int i = 0; i < BLOCK_SIZE; i++)
        x[i] = y[i];
}

void MMB::rho(uint32_t x[BLOCK_SIZE], uint32_t k[BLOCK_SIZE], uint32_t J)
{
    sigma(x, k, J);
    gamma(x);
    eta(x);
    teta(x);
}

uint32_t * MMB::rho_inv(uint32_t x[BLOCK_SIZE], uint32_t k[BLOCK_SIZE], uint32_t J)
{
    teta(x);
    eta(x);
    gamma_inv(x);
    sigma(x, k, J);
}

void MMB::Enc(uint32_t x[BLOCK_SIZE], uint32_t k[BLOCK_SIZE], uint32_t y[BLOCK_SIZE])
{
    for(int i = 0 ;i < BLOCK_SIZE;i++)
        y[i] = x[i];
    for(uint8_t i = 0; i < ROUNDS; i++)
        rho(y, k, i);
    sigma(y, k, 6);
}

void MMB::Dec(uint32_t x[BLOCK_SIZE], uint32_t k[BLOCK_SIZE], uint32_t y[BLOCK_SIZE])
{
    for(int i = 0 ;i < BLOCK_SIZE;i++)
        y[i] = x[i];
    sigma(y, k, 6);
    for(int i = ROUNDS - 1; i >= 0; i--) {
//        std::cout << i << "\n";
        rho_inv(y, k, i);
    }
}
