#include <cassert>
#include "des.h"

int main()
{
    /**
     * @brief Store key, plaintext, and expected cipher text value
     *
     * Format:
     *      * key
     *      * plaintext
     *      * expected ciphertext = if empty, the expected ciphertext wont be checked
     */
    vector<vector<string>> testCases{
        { /* William Stallings Book Testcase */
            "0000111100010101011100011100100101000111110110011110100001011001",
            "0000001001000110100010101100111011101100101010000110010000100000",
            ""
        },
        { /* GeeksForGeeks Testcase */
            "1010101010111011000010010001100000100111001101101100110011011101",
            "0001001000110100010101101010101111001101000100110010010100110110",
            ""
        },
        { /* 64-bit (1 block) Plaintext Testcase */
            "8_chars_",
            "8_chars_",
            ""
        },
        { /* 128-bit (2 block) Plaintext Testcase */
            "8_chars_",
            "16_long_strings_",
            ""
        },
        { /**
            * 127-bit Plaintext Testcase
            * (1 full-block and 1 block with 1 padding byte)
            */
            "8_chars_",
            "15_long_strings",
            ""
        },
        { /**
            * 129-bit Plaintext Testcase
            * (2 full-block and 1 block with 7 padding byte)
            */
            "8_chars_",
            "17_long_strings__",
            ""
        },
        { /**
            * 56-bit Binary Plaintext Testcase
            * (1 block with 1 padding byte)
            */
            "1010101010111011000010010001100000100111001101101100110011011101",
            "00010010001101000101011010101011110011010001001100100101",
            ""
        },
        { /** Multiple word testcase */
            "8_chars_",
            "lorem ipsum dolor sit amet",
            ""
        },
    };

    const string IV = "_iv_key_";

    for (int i = 0; i < testCases.size(); i++) {
        vector<string> testCase = testCases[i];

        DES *des = new DES(testCase[0], IV);
        // des->setLog(true);

        string cipherText = des->encrypt(testCase[1]);
        if (testCase[2] != "") {
            assert(cipherText == testCase[2]);
        }

        string decryptedCipherText = des->decrypt(cipherText);
        assert(decryptedCipherText == testCase[1]);

        cout << "Testcase " << i + 1 << " succeed" << endl;
    }

    cout << "DES test has been ran without any error." << endl;

    return 0;
}