//
// Created by axelb on 30/09/2022.
//
#ifndef AES_V2_H
#define AES_V2_H

#include <cstring>
#include "helpers/convertion.h"
#include "helpers/operation.h"
#include "encryption.h"

using namespace std;

class AES: public Encryption {
public:
    explicit AES(string key, string iv) {
//        setLog(true);
        binKey = (isBinaryString(key)) ? key : stringToBinary(key);
        if (binKey.length() != 128) {
            throw invalid_argument("Invalid key length");
        }
        initializationVector = (isBinaryString(iv)) ? iv : stringToBinary(iv);
        if (initializationVector.length() != 128) {
            throw invalid_argument("Invalid IV length");
        }
        expandedKey = KeyExpansion(binKey);
    }

    /**
    * @brief Encrypt plaintext.
    *
    * If plainText is in binary string, then output will be in binary string
    * If plainText is in ASCII string, then output will be in ASCII string
    *
    * @param plainText Can be in binary string or ASCII string
    * @return string
    */
    string encrypt(string plainText) {
        std::string tag = "Encrypt()";
        log(tag, "==== Begin Encrypting ====");

        bool isBinary = isBinaryString(plainText);
        if (!isBinary) {
            plainText = stringToBinary(plainText);
        }

        size_t blockCount = 1;
        string textToProcess;
        string cipherText;
        string prevCipher;
        vector<string> blocks = splitIntoBlocks(plainText, blockBytesLen);

        for (const auto &block: blocks) {
            if (blockCount == 1) {
                textToProcess = XOR(block, initializationVector);
            } else {
                textToProcess = XOR(block, prevCipher);
            }

            prevCipher = EncryptBlock(textToProcess);
            cipherText += prevCipher;
            blockCount++;
        }

        if (!isBinary) {
            cipherText = binaryToString(cipherText);
        }

        log(tag, "==== Encrypt Finished ====");
        return cipherText;
    }

    string decrypt(string cipherText) {
        string tag = "Decrypt()";
        log(tag, "==== Begin Decrypting ====");

        bool isBinary = isBinaryString(cipherText);
        if (!isBinary) {
            cipherText = stringToBinary(cipherText);
        }

        size_t blockCount = 1;
        string plainText;
        string currentCipher;
        string prevCipher;
        vector<string> blocks = splitIntoBlocks(cipherText, blockBytesLen);

        for (const auto &block: blocks) {
            currentCipher = DecryptBlock(block);

            if (blockCount == 1) {
                plainText += XOR(currentCipher, initializationVector);
            } else {
                plainText += XOR(currentCipher, prevCipher);
            }

            prevCipher = block;
            blockCount++;
        }
        plainText = pkcsUnpad(plainText, blockBytesLen);

        if (!isBinary) {
            plainText = binaryToString(plainText);
        }

        log(tag, "==== Decrypt Finished ====");
        return plainText;
    }

    void setLog(bool log) {
        isLogActive = log;
    }

private:
    string binKey;
    string expandedKey;
    string initializationVector;
    bool isLogActive = false;
    static constexpr unsigned int blockBytesLen = 4 * 4 * sizeof(unsigned char);

    /**
    * @brief Do S-box permutation.
    *
    * @param text 32-bit binary string
    * @param key 32-bit binary string
    * @return string
    */
    string AddRoundKey(string text, string key) {
        unsigned int i;
        string result;
        for (i = 0; i < 4; i++) {
            string a = text.substr(i * 32, 32);
            string b = key.substr(i * 32, 32);
            string temp = XOR(a, b);

            result += temp;
        }
        return result;
    }

    /**
     * @brief Do S-box permutation.
     *
     * @param a 32-bit binary string
     * @return string
     */
    string SubWord(string a) {
        string res = "";
        for (int i = 0; i < 4; i++) {
            int decimal = binaryToDecimal(a.substr(i * 8, 8));
            string sboxBit = byteToBinary(sbox[decimal / blockBytesLen][decimal % blockBytesLen]);
            res += sboxBit;
        }
        return res;
    }

    /**
     * @brief Move one byte to the left.
     *
     * @param a 32-bit binary string
     * @return string
     */
    string RotWord(string a) {
        string res = a.substr(1 * 8, 8) +
                     a.substr(2 * 8, 8) +
                     a.substr(3 * 8, 8) +
                     a.substr(0 * 8, 8);
        return res;
    }

    string Rcon(unsigned int n) {
        unsigned char c = 1;
        for (int i = 0; i < n - 1; i++) {
            c = xtime(c);
        }

        string res = byteToBinary(c);
        for (int i = res.length(); i < 32; i++) {
            res += "0";
        }
        return res;
    }

    unsigned char xtime(unsigned char b) {
        return (b << 1) ^ (((b >> 7) & 1) * 0x1b);
    } // multiply on x


