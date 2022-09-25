#ifndef OPERATION_H
#define OPERATION_H

#include <iostream>
using namespace std;

string shiftLeft(string s, int shifts) {
    size_t length = s.length();

    if (shifts % length == 0) {
        return s;
    }

    string res = "";
    for (
        size_t j = 0, currIndex = shifts % length;
        j < length;
        j++, currIndex = (currIndex + 1) % length)
    {
        res += s[currIndex];
    }

    return res;
}

string XOR(string a, string b) {
    string res = "";
    for (int i = 0; i < a.size(); i++) {
        res += (a[i] == b[i]) ? "0" : "1";
    }
    return res;
}

#endif