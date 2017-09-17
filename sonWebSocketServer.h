//
// Created by dingwenjiang on 9/14/17.
//

#ifndef XYSOCKET_SONWEBSOCKETSERVER_H
#define XYSOCKET_SONWEBSOCKETSERVER_H

#include "WebSocketServer.h"
#include "mysql_connection.h"
#include <map>
#include <list>
#include <algorithm>
#include <string>
using namespace std;

class sonWebSocketServer : public WebSocketServer{
public:
    sonWebSocketServer(int port);
    ~sonWebSocketServer();

    void onConnect(int socketID);

    void onMessage(int socketID, const string &data) ;

    void onDisconnect(int socketID) ;

    void onError(int socketID, const string &message) ;

private:
    void joinRoom(int socketID, int roomId);
    void leaveRoom(int socketID, int roomId);
    void broadcastMsgByRoomId(int roomId,const string& msg);
    map<int, list<int>> _room;

    string isUser(const string &name, const string &pwd, int &userId);

    connectionPool *pool = connectionPool::getInstance();

};


#endif //XYSOCKET_SONWEBSOCKETSERVER_H
