#ifndef COBA_COBA_H
#define COBA_COBA_H

#include "encryption.h"

class CobaCoba : public Encryption
{
    public:
        string encrypt(string plainText) {
            int counter = 0;
            for (char c : plainText) {
                plainText[counter] = c + 1;
                counter++;
            }
            return plainText;
        }

        string decrypt(string cipherText) {
            int counter = 0;
            for (char c : cipherText) {
                cipherText[counter] = c - 1;
                counter++;
            }
            return cipherText;
        }
};

#endif