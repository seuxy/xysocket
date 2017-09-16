//
// Created by dingwenjiang on 9/14/17.
//

#ifndef XYSOCKET_WEBSOCKETSERVER_H
#define XYSOCKET_WEBSOCKETSERVER_H

#include <string>
#include <list>
#include <map>

using namespace std;

class WebSocketServer {
public:
    struct Connection {
        list<string> buffer;     // Ordered list of pending messages to flush out when socket is writable
        map<string, string> keyValueMap;
        time_t createTime;
    };

    map<int, Connection *> connections;

    WebSocketServer(int port, const string &certPath = "", const string &keyPath = "");

    ~WebSocketServer();

    void run(uint64_t timeout = 50);

    void wait(uint64_t timeout = 50);

    void send(int socketID, const string &data);

    void broadcast(const string &data);


    virtual void onConnect(int socketID) = 0;

    virtual void onMessage(int socketID, const string &data) = 0;

    virtual void onDisconnect(int socketID) = 0;

    virtual void onError(int socketID, const string &message) = 0;

    void onConnectWrapper(int socketID);

    void onDisconnectWrapper(int socketID);

    void onErrorWrapper(int socketID, const string &message);

private:
    int _port;
    string _keyPath;
    string _certPath;
    struct lws_context *_context;

    void _removeConnection(int socketID);
};


#endif //XYSOCKET_WEBSOCKETSERVER_H
