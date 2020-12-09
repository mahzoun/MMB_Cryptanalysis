### MMB_Cryptanalysis
_____
A Boomerang Attack implementation on MMB block cipher


![alt text](https://github.com/mahzoun/MMB_Cryptanalysis/blob/master/Media/boomerang.png?raw=true)


#### Generate required data
______

To generate required plaintexts, we do as follow. First, we generate `2^17` random plaintexts 
`P[0][i]  i < 2^17`. ‍`P[0][i][0]` (i.e., first 32-bits word) is a random value for all `i` and `P[0][i][j] j = 1, 2, 3` is a fixed value.
      
      
`P[1][i] = P[0][i] ^ (0, ~0, 0, ~0`.   
 
Then we encrypt `P[0]` using `k[0]` and `P[1]` using `k[1]`. The result is stored in `C[0]` and `C[1]` respectively. 
We define       
`C[2] = C[0] ^ (0, 0, ~0, ~0)`     
`C[3] = C[1] ^ (0, 0, ~0, ~0)`     
`P[2]` is defined as decryption of `C[2]` using `k[2]` as the key. `P[3]` Defined in the same way.

The code works as follow.
```$cpp
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
    for(uint32_t i = 0; i < (1<<17); i++){
        bmm.Dec(C[2][i], k[2], P[2][i]);
        bmm.Dec(C[3][i], k[3], P[3][i]);
    }
}
```