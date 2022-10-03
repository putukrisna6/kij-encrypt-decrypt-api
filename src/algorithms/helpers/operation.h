#ifndef OPERATION_H
#define OPERATION_H

#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

/**
 * @brief Shift each char in string.
 *
 * If shifts > 0, then the string will be shifted left.
 * Otherwise, the string will be shifted right.
 *
 * @param s
 * @param shifts
 * @return
 */
string shift(string s, int shifts) {
    size_t length = s.length();
    size_t absShifts = abs(shifts);

    if (absShifts % length == 0) {
        return s;
    }

    if (shifts < 0) {
        absShifts = length - absShifts;
    }

    string res = "";
    for (
            size_t j = 0, currIndex = absShifts % length;
            j < length;
            j++, currIndex = (currIndex + 1) % length)
    {
        res += s[currIndex];
    }

    return res;
}

string shiftLeft(string s, unsigned int shifts) {
    return shift(s, shifts);
}

string shiftRight(string s, unsigned int shifts) {
    return shift(s, shifts * -1);
}

string XOR(string a, string b) {
    string res = "";
    for (int i = 0; i < a.size(); i++) {
        res += (a[i] == b[i]) ? "0" : "1";
    }
    return res;
}

/**
 * @brief Pad a block of binaries based on blocksize using PKCS method.
 * 
 * Each padding byte has a value equal to the total number of padding bytes that are added.
 * For example, if 6 padding bytes must be added, each of those bytes will have the value 0x06.
 * 
 * @param binaries a block of binaries with size equal to 'blockSize'
 * @param blockSize in Byte
 * @return string 
 */
string pkcsPad(string binaries, int blockSize) {
    int diff = blockSize - (binaries.length() / 8) % blockSize;
    if (diff == 0) {
        return binaries;
    }

    string paddingByte = decimalToByteBinary(diff);
    for (int i = 0; i < diff; i++) {
        binaries += paddingByte;
    }

    return binaries;
}

/**
 * @brief Unpad binaries based on blocksize using PKCS method.
 * 
 * If binaries can't be unpadded, the original values will be returned.
 * 
 * @param binaries a block of binaries with size equal to 'blockSize'
 * @param blockSize in Byte
 * @return string 
 */
string pkcsUnpad(string binaries, int blockSize) {
    try {
        if (binaries.length() % blockSize != 0) {
            throw runtime_error("Block cannot be divided by the given block size");
        }

        int padByteIndex = binaries.length() - 8;
        string paddingByte = binaries.substr(padByteIndex, 8);

        int diff = 0;
        try {
            diff = binaryToDecimal(paddingByte);
        } catch (invalid_argument &e) {
            throw runtime_error("Last byte isn't a number");
        }
        

        if (diff == 0) {
            throw runtime_error("Zero padding found instead of PCKS padding");
        }
        if (diff > blockSize) {
            throw runtime_error("Incorrect amount of PKCS padding for block size");
        }

        // Check the correctness of padding bytes by counting occurences.
        padByteIndex -= 8;
        for (int i = 1; i < diff; i++) {
            string pad = binaries.substr(padByteIndex, 8);
            if (pad != paddingByte) {
                throw runtime_error("Invalid PKCS#7 padding encountered");
            }
            padByteIndex -= 8;
        }
        return binaries.substr(0, binaries.length() - diff*8);
    } 
    catch (runtime_error &e) {}

    return binaries;
}

/**
 * @brief Receive a string and splits it into multiple blocks of 'blockSize'.
 * 
 * Add padding bytes using PKCS padding method when a block contain less than 'blockSize'.
 * 
 * @param binaries binary string
 * @param blockSize in Byte
 * @return vector<string> blocks of binaries
 */
vector<string> splitIntoBlocks(string binaries, int blockSize) {
    vector<string> blocks;

    size_t size = binaries.length() / 8; // in byte
    size_t blockCount = size / blockSize;
    int remainder = size % blockSize;

    size_t i = 0;
    while (i < blockCount) {
        string block = binaries.substr(i * 8 * blockSize, 8 * blockSize);
        blocks.push_back(block);
        i++;
    }

    // Add padding byte if needed.
    if (remainder > 0) {
        string block = binaries.substr(i * 8 * blockSize, 8 * remainder);
        block = pkcsPad(block, blockSize);
        blocks.push_back(block);
    }
    return blocks;
}

/**
 *  Multiplication over Finite Fields GF(2^8)
 *
 *
 *  @param first 8-bit binary string
 *  @param second 8-bit binary string
 */
bitset<8> GaloisFieldMul(bitset<8> a, bitset<8> b) {
    bitset<8> p = 0;
    bitset<8> hi_bit_set;
    for (int counter = 0; counter < 8; counter++) {
        if ((b & bitset<8>(1)) != 0) {
            p = p ^ a;
        }
        hi_bit_set = (bitset<8>) (a & bitset<8>(0x80));
        a = a << 1;
        if (hi_bit_set != 0) {
            a = a ^ (bitset<8>) 0x1b; /* x^8 + x^4 + x^3 + x + 1 */
        }
        b = b >> 1;
    }
    return p;
}

#endif