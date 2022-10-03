//
// Created by axelb on 24/09/2022.
//

#include <cassert>
#include "aes_v2.h"

using namespace std;

int main() {
/**
 * @brief Store key, plaintext, and expected cipher text value
 *
 * Format:
 *      * key
 *      * plaintext
 *      * expected ciphertext = if empty, the expected ciphertext wont be checked
 */
    vector<vector<string>> testCases{
//            { /* William Stallings Book testcase */
//                    "00001111000101010111000111001001010001111101100111101000010110010000110010110111101011011101011010101111011111110110011110011000",
//                    "00000001001000110100010101100111100010011010101111001101111011111111111011011100101110101001100001110110010101000011001000010000",
//                    "11111111000010111000010001001010000010000101001110111111011111000110100100110100101010110100001101100100000101001000111110111001"
//            },
             {
                "00110001001101100101111101101100011011110110111001100111010111110111000001110010011010010111011001100001011101000110010101011111", //"16_long_private_",
                 "01000001010001010101001100100000011001010110111001100011011100100111100101110000011101000010000001101101011001010111001101110011011000010110011101100101", //"AES encrypt message",
                 ""
             },
             {
                 "16_long_private_",
                  "AES encrypt message",
                 ""
             },
    };

    for (int i = 0; i < testCases.size(); i++) {
        vector<string> testCase = testCases[i];

        AES_V2 *aes = new AES_V2(testCase[0]);
        aes->setLog(true);

        auto cipher_text = aes->Encrypt(testCase[1]);

//        aes->setLog(true);
        auto decrypted_cipher_message = aes->Decrypt(cipher_text);

        // cout << endl << endl;
        // cout << "private_key_size: " << private_key.length() << endl;
         cout << "private_key: " << testCase[0] << endl;
         cout << "message_size: " << testCase[1].length() << endl;
         cout << "message: " << testCase[1] << endl;
         cout << "cipher_size: " << cipher_text.length() << endl;
         cout << "cipher: " << cipher_text << endl;
        cout << "decrypted_size: " << decrypted_cipher_message.length() << endl;
        cout << "decrypted: " << decrypted_cipher_message << endl;

        if (!testCase[2].empty()) {
            assert(cipher_text == testCase[2]);
        }
        assert(decrypted_cipher_message == testCase[1]);
    }
}
