#ifndef CONVERTION_H
#define CONVERTION_H

#include <iostream>
#include <unordered_map>
#include <bitset>
using namespace std;

// binary to hexadecimal conversion
static unordered_map<string, string> bh {
    { "0000", "0" },
    { "0001", "1" },
    { "0010", "2" },
    { "0011", "3" },
    { "0100", "4" },
    { "0101", "5" },
    { "0110", "6" },
    { "0111", "7" },
    { "1000", "8" },
    { "1001", "9" },
    { "1010", "A" },
    { "1011", "B" },
    { "1100", "C" },
    { "1101", "D" },
    { "1110", "E" },
    { "1111", "F" },
};

// hexadecimal to binary conversion
static unordered_map<char, string> hb {
    { '0', "0000" },
    { '1', "0001" },
    { '2', "0010" },
    { '3', "0011" },
    { '4', "0100" },
    { '5', "0101" },
    { '6', "0110" },
    { '7', "0111" },
    { '8', "1000" },
    { '9', "1001" },
    { 'A', "1010" },
    { 'B', "1011" },
    { 'C', "1100" },
    { 'D', "1101" },
    { 'E', "1110" },
    { 'F', "1111" },
};

string binToHex(string s) {
    string hex = "";
    for (size_t i = 0; i < s.length(); i += 4) {
        string ch = s.substr(i, 4);
        hex += bh[ch];
    }
    return hex;
}

string hexToBin(string s) {
    string bin = "";
    for (size_t i = 0; i < s.length(); i++) {
        bin += hb[s[i]];
    }
    return bin;
}

string byteToBinary(char b) {
    return bitset<8>(b).to_string();
}

string stringToBinary(string s) {
    string bin = "";
    for (char &_char : s) {
        bin += byteToBinary(_char);
    }
    return bin;
}

string binaryToString(string b) {
    string str = "";
    size_t i = 0;

    while (i < b.length()) {
        string byte = b.substr(i, 8);
        char c = strtol(byte.c_str(), 0, 2);
        str += c;
        i += 8;
    }
    return str;
}

string stringToUppercase(string s) {
    for (size_t i = 0; i < s.length(); i++) {
        if (!isalpha(s[i]) || isupper(s[i])) {
            continue;
        }
        s[i] = toupper(s[i]);
    }
    return s;
}

string decimalToBinary(unsigned int d) {
    string res = "";
    while (d > 0) {
        res = to_string(d % 2) + res;
        d /= 2;
    }
    return res;
}

/**
 * @brief Add leading zero to make output in a byte form.
 */
string decimalToByteBinary(unsigned int d) {
    string res = decimalToBinary(d);

    int remainder = res.length() % 8;
    if (remainder > 0) {
        string padder = "";
        for (int i = 8 - remainder; i > 0; i--) {
            padder += "0";
        }
        res = padder + res;
    }

    return res;
}

unsigned int binaryToDecimal(string d) {
    return stoi(d, 0, 2);
}

string convertToString(char *a) {
    string s = a;
    return a;
}

bool isBinaryString(string str) {
    for(size_t i = 0; i < str.length(); i++) {
       if(str[i] != '1' && str[i]!='0') return false;
    }
    return true; 
}

/**
 * Convert String of binary to a bitset<8>
 *
 * @param binaryString
 * @return 8 bit in form of bitset
 */
bitset<8> stringBinaryToBitset(string binaryString) {
    if (!isBinaryString(binaryString)) {
        throw runtime_error("Text is not binary string");
    }
    bitset<8> binary(binaryString);
    return binary;
}
#endif