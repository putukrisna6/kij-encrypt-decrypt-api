#include "encryption.h"
#include <bits/stdc++.h>

using namespace std;



class ARC4: public Encryption {
    public:
        ARC4(string staticKey) {
            size_t keyLength = staticKey.length();

            // keyLength + 1 to handle null
            unsigned char key[keyLength + 1];

            for(size_t i = 0; i < keyLength; i++) {
                key[i] = (unsigned char) staticKey[i];
            }
            key[keyLength] = '\0';

            this->x = 0;
            this->y = 0;

            for (int i = 0; i < 256; i++) {
                this->stateVector.push_back(i);
            }

            int j = 0;
            int k = 0;
            int a;

            for (int i = 0; i < 256; i++) {
                a = this->stateVector[i];
                j = (unsigned char)(j + a + key[k]);
                this->stateVector[i] = this->stateVector[j];
                this->stateVector[j] = a;
                if (++k >= keyLength) {
                    k = 0;
                }
            }
        };

        string encrypt(string plainText) {
            int a;
            int b;
            int x = this->x;
            int y = this->y;
            size_t plainTextLength = plainText.length();
            vector<int> stateVectorCopy = this->stateVector;
            unsigned char tmp[plainTextLength + 1];

            for(size_t i = 0; i < plainTextLength; i++) {
                tmp[i] = (unsigned char) plainText[i];
            }
            tmp[plainTextLength] = '\0';


            for (size_t i = 0; i < plainTextLength; i++) {
                x = (unsigned char)(x + 1);
                a = stateVectorCopy[x];
                y = (unsigned char)(y + a);
                stateVectorCopy[x] = b = stateVectorCopy[y];
                stateVectorCopy[y] = a;
                tmp[i] ^= stateVectorCopy[(unsigned char)(a + b)];
            }

            string result = "";
            for(size_t i = 0; i < plainTextLength; i++) {
                result += tmp[i];
            }

            return result;
        }

        string decrypt(string cipherText) {
            return this->encrypt(cipherText);
        }

    private:
        int x, y;
        vector<int> stateVector;
};
