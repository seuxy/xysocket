//
// Created by dingwenjiang on 17-9-9.
//

#include <stdio.h>
#include "Util.h"

#define LOG_PREFIX      "[cppWebSockets] "

void Util::log(const string &message) {
    const string &logMessage = LOG_PREFIX + message;
    printf("%s\n", logMessage.c_str());
}

void Util::log(const char *message) {
    log(string(message));
}

vector<string> Util::split(const string &str, const char &delimiter) {
    vector<string> res;
    int start = 0;
    for (int i = 0; i < str.size(); i++) {
        if (str[i] == delimiter) {
            res.push_back(str.substr(start,i-start));
            start = i+1;
        }
    }
    if (start < str.size())
        res.push_back(str.substr(start));
    return res;
}