    /**
         * @brief Expand key
         *
         * On AES 128, it will takes 16-byte (16 char) key
         * to produce 176-byte expanded key.
         *
         * @param binKey
         * @return string Expanded key
         */
    string KeyExpansion(string binKey) {
        string res;

        // Get first 4-words
        for (int i = 0; i < 4; i++) {
            string temp = binKey.substr(i * 32, 32);
            res += temp;
//                log("KeyExpansion", "w" + to_string(i) + " = " + binToHex(temp));
        }

        // Get remaining words
        for (int i = 4; i < 44; i++) {
            string temp = res.substr((i - 1) * 32, 32);

            if (i % 4 == 0) {
                string x = RotWord(temp);
                x = SubWord(x);
                string rCon = Rcon(i / 4);
                temp = XOR(x, rCon);
            }
            temp = XOR(res.substr((i - 4) * 32, 32), temp);
//                log("KeyExpansion", "w" + to_string(i) + " = " + binToHex(temp));
            res += temp;
        }


        return res;
    }

    string SubBytes(string plainText) {
        unsigned int i;
        string result;

        for (i = 0; i < 4; i++) {
            string word = plainText.substr(i * 32, 32);
            result += SubWord(word);
        }

        return result;
    }

    string InvSubBytes(string cipherText) {
        string result;

        for (int i = 0; i < 4; i++) {
            string word = cipherText.substr(i * 32, 32);
            string temp;
            for (int j = 0; j < 4; j++) {
                int decimal = binaryToDecimal(word.substr(j * 8, 8));
                temp += byteToBinary(inv_sbox[decimal / blockBytesLen][decimal % blockBytesLen]);;
            }
            result += temp;
        }

        return result;
    }

