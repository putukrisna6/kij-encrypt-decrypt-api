#include <assert.h>
#include "des.h"

/**
 * @brief Store key, plaintext, and expected cipher text value
 * 
 * Input is expected to be in binary.
 * 
 * Format:
 *      * key
 *      * plaintext
 *      * expected ciphertext
 */
vector<vector<string>> testCases{
    { /* William Stallings Book Testcase */
        "0000111100010101011100011100100101000111110110011110100001011001", 
        "0000001001000110100010101100111011101100101010000110010000100000", 
        "1101101000000010110011100011101010001001111011001010110000111011"
    },
    { /* GeeksForGeeks Testcase */
        "1010101010111011000010010001100000100111001101101100110011011101", 
        "0001001000110100010101101010101111001101000100110010010100110110", 
        "1100000010110111101010001101000001011111001110101000001010011100"
    },
};

int main()
{
    for (int i = 0; i < testCases.size(); i++) {
        vector<string> testCase = testCases[i];

        DES *des = new DES(testCase[0]);
        des->setLog(true);

        string cipherText = des->encrypt(testCase[1]);
        assert(cipherText == stringToUppercase(testCase[2]));

        string decryptedCipherText = des->decrypt(cipherText);
        assert(decryptedCipherText == stringToUppercase(testCase[1]));

        cout << "Testcase " << i + 1 << " succeed" << endl;
    }

    cout << "DES test has been ran without any error." << endl;

    return 0;
}
