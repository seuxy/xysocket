//
// Created by dingwenjiang on 9/14/17.
//

#include "sonWebSocketServer.h"
#include "Util.h"

sonWebSocketServer::sonWebSocketServer(int port) : WebSocketServer(port) {}

sonWebSocketServer::~sonWebSocketServer() {}

void sonWebSocketServer::onConnect(int socketID) {
    Util::log("Connect Success!");
}

void sonWebSocketServer::onMessage(int socketID, const string &data) {
    // split #
    // type#userId#roomId#message
    // MSG#1#2#test
    // LOGIN#username#password
    Util::log("Receive Success!");
    // send(socketID,data);
    // broadcast(data);
    vector<string> mes = Util::split(data,'#');
    /*
    int type = Util::converMsg(mes[0]);

    switch (type) {
        case Util::MSGTYPE::LOGIN : Util::log("login");
            break;

    }
     */
    if (mes[0] == "LOGIN") {

    }
    else if (mes[0] == "MSG") {

    }
    else {

    }


}

void sonWebSocketServer::onDisconnect(int socketID) {
    Util::log("Disconnected");
}

void sonWebSocketServer::onError(int socketID, const string &message) {
    Util::log(" Error " );
}

void sonWebSocketServer::joinRoom(int socketID, int roomId) {
    map<int,list>::iterator it = _room.find(roomId);
    
    if (it == _room.end()) {
        list<int> l;
        l.push_back(socketID);
        _room.insert(make_pair(roomId,l));
    }
    else {
        if ()
        it->second.push_back(socketID);
    }



}


