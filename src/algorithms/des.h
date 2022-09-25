#include "encryption.h"
#include "helper.h"
#include <vector>

using namespace std;

typedef unsigned short int usi;

class DES : public Encryption {
    // TODO: Handle string as encryption input
    // TODO: Add sanity check for input (key, plaintext, ciphertext)
    // TODO: Handle file as encryption input
    public:
        // Key is expected to be in hexadecimal
        DES(string key) {
            bin64Key = hexToBin(stringToUppercase(key));
            generateRoundKeys();
        }

        string encrypt(string plainText) {
            log("Encryption:\n");

            string cipherText = __encrypt(
                stringToUppercase(plainText), encryptionRoundKeys
            );
            cipherText = binToHex(cipherText);
            log("--------------------------------------------");
            log("Ciphertext: " + cipherText + "\n");

            return cipherText;
        }

        string decrypt(string cipherText) {
            log("Decryption:\n");

            string plainText = __encrypt(
                stringToUppercase(cipherText),
                decryptionRoundKeys
            );
            plainText = binToHex(plainText);
            log("--------------------------------------------");
            log("Plaintext: " + plainText + "\n");

            return plainText;
        }

        bool getLog() {
            return logIsActive;
        }

        void setLog(bool log) {
            logIsActive = log;
        }

    private:
        /*********************************************************************
         * Variables
         *********************************************************************/
        string bin64Key;
        vector<string> encryptionRoundKeys; // in binary
        vector<string> decryptionRoundKeys; // in binary
        bool logIsActive;

        /* number of bits to shift on key for each round */
        static constexpr int ls[16] = { 
            1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 
        };

        /* permuted choice table for create bin56Key */
        static constexpr usi pc1[56] = {
            57, 49, 41, 33, 25, 17, 9,
            1, 58, 50, 42, 34, 26, 18,
            10, 2, 59, 51, 43, 35, 27,
            19, 11, 3, 60, 52, 44, 36,
            63, 55, 47, 39, 31, 23, 15,
            7, 62, 54, 46, 38, 30, 22,
            14, 6, 61, 53, 45, 37, 29,
            21, 13, 5, 28, 20, 12, 4 
        };

        /* permuted choice table for key compression permutation */
        static constexpr usi pc2[48] = {
            14, 17, 11, 24, 1, 5,
            3, 28, 15, 6, 21, 10,
            23, 19, 12, 4, 26, 8,
            16, 7, 27, 20, 13, 2,
            41, 52, 31, 37, 47, 55,
            30, 40, 51, 45, 33, 48,
            44, 49, 39, 56, 34, 53,
            46, 42, 50, 36, 29, 32 
        };

        /* initial permutation */
        static constexpr usi ip[64] = {
            58, 50, 42, 34, 26, 18, 10, 2,
            60, 52, 44, 36, 28, 20, 12, 4,
            62, 54, 46, 38, 30, 22, 14, 6,
            64, 56, 48, 40, 32, 24, 16, 8,
            57, 49, 41, 33, 25, 17, 9, 1,
            59, 51, 43, 35, 27, 19, 11, 3,
            61, 53, 45, 37, 29, 21, 13, 5,
            63, 55, 47, 39, 31, 23, 15, 7 
        };

        /* expansion permutation table*/
        static constexpr usi ep[48] = {
            32, 1, 2, 3, 4, 5,
            4, 5, 6, 7, 8, 9,
            8, 9, 10, 11, 12, 13,
            12, 13, 14, 15, 16, 17,
            16, 17, 18, 19, 20, 21,
            20, 21, 22, 23, 24, 25,
            24, 25, 26, 27, 28, 29,
            28, 29, 30, 31, 32, 1 
        };

