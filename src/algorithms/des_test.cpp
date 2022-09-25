#include <iostream>
#include <assert.h>
#include "des.h"
#include "helpers/helper.h"
using namespace std;

/**
 * @brief Store key, plaintext, and expected cipher text value
 * 
 * Format:
 *      * key
 *      * plaintext
 *      * expected ciphertext
 */
vector<vector<string>> testCases{
    {
        "0f1571c947d9e859", 
        "02468aceeca86420", 
        "da02ce3a89ecac3b"
    },
    {
        "AABB09182736CCDD", 
        "123456ABCD132536", 
        "C0B7A8D05F3A829C"
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
