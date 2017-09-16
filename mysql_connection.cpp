//
// Created by joyce on 17-9-11.
//
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "mysql_connection.h"

connectionPool* connectionPool:: pool = NULL;

connectionPool::connectionPool(string name, string pwd, string nurl, int maxsize) :
    username(name),password(pwd),url(nurl),maxSize(maxsize),poolSize(0)
{
    driver = get_driver_instance();
    initPool(maxSize/2);
}

connectionPool::~connectionPool() {
    destroyPool();
}

int connectionPool::getPoolSize() {
    return poolSize;
}

void connectionPool::addConn(int size) {
    for (int i = 0; i < size; i++) {
        Connection *conn = driver->connect(url,username,password);
        shared_ptr<Connection> sp(conn,[](Connection *conn){delete conn;});
        connList.push_back(move(sp));
    }
    poolSize += size;
}

void connectionPool::initPool(int initSize) {
    lock_guard<mutex> locker(lock);
    addConn(initSize);
}

void connectionPool::destroyPool() {
    for (auto &conn : connList) {
        shared_ptr<Connection> && sp = move(connList.front());
        sp->close();
    }
}

void connectionPool::destroyOneConn() {
    shared_ptr<Connection> && sp = move(connList.front());
    sp->close();
    --poolSize;
}

void connectionPool::expandPool(int size) {
    lock_guard<mutex> locker(lock);
    addConn(size);
    poolSize += size;
}

void connectionPool::reducePool(int size) {
    lock_guard<mutex> locker(lock);
    if (size > poolSize) {
        return;
    }
    for (int i = 0; i < size; i++)
        destroyOneConn();

    poolSize -= size;
}

connectionPool* connectionPool::getInstance() {
    if (pool == NULL) {
        pool = new connectionPool("root","19930722","localhost",40);
    }
    return pool;
}

shared_ptr<Connection> connectionPool::getConnect() {
    lock_guard<mutex> locker(lock);
    if(connList.size() > 0) {
        shared_ptr<Connection> sp = move(connList.front());
        connList.pop_front();
        return sp;
    }
    else {
        if (poolSize < maxSize) {
            addConn(1);
            shared_ptr<Connection> sp = move(connList.front());
            connList.pop_front();
            return sp;
        }
        else
            return NULL;
    }
}

void connectionPool::returnConnect(shared_ptr<Connection> &ret) {
    lock_guard<mutex> locker(lock);
    connList.push_back(ret);
}