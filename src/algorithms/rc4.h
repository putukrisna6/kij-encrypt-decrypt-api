#include "encryption.h"
#include <bits/stdc++.h>

using namespace std;



class ARC4: public Encryption {
    public:
        ARC4(string key) {
            size_t keyLength = key.length();

            for (int i = 0; i < 256; i++) {
                this->stateVector.push_back(i);
            }

            int j = 0;

            for (int i = 0; i < 256; i++) {
                j = (unsigned char)(j + this->stateVector[i] + key[i % keyLength]);
                swap(this->stateVector[i], this->stateVector[j]);
            }
        };

        string encrypt(string plainText) {
            int a;
            int b;
            int x = 0;
            int y = 0;
            size_t plainTextLength = plainText.length();
            vector<int> stateVectorCopy = this->stateVector;
            string result = plainText;

            for (size_t i = 0; i < plainTextLength; i++) {
                x = (unsigned char)(x + 1);
                a = stateVectorCopy[x];
                y = (unsigned char)(y + a);
                stateVectorCopy[x] = b = stateVectorCopy[y];
                stateVectorCopy[y] = a;
                result[i] ^= stateVectorCopy[(unsigned char)(a + b)];
            }

            return result;
        }

        string decrypt(string cipherText) {
            return this->encrypt(cipherText);
        }

    private:
        vector<int> stateVector;
};
