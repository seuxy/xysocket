//
// Created by joyce on 17-9-11.
//

#ifndef MYWEBSOCKET_MYSQL_CONNECTION_H
#define MYWEBSOCKET_MYSQL_CONNECTION_H

#endif //MYWEBSOCKET_MYSQL_CONNECTION_H

#include <iostream>
#include <string>
#include <list>
#include <memory>

#include <cppconn/driver.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <exception>

#include <mutex>

using namespace std;
using namespace sql;

class connectionPool {
public:
    static connectionPool* getInstance();
    auto getConnect()->shared_ptr<Connection>;
    auto returnConnect(shared_ptr<Connection> &ret)->void;
    ~connectionPool();
    auto getPoolSize()->int;

private:
    connectionPool(string name, string pwd, string nurl, int maxSize) ;
    auto initPool(int initSize)->void ;
    auto destroyOneConn()->void ;
    auto destroyPool()->void ;
    auto expandPool(int size)->void ;
    auto reducePool(int size)->void ;
    auto addConn(int size)->void ;

private:
    string username;
    string password;
    string url;
    int maxSize;
    int poolSize;


    list<shared_ptr<Connection>> connList;
    static connectionPool *pool;
    mutex lock;
    Driver *driver;
};
