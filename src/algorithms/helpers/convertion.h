#ifndef CONVERTION_H
#define CONVERTION_H

#include <iostream>
#include <unordered_map>
#include <bitset>
using namespace std;

string binToHex(string s) {
    // binary to hexadecimal conversion
    unordered_map<string, string> mp;
    mp["0000"] = "0";
    mp["0001"] = "1";
    mp["0010"] = "2";
    mp["0011"] = "3";
    mp["0100"] = "4";
    mp["0101"] = "5";
    mp["0110"] = "6";
    mp["0111"] = "7";
    mp["1000"] = "8";
    mp["1001"] = "9";
    mp["1010"] = "A";
    mp["1011"] = "B";
    mp["1100"] = "C";
    mp["1101"] = "D";
    mp["1110"] = "E";
    mp["1111"] = "F";

    string hex = "";
    for (size_t i = 0; i < s.length(); i += 4) {
        string ch = s.substr(i, 4);
        hex += mp[ch];
    }
    return hex;
}

string hexToBin(string s) {
    // hexadecimal to binary conversion
    unordered_map<char, string> mp;
    mp['0'] = "0000";
    mp['1'] = "0001";
    mp['2'] = "0010";
    mp['3'] = "0011";
    mp['4'] = "0100";
    mp['5'] = "0101";
    mp['6'] = "0110";
    mp['7'] = "0111";
    mp['8'] = "1000";
    mp['9'] = "1001";
    mp['A'] = "1010";
    mp['B'] = "1011";
    mp['C'] = "1100";
    mp['D'] = "1101";
    mp['E'] = "1110";
    mp['F'] = "1111";

    string bin = "";
    for (size_t i = 0; i < s.length(); i++) {
        bin += mp[s[i]];
    }
    return bin;
}

string stringToBinary(string s) {
    string bin = "";
    for (char &_char : s) {
        bin += bitset<8>(_char).to_string();
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

unsigned int binaryToDecimal(string d) {
    return stoi(d, 0, 2);
}

string convertToString(char *a) {
    string s = a;
    return a;
}

#endif