#ifndef LOG_H
#define LOG_H

#include <iostream>
using namespace std;

bool logIsActive = false;

bool getLog() {
    return logIsActive;
}

void activateLog(bool log) {
    logIsActive = log;
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

#endif