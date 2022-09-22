#include <iostream>

class Encryption
{
    public:
        virtual std::string encrypt(std::string plainText) = 0;
        virtual std::string decrypt(std::string cipherText) = 0;
};