    string MixColumns(string text) {
        string result;
        bitset<8> mtx[4 * 4];
        bitset<8> arr[4];

        int l = 0;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                bitset<8> byte = stringBinaryToBitset(text.substr((j * 32) + i * 8, 8));
                mtx[l] = byte;
                l++;
            }
        }

        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) { // Take one row from 4x4 matrix
                arr[j] = mtx[i + j * 4];
            }

            mtx[i] = GaloisFieldMul(0x02, arr[0]) ^ GaloisFieldMul(0x03, arr[1]) ^ arr[2] ^ arr[3];
            mtx[i + 4] = arr[0] ^ GaloisFieldMul(0x02, arr[1]) ^ GaloisFieldMul(0x03, arr[2]) ^ arr[3];
            mtx[i + 8] = arr[0] ^ arr[1] ^ GaloisFieldMul(0x02, arr[2]) ^ GaloisFieldMul(0x03, arr[3]);
            mtx[i + 12] = GaloisFieldMul(0x03, arr[0]) ^ arr[1] ^ arr[2] ^ GaloisFieldMul(0x02, arr[3]);
        }

        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result += mtx[i + (j * 4)].to_string();
            }
        }

        return result;
    }

    string InvMixColumns(string cipherText) {
        string result;
        bitset<8> mtx[4 * 4];
        bitset<8> arr[4];

        int l = 0;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                bitset<8> byte = stringBinaryToBitset(cipherText.substr((j * 32) + i * 8, 8));
                mtx[l] = byte;
                l++;
            }
        }

        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                arr[j] = mtx[i + j * 4];
            }

            mtx[i] = GaloisFieldMul(0x0e, arr[0]) ^ GaloisFieldMul(0x0b, arr[1]) ^ GaloisFieldMul(0x0d, arr[2]) ^
                     GaloisFieldMul(0x09, arr[3]);
            mtx[i + 4] = GaloisFieldMul(0x09, arr[0]) ^ GaloisFieldMul(0x0e, arr[1]) ^ GaloisFieldMul(0x0b, arr[2]) ^
                         GaloisFieldMul(0x0d, arr[3]);
            mtx[i + 8] = GaloisFieldMul(0x0d, arr[0]) ^ GaloisFieldMul(0x09, arr[1]) ^ GaloisFieldMul(0x0e, arr[2]) ^
                         GaloisFieldMul(0x0b, arr[3]);
            mtx[i + 12] = GaloisFieldMul(0x0b, arr[0]) ^ GaloisFieldMul(0x0d, arr[1]) ^ GaloisFieldMul(0x09, arr[2]) ^
                          GaloisFieldMul(0x0e, arr[3]);
        }

        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result += mtx[i + (j * 4)].to_string();
            }
        }

        return result;
    }

    string ShiftRows(string text) {
        string result, result2;

        for (int i = 0; i < 4; ++i) {
            string word;
            for (int j = 0; j < 4; ++j) {
                word += text.substr((j * 32) + i * 8, 8);
            }
            result += shiftLeft(word, i * 8);
        }

        for (int i = 0; i < 4; ++i) {
            string word;
            for (int j = 0; j < 4; ++j) {
                word += result.substr((j * 32) + i * 8, 8);
            }
            result2 += word;
        }
        return result2;
    }

    string InvShiftRows(string cipherText) {
        string result, result2;

        for (int i = 0; i < 4; ++i) {
            string word;
            for (int j = 0; j < 4; ++j) {
                word += cipherText.substr((j * 32) + i * 8, 8);
            }
            result += shiftRight(word, i * 8);
        }

        for (int i = 0; i < 4; ++i) {
            string word;
            for (int j = 0; j < 4; ++j) {
                word += result.substr((j * 32) + i * 8, 8);
            }
            result2 += word;
        }
        return result2;
    }

    /**
     * Encrypt a single block (128-bit) of Binary String
     * ex. "0000111100010101..."
     *
     * @param text
     * @return
     */
    string EncryptBlock(string text) {
        string tag = "EncryptBlock";
        string result;
        unsigned int round;


        string key = expandedKey.substr(0, 128);
        result = AddRoundKey(text, key);
        log128BitBinaryStringAs32BitEachRow("AddRoundKey", result);

        for (round = 1; round <= 9; round++) {
            log("EncryptBlock", "Round " + to_string(round) + "\n");

            result = SubBytes(result);
            log128BitBinaryStringAs32BitEachRow("SubBytes", result);

            result = ShiftRows(result);
            log128BitBinaryStringAs32BitEachRow("ShiftRows", result);

            result = MixColumns(result);
            log128BitBinaryStringAs32BitEachRow("MixColumns", result);

            // Get current Round of Key from expandedKey
            string nextRoundKey = expandedKey.substr(round * 128, 128);
            result = AddRoundKey(result, nextRoundKey);
            log128BitBinaryStringAs32BitEachRow("AddRoundKey", result);
            log("", "\n\n");
        }

        result = SubBytes(result);
        log128BitBinaryStringAs32BitEachRow("SubBytes", result);

        result = ShiftRows(result);
        log128BitBinaryStringAs32BitEachRow("ShiftRows", result);

        string nextRoundKey = expandedKey.substr(round * 128, 128);
        result = AddRoundKey(result, nextRoundKey);
        log128BitBinaryStringAs32BitEachRow("AddRoundKey", result);

        return result;
    }

    string DecryptBlock(string cipherText) {
        string tag = "DecryptBlock";
        string result;
        unsigned int round;

        log(tag, "Expanded key length: " + to_string(expandedKey.length()));
        string key = expandedKey.substr(10 * 128, 128);
        result = AddRoundKey(cipherText, key);
        log128BitBinaryStringAs32BitEachRow("AddRoundKey", result);

        for (round = 1; round <= 9; round++) {
            log("EncryptBlock", "Round " + to_string(round) + "\n");

            result = InvShiftRows(result);
            log128BitBinaryStringAs32BitEachRow("ShiftRows", result);

            result = InvSubBytes(result);
            log128BitBinaryStringAs32BitEachRow("SubBytes", result);

            // Get current Round of Key from expandedKey
            string nextRoundKey = expandedKey.substr((10 - round) * 128, 128);
            result = AddRoundKey(result, nextRoundKey);
            log128BitBinaryStringAs32BitEachRow("AddRoundKey", result);


            result = InvMixColumns(result);
            log128BitBinaryStringAs32BitEachRow("MixColumns", result);
            log("", "\n\n");
        }

        result = InvShiftRows(result);
        log128BitBinaryStringAs32BitEachRow("ShiftRows", result);

        result = InvSubBytes(result);
        log128BitBinaryStringAs32BitEachRow("SubBytes", result);

        string nextRoundKey = expandedKey.substr(0, 128);
        result = AddRoundKey(result, nextRoundKey);
        log128BitBinaryStringAs32BitEachRow("AddRoundKey", result);

        return result;

    }

    void log(const std::string &tag, const std::string &message) {
        if (isLogActive) {
            printf("%s | %s\n", tag.c_str(), message.c_str());
        }
    }

    void log128BitBinaryStringAs32BitEachRow(string tag, string message) {
        for (int k = 0; k < message.length() / 32; k++) {
            log(tag, binToHex(message.substr(k * 32, 32)));
        }
    }


    const unsigned char sbox[16][16] = {
            {0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b,
                    0xfe, 0xd7, 0xab, 0x76},
            {0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf,
                    0x9c, 0xa4, 0x72, 0xc0},
            {0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1,
                    0x71, 0xd8, 0x31, 0x15},
            {0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2,
                    0xeb, 0x27, 0xb2, 0x75},
            {0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3,
                    0x29, 0xe3, 0x2f, 0x84},
            {0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39,
                    0x4a, 0x4c, 0x58, 0xcf},
            {0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f,
                    0x50, 0x3c, 0x9f, 0xa8},
            {0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21,
                    0x10, 0xff, 0xf3, 0xd2},
            {0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d,
                    0x64, 0x5d, 0x19, 0x73},
            {0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14,
                    0xde, 0x5e, 0x0b, 0xdb},
            {0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62,
                    0x91, 0x95, 0xe4, 0x79},
            {0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea,
                    0x65, 0x7a, 0xae, 0x08},
            {0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f,
                    0x4b, 0xbd, 0x8b, 0x8a},
            {0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9,
                    0x86, 0xc1, 0x1d, 0x9e},
            {0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9,
                    0xce, 0x55, 0x28, 0xdf},
            {0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f,
                    0xb0, 0x54, 0xbb, 0x16}};

    const unsigned char inv_sbox[16][16] = {
            {0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e,
                    0x81, 0xf3, 0xd7, 0xfb},
            {0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44,
                    0xc4, 0xde, 0xe9, 0xcb},
            {0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b,
                    0x42, 0xfa, 0xc3, 0x4e},
            {0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49,
                    0x6d, 0x8b, 0xd1, 0x25},
            {0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc,
                    0x5d, 0x65, 0xb6, 0x92},
            {0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57,
                    0xa7, 0x8d, 0x9d, 0x84},
            {0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05,
                    0xb8, 0xb3, 0x45, 0x06},
            {0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03,
                    0x01, 0x13, 0x8a, 0x6b},
            {0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce,
                    0xf0, 0xb4, 0xe6, 0x73},
            {0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8,
                    0x1c, 0x75, 0xdf, 0x6e},
            {0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e,
                    0xaa, 0x18, 0xbe, 0x1b},
            {0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe,
                    0x78, 0xcd, 0x5a, 0xf4},
            {0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59,
                    0x27, 0x80, 0xec, 0x5f},
            {0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f,
                    0x93, 0xc9, 0x9c, 0xef},
            {0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c,
                    0x83, 0x53, 0x99, 0x61},
            {0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63,
                    0x55, 0x21, 0x0c, 0x7d}};

