#include "AES128.h"

BYTE AES128Encryption::SBox[256] = {0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
                                    0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
                                    0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
                                    0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
                                    0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
                                    0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
                                    0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
                                    0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
                                    0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
                                    0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
                                    0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
                                    0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
                                    0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
                                    0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
                                    0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
                                    0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16};

UINT32 AES128Encryption::Rcon[11] = {0x00, 0x01, 0x02, 0x04, 0x08, 0x10,
                                     0x20, 0x40, 0x80, 0x1B, 0x36};

void AES128Encryption::Cipher(BYTE srcBytes[16], UINT32 srcExpandedKey[44]) {
    *((UINT32*)srcBytes) ^= srcExpandedKey[0];
    *((UINT32*)srcBytes + 1) ^= srcExpandedKey[1];
    *((UINT32*)srcBytes + 2) ^= srcExpandedKey[2];
    *((UINT32*)srcBytes + 3) ^= srcExpandedKey[3];

    for(BYTE i = 1; i < 10; i++) {
        for(BYTE j = 0; j < 16; j++) srcBytes[j] = SBox[srcBytes[j]];

        //Shift rows starts;
        //Shift the second row;
        srcBytes[1] ^= srcBytes[5] ^= srcBytes[1] ^= srcBytes[5];
        srcBytes[5] ^= srcBytes[9] ^= srcBytes[5] ^= srcBytes[9];
        srcBytes[9] ^= srcBytes[13] ^= srcBytes[9] ^= srcBytes[13];
        //Shift the third row;
        srcBytes[2] ^= srcBytes[10] ^= srcBytes[2] ^= srcBytes[10];
        srcBytes[6] ^= srcBytes[14] ^= srcBytes[6] ^= srcBytes[14];
        //Shift the fourth row;
        srcBytes[3] ^= srcBytes[15] ^= srcBytes[3] ^= srcBytes[15];
        srcBytes[15] ^= srcBytes[11] ^= srcBytes[15] ^= srcBytes[11];
        srcBytes[11] ^= srcBytes[7] ^= srcBytes[11] ^= srcBytes[7];
        //Shift rows ends;

        for(BYTE j = 0; j < 16; j += 4) {
            BYTE tmp[4];
            *(UINT32*)tmp = *((UINT32*)srcBytes + (j >> 2));
            srcBytes[j] = (tmp[0] & 0x80 ? (tmp[0] << 1 ^ 0x1B) : tmp[0] << 1) ^ (tmp[1] & 0x80 ? (tmp[1] ^ tmp[1] << 1 ^ 0x1B) : tmp[1] ^ tmp[1] << 1) ^ tmp[2] ^ tmp[3];
            srcBytes[j + 1] = tmp[0] ^ (tmp[1] & 0x80 ? (tmp[1] << 1 ^ 0x1B) : tmp[1] << 1) ^ (tmp[2] & 0x80 ? (tmp[2] ^ tmp[2] << 1 ^ 0x1B) : tmp[2] ^ tmp[2] << 1) ^ tmp[3];
            srcBytes[j + 2] = tmp[0] ^ tmp[1] ^ (tmp[2] & 0x80 ? (tmp[2] << 1 ^ 0x1B) : tmp[2] << 1) ^ (tmp[3] & 0x80 ? (tmp[3] ^ tmp[3] << 1 ^ 0x1B) : tmp[3] ^ tmp[3] << 1);
            srcBytes[j + 3] = (tmp[0] & 0x80 ? (tmp[0] ^ tmp[0] << 1 ^ 0x1B) : tmp[0] ^ tmp[0] << 1) ^ tmp[1] ^ tmp[2] ^ (tmp[3] & 0x80 ? (tmp[3] << 1 ^ 0x1B) : tmp[3] << 1);
        }
        *((UINT32*)srcBytes) ^= srcExpandedKey[i << 2];
        *((UINT32*)srcBytes + 1) ^= srcExpandedKey[(i << 2) + 1];
        *((UINT32*)srcBytes + 2) ^= srcExpandedKey[(i << 2) + 2];
        *((UINT32*)srcBytes + 3) ^= srcExpandedKey[(i << 2) + 3];
    }

    for(BYTE j = 0; j < 16; j++) srcBytes[j] = SBox[srcBytes[j]];
    //Shift rows starts;
    //Shift the second row;
    srcBytes[1] ^= srcBytes[5] ^= srcBytes[1] ^= srcBytes[5];
    srcBytes[5] ^= srcBytes[9] ^= srcBytes[5] ^= srcBytes[9];
    srcBytes[9] ^= srcBytes[13] ^= srcBytes[9] ^= srcBytes[13];
    //Shift the third row;
    srcBytes[2] ^= srcBytes[10] ^= srcBytes[2] ^= srcBytes[10];
    srcBytes[6] ^= srcBytes[14] ^= srcBytes[6] ^= srcBytes[14];
    //Shift the fourth row;
    srcBytes[3] ^= srcBytes[15] ^= srcBytes[3] ^= srcBytes[15];
    srcBytes[15] ^= srcBytes[11] ^= srcBytes[15] ^= srcBytes[11];
    srcBytes[11] ^= srcBytes[7] ^= srcBytes[11] ^= srcBytes[7];
    //Shift rows ends;

    *((UINT32*)srcBytes) ^= srcExpandedKey[40];
    *((UINT32*)srcBytes + 1) ^= srcExpandedKey[41];
    *((UINT32*)srcBytes + 2) ^= srcExpandedKey[42];
    *((UINT32*)srcBytes + 3) ^= srcExpandedKey[43];
}

void AES128Encryption::KeyExpansion(BYTE srcKey[16], UINT32 dstExpandedKey[44]) {
    for(BYTE i = 0; i < 4; i++) dstExpandedKey[i] = *((UINT32*)srcKey + i);
    for(BYTE i = 4; i < 44; i++) {
        UINT32 tmp = dstExpandedKey[i - 1];
        if(i % 4 == 0) {
            tmp = (tmp >> 8 | tmp << 24);
            *((BYTE*)&tmp) = SBox[*((BYTE*)&tmp)];
            *((BYTE*)&tmp + 1) = SBox[*((BYTE*)&tmp + 1)];
            *((BYTE*)&tmp + 2) = SBox[*((BYTE*)&tmp + 2)];
            *((BYTE*)&tmp + 3) = SBox[*((BYTE*)&tmp + 3)];
            tmp ^= Rcon[i >> 2];
        }
        dstExpandedKey[i] = dstExpandedKey[i - 4] ^ tmp;
    }
}