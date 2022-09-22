class CobaCoba : public Encryption
{
    public:
        std::string encrypt(std::string plainText) {
            int counter = 0;
            for (char c : plainText) {
                plainText[counter] = c + 1;
                counter++;
            }
            return plainText;
        }

        std::string decrypt(std::string cipherText) {
            int counter = 0;
            for (char c : cipherText) {
                cipherText[counter] = c - 1;
                counter++;
            }
            return cipherText;
        }
};