//
// Created by joyce on 17-9-12.
//

#include <assert.h>
#include <vector>
#include <ctime>
#include "mysql_connection.h"

connectionPool *pool = connectionPool::getInstance();

string IsUser(string name, string pwd) {
    shared_ptr<Connection> con;
    Statement *state;
    ResultSet *res;
    string result;

    con = pool->getConnect();
    while(!con) {
        sleep(1);
    }
    state = con->createStatement();
    state->execute("use danmu");

    res = state->executeQuery(" select * from user where uname = '"+name+"' " );
    if (!res->next())
        result = "no user";
    else {
        string password = res->getString("password");
        if (password == pwd)
            result = "success";
        else
            result = "password error";
    }
    pool->returnConnect(con);
    return result;
}

bool InsertBarrage(string fromID, string toID, string message, string time) {
    shared_ptr<Connection> con;
    Statement *state;
    ResultSet *res;
    string result;

    con = pool->getConnect();
    while(!con) {
        sleep(1);
    }
    state = con->createStatement();
    state->execute("use danmu");


}

int main() {
//    shared_ptr<Connection> con;
//    Statement *state;
//    ResultSet *res;
//
//    con = pool->getConnect();
//    state = con->createStatement();
//    state->execute("use danmu");
//
//    res = state->executeQuery("select * from user where uname = 'xy'");
//    while(res->next()) {
//        string name = res->getString("uname");
//        string pwd = res->getString("password");
//    }

//    sleep(20);
//    pool->returnConnect(con);
//    std::cout<<pool->getPoolSize()<<endl;
//    sleep(20);
    time_t t=time(0);
    char tmp[64];
    strftime(tmp,sizeof(tmp),"%Y/%m/%d %X",localtime(&t));
    puts(tmp);


    return 0;
}