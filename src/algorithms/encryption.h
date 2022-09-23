#include <iostream>
using namespace std;

#ifndef ENCRYPTION_H
#define ENCRYPTION_H

class Encryption
{
    public:
        Encryption() {}
        virtual string encrypt(string plainText) = 0;
        virtual string decrypt(string cipherText) = 0;
};

#endif