        /* s-box */
        static constexpr usi sb[8][4][16] = {
            {/* S1 */
                {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
                {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
                {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
                {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
            }, 
            {/* S2 */
                {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
                {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
                {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
                {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
            }, 
            {/* S3 */
                {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
                {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
                {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
                {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
            },
            {/* S4 */
                {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
                {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
                {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
                {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}
            },
            {/* S5 */
                {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
                {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
                {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
                {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}
            },
            {/* S6 */
                {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
                {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
                {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
                {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}
            },
            {/* S7 */
                {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
                {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
                {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
                {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}
            },
            {/* S8 */
                {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
                {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
                {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
                {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
            }
        };

        /* p-box */
        static constexpr usi pb[32] = {
            16, 7, 20, 21, 29, 12, 28, 17,
            1, 15, 23, 26, 5, 18, 31, 10,
            2, 8, 24, 14, 32, 27, 3, 9,
            19, 13, 30, 6, 22, 11, 4, 25 
        };

        /* final permutation */
        static constexpr usi fp[64] = {
            40, 8, 48, 16, 56, 24, 64, 32,
            39, 7, 47, 15, 55, 23, 63, 31,
            38, 6, 46, 14, 54, 22, 62, 30,
            37, 5, 45, 13, 53, 21, 61, 29,
            36, 4, 44, 12, 52, 20, 60, 28,
            35, 3, 43, 11, 51, 19, 59, 27,
            34, 2, 42, 10, 50, 18, 58, 26,
            33, 1, 41, 9, 49, 17, 57, 25 
        };

        /**********************************************************************
         * Functions
         **********************************************************************/

        string __encrypt(string plainText, vector<string> roundKeys) {
            // Initial permutation
            string binPT = hexToBin(plainText);
            binPT = permute(binPT, ip, 64);
            log("After initial permutation: " + binToHex(binPT));

            // Splitting
            string left = binPT.substr(0, 32);
            string right = binPT.substr(32, 32);
            char buffer[64];
            snprintf(buffer, 64,
                "After splitting: LPT=%s RPT=%s\n",
                binToHex(left).c_str(), binToHex(right).c_str()
            );
            log(convertToString(buffer));

            // 16 Rounds of DES functions
            log("Round XX |   LPT    |   RPT    | Round Keys");
            log("--------------------------------------------");
            for (usi i = 0; i < 16; i++) {
                // Expansion Permutation
                string RPT = permute(right, ep, 48);
                RPT = XOR(roundKeys[i], RPT);

                // S-box Permutation
                RPT = sBoxPermutation(RPT);

                // P-box Permutation
                RPT = permute(RPT, pb, 32);

                // XOR RPT with original LPT in current round
                RPT = XOR(RPT, left);

                // Swap left and right part
                left = RPT;
                if (i < 15) {
                    swap(left, right);
                }

                char buffer[64];
                sprintf(
                    buffer, "Round %02d | %s | %s | %s", (i + 1),
                    binToHex(left).c_str(), binToHex(right).c_str(),
                    binToHex(roundKeys[i]).c_str());
                log(convertToString(buffer));
            }

            // Final Permutation
            return permute(left + right, fp, 64);
        }

        void log(string str) {
            if (!logIsActive) {
                return;
            }

            time_t currTime;
            tm* currTm;
            char formatedTime[100];

            time(&currTime);
            currTm = localtime(&currTime);
            strftime(formatedTime, 50, "%T", currTm);

            printf("%s - %s\n", formatedTime, str.c_str());
        }

        /**
         * @brief Do S-Box permutation.
         *
         * @param xepRight Result of XOR operation between
         *                  48-bit round key with 48-bit expanded RPT.
         * @return string
         */
        string sBoxPermutation(string xepRight) {
            string result = "";

            // Divide into 8 blocks.
            for (usi i = 0; i < 8; i++) {
                int startIndex = i * 6;

                // Determine row in s-box Table;
                string outerBits = string(1, xepRight[startIndex]) + xepRight[startIndex + 5];
                int row = binaryToDecimal(outerBits);

                // Determine col in s-box Table;
                string middleBits = xepRight.substr(startIndex + 1, 4);
                int col = binaryToDecimal(middleBits);

                // Val must be a 4-bit string
                string val = decimalToBinary(sb[i][row][col]);
                for (int j = val.length(); j < 4; j++) {
                    val = "0" + val;
                    }
                result += val;
            }
            return result;
        }

        /**
         * @brief Reorder elements in 'k' based on array 'arr' containing 'n' elements.
         *
         * First element in 'res' is equal to 'k[i]', where 'i' is
         * equal to first element of 'arr'.
         *
         * @param k Array of byte that want to be reordered.
         * @param arr Permutation table that store reorder rule.
         * @param n Store how many elements does arr contains.
         * @return string
         */
        string permute(string k, const usi* arr, usi n) {
            string res = "";
            for (usi i = 0; i < n; i++) {
                res += k[arr[i] - 1];
            }
            return res;
        }

        /**
         * @brief Generate keys for each of the 16 DES rounds.
         */
        void generateRoundKeys() {
            string bin56Key = permute(bin64Key, pc1, 56);
            string left = bin56Key.substr(0, 28);
            string right = bin56Key.substr(28, 28);

            // Set encryption round keys.
            for (usi i = 0; i < 16; i++) {
                left = shiftLeft(left, ls[i]);
                right = shiftLeft(right, ls[i]);

                string combine = left + right;

                string roundKey = permute(combine, pc2, 48);

                encryptionRoundKeys.push_back(roundKey);
            }

            // Set decryption round keys.
            for (int i = 15; i >= 0; i--) {
                decryptionRoundKeys.push_back(encryptionRoundKeys[i]);
            }
        }
};