/// Galois Multiplication lookup tables
    constexpr static const unsigned char GF_MUL_TABLE[15][256] = {
            {},
            {},

            // mul 2
            {0x00, 0x02, 0x04, 0x06, 0x08, 0x0a, 0x0c, 0x0e, 0x10, 0x12, 0x14, 0x16,
                    0x18, 0x1a, 0x1c, 0x1e, 0x20, 0x22, 0x24, 0x26, 0x28, 0x2a, 0x2c, 0x2e,
                    0x30, 0x32, 0x34, 0x36, 0x38, 0x3a, 0x3c, 0x3e, 0x40, 0x42, 0x44, 0x46,
                    0x48, 0x4a, 0x4c, 0x4e, 0x50, 0x52, 0x54, 0x56, 0x58, 0x5a, 0x5c, 0x5e,
                    0x60, 0x62, 0x64, 0x66, 0x68, 0x6a, 0x6c, 0x6e, 0x70, 0x72, 0x74, 0x76,
                    0x78, 0x7a, 0x7c, 0x7e, 0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c, 0x8e,
                    0x90, 0x92, 0x94, 0x96, 0x98, 0x9a, 0x9c, 0x9e, 0xa0, 0xa2, 0xa4, 0xa6,
                    0xa8, 0xaa, 0xac, 0xae, 0xb0, 0xb2, 0xb4, 0xb6, 0xb8, 0xba, 0xbc, 0xbe,
                    0xc0, 0xc2, 0xc4, 0xc6, 0xc8, 0xca, 0xcc, 0xce, 0xd0, 0xd2, 0xd4, 0xd6,
                    0xd8, 0xda, 0xdc, 0xde, 0xe0, 0xe2, 0xe4, 0xe6, 0xe8, 0xea, 0xec, 0xee,
                    0xf0, 0xf2, 0xf4, 0xf6, 0xf8, 0xfa, 0xfc, 0xfe, 0x1b, 0x19, 0x1f, 0x1d,
                    0x13, 0x11, 0x17, 0x15, 0x0b, 0x09, 0x0f, 0x0d, 0x03, 0x01, 0x07, 0x05,
                    0x3b, 0x39, 0x3f, 0x3d, 0x33, 0x31, 0x37, 0x35, 0x2b, 0x29, 0x2f, 0x2d,
                    0x23, 0x21, 0x27, 0x25, 0x5b, 0x59, 0x5f, 0x5d, 0x53, 0x51, 0x57, 0x55,
                    0x4b, 0x49, 0x4f, 0x4d, 0x43, 0x41, 0x47, 0x45, 0x7b, 0x79, 0x7f, 0x7d,
                    0x73, 0x71, 0x77, 0x75, 0x6b, 0x69, 0x6f, 0x6d, 0x63, 0x61, 0x67, 0x65,
                    0x9b, 0x99, 0x9f, 0x9d, 0x93, 0x91, 0x97, 0x95, 0x8b, 0x89, 0x8f, 0x8d,
                    0x83, 0x81, 0x87, 0x85, 0xbb, 0xb9, 0xbf, 0xbd, 0xb3, 0xb1, 0xb7, 0xb5,
                    0xab, 0xa9, 0xaf, 0xad, 0xa3, 0xa1, 0xa7, 0xa5, 0xdb, 0xd9, 0xdf, 0xdd,
                    0xd3, 0xd1, 0xd7, 0xd5, 0xcb, 0xc9, 0xcf, 0xcd, 0xc3, 0xc1, 0xc7, 0xc5,
                    0xfb, 0xf9, 0xff, 0xfd, 0xf3, 0xf1, 0xf7, 0xf5, 0xeb, 0xe9, 0xef, 0xed,
                    0xe3, 0xe1, 0xe7, 0xe5},

            // mul 3
            {0x00, 0x03, 0x06, 0x05, 0x0c, 0x0f, 0x0a, 0x09, 0x18, 0x1b, 0x1e, 0x1d,
                    0x14, 0x17, 0x12, 0x11, 0x30, 0x33, 0x36, 0x35, 0x3c, 0x3f, 0x3a, 0x39,
                    0x28, 0x2b, 0x2e, 0x2d, 0x24, 0x27, 0x22, 0x21, 0x60, 0x63, 0x66, 0x65,
                    0x6c, 0x6f, 0x6a, 0x69, 0x78, 0x7b, 0x7e, 0x7d, 0x74, 0x77, 0x72, 0x71,
                    0x50, 0x53, 0x56, 0x55, 0x5c, 0x5f, 0x5a, 0x59, 0x48, 0x4b, 0x4e, 0x4d,
                    0x44, 0x47, 0x42, 0x41, 0xc0, 0xc3, 0xc6, 0xc5, 0xcc, 0xcf, 0xca, 0xc9,
                    0xd8, 0xdb, 0xde, 0xdd, 0xd4, 0xd7, 0xd2, 0xd1, 0xf0, 0xf3, 0xf6, 0xf5,
                    0xfc, 0xff, 0xfa, 0xf9, 0xe8, 0xeb, 0xee, 0xed, 0xe4, 0xe7, 0xe2, 0xe1,
                    0xa0, 0xa3, 0xa6, 0xa5, 0xac, 0xaf, 0xaa, 0xa9, 0xb8, 0xbb, 0xbe, 0xbd,
                    0xb4, 0xb7, 0xb2, 0xb1, 0x90, 0x93, 0x96, 0x95, 0x9c, 0x9f, 0x9a, 0x99,
                    0x88, 0x8b, 0x8e, 0x8d, 0x84, 0x87, 0x82, 0x81, 0x9b, 0x98, 0x9d, 0x9e,
                    0x97, 0x94, 0x91, 0x92, 0x83, 0x80, 0x85, 0x86, 0x8f, 0x8c, 0x89, 0x8a,
                    0xab, 0xa8, 0xad, 0xae, 0xa7, 0xa4, 0xa1, 0xa2, 0xb3, 0xb0, 0xb5, 0xb6,
                    0xbf, 0xbc, 0xb9, 0xba, 0xfb, 0xf8, 0xfd, 0xfe, 0xf7, 0xf4, 0xf1, 0xf2,
                    0xe3, 0xe0, 0xe5, 0xe6, 0xef, 0xec, 0xe9, 0xea, 0xcb, 0xc8, 0xcd, 0xce,
                    0xc7, 0xc4, 0xc1, 0xc2, 0xd3, 0xd0, 0xd5, 0xd6, 0xdf, 0xdc, 0xd9, 0xda,
                    0x5b, 0x58, 0x5d, 0x5e, 0x57, 0x54, 0x51, 0x52, 0x43, 0x40, 0x45, 0x46,
                    0x4f, 0x4c, 0x49, 0x4a, 0x6b, 0x68, 0x6d, 0x6e, 0x67, 0x64, 0x61, 0x62,
                    0x73, 0x70, 0x75, 0x76, 0x7f, 0x7c, 0x79, 0x7a, 0x3b, 0x38, 0x3d, 0x3e,
                    0x37, 0x34, 0x31, 0x32, 0x23, 0x20, 0x25, 0x26, 0x2f, 0x2c, 0x29, 0x2a,
                    0x0b, 0x08, 0x0d, 0x0e, 0x07, 0x04, 0x01, 0x02, 0x13, 0x10, 0x15, 0x16,
                    0x1f, 0x1c, 0x19, 0x1a},

            {},
            {},
            {},
            {},
            {},

            // mul 9
            {0x00, 0x09, 0x12, 0x1b, 0x24, 0x2d, 0x36, 0x3f, 0x48, 0x41, 0x5a, 0x53,
                    0x6c, 0x65, 0x7e, 0x77, 0x90, 0x99, 0x82, 0x8b, 0xb4, 0xbd, 0xa6, 0xaf,
                    0xd8, 0xd1, 0xca, 0xc3, 0xfc, 0xf5, 0xee, 0xe7, 0x3b, 0x32, 0x29, 0x20,
                    0x1f, 0x16, 0x0d, 0x04, 0x73, 0x7a, 0x61, 0x68, 0x57, 0x5e, 0x45, 0x4c,
                    0xab, 0xa2, 0xb9, 0xb0, 0x8f, 0x86, 0x9d, 0x94, 0xe3, 0xea, 0xf1, 0xf8,
                    0xc7, 0xce, 0xd5, 0xdc, 0x76, 0x7f, 0x64, 0x6d, 0x52, 0x5b, 0x40, 0x49,
                    0x3e, 0x37, 0x2c, 0x25, 0x1a, 0x13, 0x08, 0x01, 0xe6, 0xef, 0xf4, 0xfd,
                    0xc2, 0xcb, 0xd0, 0xd9, 0xae, 0xa7, 0xbc, 0xb5, 0x8a, 0x83, 0x98, 0x91,
                    0x4d, 0x44, 0x5f, 0x56, 0x69, 0x60, 0x7b, 0x72, 0x05, 0x0c, 0x17, 0x1e,
                    0x21, 0x28, 0x33, 0x3a, 0xdd, 0xd4, 0xcf, 0xc6, 0xf9, 0xf0, 0xeb, 0xe2,
                    0x95, 0x9c, 0x87, 0x8e, 0xb1, 0xb8, 0xa3, 0xaa, 0xec, 0xe5, 0xfe, 0xf7,
                    0xc8, 0xc1, 0xda, 0xd3, 0xa4, 0xad, 0xb6, 0xbf, 0x80, 0x89, 0x92, 0x9b,
                    0x7c, 0x75, 0x6e, 0x67, 0x58, 0x51, 0x4a, 0x43, 0x34, 0x3d, 0x26, 0x2f,
                    0x10, 0x19, 0x02, 0x0b, 0xd7, 0xde, 0xc5, 0xcc, 0xf3, 0xfa, 0xe1, 0xe8,
                    0x9f, 0x96, 0x8d, 0x84, 0xbb, 0xb2, 0xa9, 0xa0, 0x47, 0x4e, 0x55, 0x5c,
                    0x63, 0x6a, 0x71, 0x78, 0x0f, 0x06, 0x1d, 0x14, 0x2b, 0x22, 0x39, 0x30,
                    0x9a, 0x93, 0x88, 0x81, 0xbe, 0xb7, 0xac, 0xa5, 0xd2, 0xdb, 0xc0, 0xc9,
                    0xf6, 0xff, 0xe4, 0xed, 0x0a, 0x03, 0x18, 0x11, 0x2e, 0x27, 0x3c, 0x35,
                    0x42, 0x4b, 0x50, 0x59, 0x66, 0x6f, 0x74, 0x7d, 0xa1, 0xa8, 0xb3, 0xba,
                    0x85, 0x8c, 0x97, 0x9e, 0xe9, 0xe0, 0xfb, 0xf2, 0xcd, 0xc4, 0xdf, 0xd6,
                    0x31, 0x38, 0x23, 0x2a, 0x15, 0x1c, 0x07, 0x0e, 0x79, 0x70, 0x6b, 0x62,
                    0x5d, 0x54, 0x4f, 0x46},

            {},

            // mul 11
            {0x00, 0x0b, 0x16, 0x1d, 0x2c, 0x27, 0x3a, 0x31, 0x58, 0x53, 0x4e, 0x45,
                    0x74, 0x7f, 0x62, 0x69, 0xb0, 0xbb, 0xa6, 0xad, 0x9c, 0x97, 0x8a, 0x81,
                    0xe8, 0xe3, 0xfe, 0xf5, 0xc4, 0xcf, 0xd2, 0xd9, 0x7b, 0x70, 0x6d, 0x66,
                    0x57, 0x5c, 0x41, 0x4a, 0x23, 0x28, 0x35, 0x3e, 0x0f, 0x04, 0x19, 0x12,
                    0xcb, 0xc0, 0xdd, 0xd6, 0xe7, 0xec, 0xf1, 0xfa, 0x93, 0x98, 0x85, 0x8e,
                    0xbf, 0xb4, 0xa9, 0xa2, 0xf6, 0xfd, 0xe0, 0xeb, 0xda, 0xd1, 0xcc, 0xc7,
                    0xae, 0xa5, 0xb8, 0xb3, 0x82, 0x89, 0x94, 0x9f, 0x46, 0x4d, 0x50, 0x5b,
                    0x6a, 0x61, 0x7c, 0x77, 0x1e, 0x15, 0x08, 0x03, 0x32, 0x39, 0x24, 0x2f,
                    0x8d, 0x86, 0x9b, 0x90, 0xa1, 0xaa, 0xb7, 0xbc, 0xd5, 0xde, 0xc3, 0xc8,
                    0xf9, 0xf2, 0xef, 0xe4, 0x3d, 0x36, 0x2b, 0x20, 0x11, 0x1a, 0x07, 0x0c,
                    0x65, 0x6e, 0x73, 0x78, 0x49, 0x42, 0x5f, 0x54, 0xf7, 0xfc, 0xe1, 0xea,
                    0xdb, 0xd0, 0xcd, 0xc6, 0xaf, 0xa4, 0xb9, 0xb2, 0x83, 0x88, 0x95, 0x9e,
                    0x47, 0x4c, 0x51, 0x5a, 0x6b, 0x60, 0x7d, 0x76, 0x1f, 0x14, 0x09, 0x02,
                    0x33, 0x38, 0x25, 0x2e, 0x8c, 0x87, 0x9a, 0x91, 0xa0, 0xab, 0xb6, 0xbd,
                    0xd4, 0xdf, 0xc2, 0xc9, 0xf8, 0xf3, 0xee, 0xe5, 0x3c, 0x37, 0x2a, 0x21,
                    0x10, 0x1b, 0x06, 0x0d, 0x64, 0x6f, 0x72, 0x79, 0x48, 0x43, 0x5e, 0x55,
                    0x01, 0x0a, 0x17, 0x1c, 0x2d, 0x26, 0x3b, 0x30, 0x59, 0x52, 0x4f, 0x44,
                    0x75, 0x7e, 0x63, 0x68, 0xb1, 0xba, 0xa7, 0xac, 0x9d, 0x96, 0x8b, 0x80,
                    0xe9, 0xe2, 0xff, 0xf4, 0xc5, 0xce, 0xd3, 0xd8, 0x7a, 0x71, 0x6c, 0x67,
                    0x56, 0x5d, 0x40, 0x4b, 0x22, 0x29, 0x34, 0x3f, 0x0e, 0x05, 0x18, 0x13,
                    0xca, 0xc1, 0xdc, 0xd7, 0xe6, 0xed, 0xf0, 0xfb, 0x92, 0x99, 0x84, 0x8f,
                    0xbe, 0xb5, 0xa8, 0xa3},

            {},

            // mul 13
            {0x00, 0x0d, 0x1a, 0x17, 0x34, 0x39, 0x2e, 0x23, 0x68, 0x65, 0x72, 0x7f,
                    0x5c, 0x51, 0x46, 0x4b, 0xd0, 0xdd, 0xca, 0xc7, 0xe4, 0xe9, 0xfe, 0xf3,
                    0xb8, 0xb5, 0xa2, 0xaf, 0x8c, 0x81, 0x96, 0x9b, 0xbb, 0xb6, 0xa1, 0xac,
                    0x8f, 0x82, 0x95, 0x98, 0xd3, 0xde, 0xc9, 0xc4, 0xe7, 0xea, 0xfd, 0xf0,
                    0x6b, 0x66, 0x71, 0x7c, 0x5f, 0x52, 0x45, 0x48, 0x03, 0x0e, 0x19, 0x14,
                    0x37, 0x3a, 0x2d, 0x20, 0x6d, 0x60, 0x77, 0x7a, 0x59, 0x54, 0x43, 0x4e,
                    0x05, 0x08, 0x1f, 0x12, 0x31, 0x3c, 0x2b, 0x26, 0xbd, 0xb0, 0xa7, 0xaa,
                    0x89, 0x84, 0x93, 0x9e, 0xd5, 0xd8, 0xcf, 0xc2, 0xe1, 0xec, 0xfb, 0xf6,
                    0xd6, 0xdb, 0xcc, 0xc1, 0xe2, 0xef, 0xf8, 0xf5, 0xbe, 0xb3, 0xa4, 0xa9,
                    0x8a, 0x87, 0x90, 0x9d, 0x06, 0x0b, 0x1c, 0x11, 0x32, 0x3f, 0x28, 0x25,
                    0x6e, 0x63, 0x74, 0x79, 0x5a, 0x57, 0x40, 0x4d, 0xda, 0xd7, 0xc0, 0xcd,
                    0xee, 0xe3, 0xf4, 0xf9, 0xb2, 0xbf, 0xa8, 0xa5, 0x86, 0x8b, 0x9c, 0x91,
                    0x0a, 0x07, 0x10, 0x1d, 0x3e, 0x33, 0x24, 0x29, 0x62, 0x6f, 0x78, 0x75,
                    0x56, 0x5b, 0x4c, 0x41, 0x61, 0x6c, 0x7b, 0x76, 0x55, 0x58, 0x4f, 0x42,
                    0x09, 0x04, 0x13, 0x1e, 0x3d, 0x30, 0x27, 0x2a, 0xb1, 0xbc, 0xab, 0xa6,
                    0x85, 0x88, 0x9f, 0x92, 0xd9, 0xd4, 0xc3, 0xce, 0xed, 0xe0, 0xf7, 0xfa,
                    0xb7, 0xba, 0xad, 0xa0, 0x83, 0x8e, 0x99, 0x94, 0xdf, 0xd2, 0xc5, 0xc8,
                    0xeb, 0xe6, 0xf1, 0xfc, 0x67, 0x6a, 0x7d, 0x70, 0x53, 0x5e, 0x49, 0x44,
                    0x0f, 0x02, 0x15, 0x18, 0x3b, 0x36, 0x21, 0x2c, 0x0c, 0x01, 0x16, 0x1b,
                    0x38, 0x35, 0x22, 0x2f, 0x64, 0x69, 0x7e, 0x73, 0x50, 0x5d, 0x4a, 0x47,
                    0xdc, 0xd1, 0xc6, 0xcb, 0xe8, 0xe5, 0xf2, 0xff, 0xb4, 0xb9, 0xae, 0xa3,
                    0x80, 0x8d, 0x9a, 0x97},

            // mul 14
            {0x00, 0x0e, 0x1c, 0x12, 0x38, 0x36, 0x24, 0x2a, 0x70, 0x7e, 0x6c, 0x62,
                    0x48, 0x46, 0x54, 0x5a, 0xe0, 0xee, 0xfc, 0xf2, 0xd8, 0xd6, 0xc4, 0xca,
                    0x90, 0x9e, 0x8c, 0x82, 0xa8, 0xa6, 0xb4, 0xba, 0xdb, 0xd5, 0xc7, 0xc9,
                    0xe3, 0xed, 0xff, 0xf1, 0xab, 0xa5, 0xb7, 0xb9, 0x93, 0x9d, 0x8f, 0x81,
                    0x3b, 0x35, 0x27, 0x29, 0x03, 0x0d, 0x1f, 0x11, 0x4b, 0x45, 0x57, 0x59,
                    0x73, 0x7d, 0x6f, 0x61, 0xad, 0xa3, 0xb1, 0xbf, 0x95, 0x9b, 0x89, 0x87,
                    0xdd, 0xd3, 0xc1, 0xcf, 0xe5, 0xeb, 0xf9, 0xf7, 0x4d, 0x43, 0x51, 0x5f,
                    0x75, 0x7b, 0x69, 0x67, 0x3d, 0x33, 0x21, 0x2f, 0x05, 0x0b, 0x19, 0x17,
                    0x76, 0x78, 0x6a, 0x64, 0x4e, 0x40, 0x52, 0x5c, 0x06, 0x08, 0x1a, 0x14,
                    0x3e, 0x30, 0x22, 0x2c, 0x96, 0x98, 0x8a, 0x84, 0xae, 0xa0, 0xb2, 0xbc,
                    0xe6, 0xe8, 0xfa, 0xf4, 0xde, 0xd0, 0xc2, 0xcc, 0x41, 0x4f, 0x5d, 0x53,
                    0x79, 0x77, 0x65, 0x6b, 0x31, 0x3f, 0x2d, 0x23, 0x09, 0x07, 0x15, 0x1b,
                    0xa1, 0xaf, 0xbd, 0xb3, 0x99, 0x97, 0x85, 0x8b, 0xd1, 0xdf, 0xcd, 0xc3,
                    0xe9, 0xe7, 0xf5, 0xfb, 0x9a, 0x94, 0x86, 0x88, 0xa2, 0xac, 0xbe, 0xb0,
                    0xea, 0xe4, 0xf6, 0xf8, 0xd2, 0xdc, 0xce, 0xc0, 0x7a, 0x74, 0x66, 0x68,
                    0x42, 0x4c, 0x5e, 0x50, 0x0a, 0x04, 0x16, 0x18, 0x32, 0x3c, 0x2e, 0x20,
                    0xec, 0xe2, 0xf0, 0xfe, 0xd4, 0xda, 0xc8, 0xc6, 0x9c, 0x92, 0x80, 0x8e,
                    0xa4, 0xaa, 0xb8, 0xb6, 0x0c, 0x02, 0x10, 0x1e, 0x34, 0x3a, 0x28, 0x26,
                    0x7c, 0x72, 0x60, 0x6e, 0x44, 0x4a, 0x58, 0x56, 0x37, 0x39, 0x2b, 0x25,
                    0x0f, 0x01, 0x13, 0x1d, 0x47, 0x49, 0x5b, 0x55, 0x7f, 0x71, 0x63, 0x6d,
                    0xd7, 0xd9, 0xcb, 0xc5, 0xef, 0xe1, 0xf3, 0xfd, 0xa7, 0xa9, 0xbb, 0xb5,
                    0x9f, 0x91, 0x83, 0x8d}};

/// circulant MDS matrix
    constexpr static const unsigned char CMDS[4][4] = {
            {2, 3, 1, 1},
            {1, 2, 3, 1},
            {1, 1, 2, 3},
            {3, 1, 1, 2}};

/// Inverse circulant MDS matrix
    constexpr static const unsigned char INV_CMDS[4][4] = {
            {14, 11, 13, 9},
            {9,  14, 11, 13},
            {13, 9,  14, 11},
            {11, 13, 9,  14}};

};


#endif //KIJ_ENCRYPT_DECRYPT_API_AES_V2_H