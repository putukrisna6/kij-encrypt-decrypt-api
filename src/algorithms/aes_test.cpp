//
// Created by axelb on 24/09/2022.
//

#include <cassert>
#include "aes_v2.h"

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
        "+~(®Ò¦«÷	ÏO<",
        "21àCZ17ö0¨¢4",
        "9-Á]Á¨I±ß¹s"
    },
    {
        "16_long_private1",
        "AES encrypt message",
        ""
    },
    { /* William Stallings Book testcase */
        "qÉGÙèY·­Ö¯g",
        "\u0001\u0023\u0045\u0067\u0089\u00ab\u00cd\u00ef\u00fe\u00dc\u00ba\u0098\u0076\u0054\u0032\u0010",
        "ÿJS¿|i4«Cd¹"
    },
};

int main() {
    for (int i = 0; i < testCases.size(); i++) {
        vector<string> testCase = testCases[i];

        string private_key = testCase[0];
        string message = testCase[1];

        auto *aes = new AES_V2(AESKeyLength::AES_128);
        aes->setLog(true);
        auto cipher_message = aes->Encrypt(
            reinterpret_cast<const unsigned char *>(&message[0]),
            message.length() * sizeof(unsigned char),
            reinterpret_cast<const unsigned char *>(&private_key[0])
        );
//        string decrypted_cipher_message = aes->decrypt(cipher_message);

        cout << endl << endl;
        cout << "private_key_size: " << private_key.length() << endl;
        cout << "private_key: " << private_key << endl;
        cout << "message_size: " << message.length() << endl;
        cout << "message: " << message << endl;
        cout << "cipher_size: " << strlen(reinterpret_cast<const char *>(cipher_message)) << endl;
        cout << "cipher: " << cipher_message << endl;
//        cout << "decrypted_size: " << decrypted_cipher_message.length() << endl;
//        cout << "decrypted: " << decrypted_cipher_message << endl;

//        if (!testCase[2].empty()) {
//            assert(cipher_message == testCase[2]);
//        }
//        assert(decrypted_cipher_message == testCase[1]);
    }
}
