//
// Created by dingwenjiang on 17-9-9.
//

#ifndef MYWEBSOCKET_UTIL_H
#define MYWEBSOCKET_UTIL_H

#include <string>
#include <sstream>
#include <vector>
using std::string;
using std::stringstream;
using std::vector;

class Util {
public:
    static void log(const string &message);

    static void log(const char *message);

    template<typename T>
    static inline string toString(T t) {
        stringstream s;
        s << t;
        return s.str();
    }

    static vector<string> split(const string &str, const char &delimiter);


    enum MSGTYPE{
        DEFAULT,
        LOGIN,
        MEG
    };

    static MSGTYPE converMsg(const string& strType){

    }
};


#endif //MYWEBSOCKET_UTIL_H
