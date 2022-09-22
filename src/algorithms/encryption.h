#include <iostream>

class Encryption
{
    public:
        virtual string encrypt(string plainText) = 0;
        virtual string decrypt(string cipherText) = 0;
};