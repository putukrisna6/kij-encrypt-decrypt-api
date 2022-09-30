//
// Created by axelb on 24/09/2022.
//

#include <assert.h>
#include "aes.h"

using namespace std;

/**
 * @brief Store key, plaintext, and expected cipher text value
 * 
 * Format:
 *      * key
 *      * plaintext
 *      * expected ciphertext = if empty, the expected ciphertext wont be checked
 */
vector<vector<string>> testCases{
    {
        "16_long_private1",
        "AES encrypt message",
        ""
    },
    {
        "qÉGÙèY·­Ö¯g",
        "#Eg«ÍïþÜºvT2",
        "ÿJS¿|i4«Cd¹"
    },
};

int main() {
    for (int i = 0; i < testCases.size(); i++) {
        vector<string> testCase = testCases[i];

        string private_key = testCase[0];
        string message = testCase[1];

        AES *aes = new AES(private_key);
        string cipher_message = aes->encrypt(message);
        string decrypted_cipher_message = aes->decrypt(cipher_message);

        cout << endl << endl;
        cout << "private_key: " << private_key << endl;
        cout << "message_size: " << message.length() << endl;
        cout << "message: " << message << endl;
        cout << "cipher_size: " << cipher_message.length() << endl;
        cout << "cipher: " << cipher_message << endl;
        cout << "decrypted_size: " << decrypted_cipher_message.length() << endl;
        cout << "decrypted: " << decrypted_cipher_message << endl;

        if (testCase[2] != "") {
            assert(cipher_message == testCase[2]);
        }
        assert(decrypted_cipher_message == testCase[1]);
    }
}
