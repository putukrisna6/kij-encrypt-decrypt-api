#include <iostream>
#include <unordered_map>
#include <bitset>

using namespace std;

#ifndef HELPER_H
#define HELPER_H

string binToHex(string s)
{
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

string hexToBin(string s)
{
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
    for (char& _char : s) {
        bin +=bitset<8>(_char).to_string();
    }
    return bin;
}

string shiftLeft(string s, int shifts)
{
    size_t length = s.length();

    if (shifts % length == 0) {
        return s;
    }

    string res = "";
    for (
        size_t j = 0, currIndex = shifts % length; 
        j < length - 1; 
        j++, currIndex = (currIndex + 1) % length
    ) {
        res += s[currIndex];
    }

    return res;
}

#endif