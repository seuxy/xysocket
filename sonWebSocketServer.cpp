//
// Created by dingwenjiang on 9/14/17.
//

#include "sonWebSocketServer.h"
#include "Util.h"
//#include "mysql_connection.h"


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
        //LOGIN#username#password
        string username = mes[1];
        string password = mes[2];
        int userId = -1;
        string res = isUser(username, password, userId);

        send(socketID, "LOGIN#"+res+"#"+to_string(userId));
        if (res == "success") {
            setValue(socketID, "status", "valid");
            setValue(socketID, "room", "-1");

        }
        else {
            setValue(socketID, "status", "invalid");
        }
    }
    else if (mes[0] == "JOIN") {
        //JOIN#roomId
        int roomId = atoi(mes[1].c_str());
        if (getValue(socketID,"status") == "valid") {
            string curroom = getValue(socketID, "room");
            if(curroom == "-1")
                joinRoom(socketID, roomId);
            else if (roomId != atoi(curroom.c_str())) {
                leaveRoom(socketID, atoi(curroom.c_str()));
                joinRoom(socketID, roomId);
            }

            send(socketID, "JOIN#join success");
        }
        else {
            send(socketID, "JOIN#invalid user");
        }
    }
    else if (mes[0] == "MSG"){
        //MSG#roomId#userId#content
        int roomId = atoi(mes[1].c_str());
        int userId = atoi(mes[2].c_str());
        string content = "MSG#"+mes[2]+"#"+mes[3];
        //TODO danmu
        broadcastMsgByRoomId(roomId, content);
    }


}

void sonWebSocketServer::onDisconnect(int socketID) {
    Util::log("Disconnected");
}

void sonWebSocketServer::onError(int socketID, const string &message) {
    Util::log(" Error " );
}

void sonWebSocketServer::joinRoom(int socketID, int roomId) {
    map<int,list<int>>::iterator it = _room.find(roomId);
    
    if (it == _room.end()) {
        list<int> l;
        l.push_back(socketID);
        _room.insert(make_pair(roomId,l));
        setValue(socketID, "room", to_string(roomId));
    }
    else {
        list<int> l = it->second;
        auto iter = find(l.begin(),l.end(),socketID);
        if (iter == l.end())  {
            it->second.push_back(socketID);
            setValue(socketID, "room", to_string(roomId));
        }
    }

}

void sonWebSocketServer::leaveRoom(int socketID, int roomId) {
    map<int,list<int>>::iterator it = _room.find(roomId);

    if (it != _room.end()) {
        list<int>& l = it->second;
        auto iter = find(l.begin(),l.end(),socketID);
        if (iter != l.end()) {
            l.erase(iter);
            setValue(socketID, "room", "-1");
        }
    }
}

void sonWebSocketServer::broadcastMsgByRoomId(int roomId, const string &msg) {
    map<int,list<int>>::iterator it = _room.find(roomId);

    if (it != _room.end()) {
        list<int> l = it->second;

        for(auto iter = l.begin();iter!=l.end();iter++) {
            send(*iter, msg);
        }
    }
}

string sonWebSocketServer::isUser(const string &name, const string &pwd, int &userId) {
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

    res = state->executeQuery(" select * from user where username = '"+name+"' " );
    if (!res->next())
        result = "no user";
    else {
        string password = res->getString("password");
        userId = res->getInt("userid");
        if (password == pwd)
            result = "success";
        else
            result = "password error";
    }
    pool->returnConnect(con);
    return result;
}


