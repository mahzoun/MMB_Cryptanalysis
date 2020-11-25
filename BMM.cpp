//
// Created by sauron on 11/24/20.
//

#include "BMM.h"

uint32_t * BMM::teta(uint32_t x[BLOCK_SIZE])
{
    uint32_t *y = new uint32_t[BLOCK_SIZE];
    y[0] = x[0] ^ x[1] ^ x[3];
    y[1] = x[0] ^ x[1] ^ x[2];
    y[2] = x[1] ^ x[2] ^ x[3];
    y[3] = x[0] ^ x[2] ^ x[3];
    return y;
}

uint32_t * BMM::eta(uint32_t x[BLOCK_SIZE])
{
    uint32_t *y = new uint32_t[BLOCK_SIZE];
    y[0] = x[0] ^ ((1&x[0])*delta);
    y[1] = x[1];
    y[2] = x[2];
    y[3] = x[3] ^ ((1&x[3])*delta);
    return y;
}

uint32_t * BMM::gamma(uint32_t x[BLOCK_SIZE])
{
    uint32_t *y = new uint32_t[BLOCK_SIZE];
    for (uint8_t i = 0; i < BLOCK_SIZE; i++){
        if (x[i] == ~0x0)
            y[i] = x[i];
        else
            y[i] = x[i] * G[i];
    }
    return y;
}

uint32_t * BMM::sigma(uint32_t x[BLOCK_SIZE], uint32_t k[BLOCK_SIZE], uint8_t J)
{
    uint32_t *y = new uint32_t[BLOCK_SIZE];
    for (int i = 0; i < BLOCK_SIZE; i++){
        uint32_t k_iJ = k[(i+J)%4] ^ ((1<<J) * B);
        y[i] = x[i] ^ k_iJ;
    }
    return y;
}

uint32_t * BMM::rho(uint32_t x[BLOCK_SIZE], uint32_t k[BLOCK_SIZE], uint8_t J)
{
    uint32_t *y = sigma(x, k, J);
    y = gamma(y);
    y = eta(y);
    y = teta(y);
    return y;
}

uint32_t * BMM::Enc(uint32_t x[BLOCK_SIZE], uint32_t k[BLOCK_SIZE])
{
    uint32_t *y = rho(x, k, 0);
    for(uint8_t i = 0; i < ROUNDS; i++)
        y = rho(y, k, i);
    y = sigma(y, k, 6);
    return y;
}

