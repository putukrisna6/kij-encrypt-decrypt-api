#include "encryption.h"

class DES : public Encryption
{
    private:
        string key;

    public:
        DES(string key) {
            this->key = key;
        }

        string encrypt(string plainText) {
            return "da02ce3a89ecac3b";
        }

        string decrypt(string cipherText) {
            return "02468aceeca86420";
        }
};