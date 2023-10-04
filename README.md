# Whitebox Implementation of AES and SM4

This repository implements two popular approaches of symmetric whitebox cryptography: boolean circuit and look up table.

We implement both approaches on two widely used block cipher: SMS4 and AES.

`masked_aes.cpp` `masked_sm4.cpp` contain the boolean circuit implementation of AES and SM4, `look_up_table_aes.cpp` `look_up_table_sm4.cpp` contain the look up table implementation of AES and SM4