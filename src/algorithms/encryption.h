#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <iostream>
using namespace std;

class Encryption
{
    public:
        virtual string encrypt(string plainText) = 0;
        virtual string decrypt(string cipherText) = 0;
};

#endif
