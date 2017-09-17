//
// Created by dingwenjiang on 9/14/17.
//

#include <ctime>
#include "WebSocketServer.h"
#include "Util.h"
#include "libwebsockets.h"

#define MAX_BUFFER_SIZE 0
using namespace xy;
WebSocketServer *self;

static int callback_main(   struct lws *wsi,
                            enum lws_callback_reasons reason,
                            void *user,
                            void *in,
                            size_t len )
{
    int fd;
    unsigned char buf[LWS_SEND_BUFFER_PRE_PADDING + 512 + LWS_SEND_BUFFER_POST_PADDING];
    unsigned char *p = &buf[LWS_SEND_BUFFER_PRE_PADDING];

    switch( reason ) {
        case LWS_CALLBACK_ESTABLISHED:
            self->onConnectWrapper( lws_get_socket_fd( wsi ) );
            lws_callback_on_writable( wsi );
            break;

        case LWS_CALLBACK_SERVER_WRITEABLE:
            fd = lws_get_socket_fd( wsi );
            while( !self->connections[fd]->buffer.empty( ) )
            {
                string message = (self->connections[fd]->buffer.front( ));
                unsigned long msgLen = message.length();
                int charsSent = lws_write( wsi, (unsigned char*)message.c_str(), msgLen, LWS_WRITE_TEXT );
                if( charsSent < msgLen )
                    self->onErrorWrapper( fd, string( "Error writing to socket" ) );
                else
                    // Only pop the message if it was sent successfully.
                    self->connections[fd]->buffer.pop_front( );
            }
            lws_callback_on_writable( wsi );
            break;

        case LWS_CALLBACK_RECEIVE:
            self->onMessage( lws_get_socket_fd( wsi ), string( (const char *)in ) );
            break;

        case LWS_CALLBACK_CLOSED:
            self->onDisconnectWrapper( lws_get_socket_fd( wsi ) );
            break;

        default:
            break;
    }
    return 0;
}


static struct lws_protocols protocols[] = {
        {
                "/",
                callback_main,
                0, // user data struct not used
                MAX_BUFFER_SIZE,
        },{ NULL, NULL, 0, 0 } // terminator
};

WebSocketServer::WebSocketServer(int port, const string &certPath, const string &keyPath) {
    this->_port     = port;
    this->_certPath = certPath;
    this->_keyPath  = keyPath;

    lws_set_log_level( 0, lwsl_emit_syslog ); // We'll do our own logging, thank you.
    struct lws_context_creation_info info;
    memset( &info, 0, sizeof info );
    info.port = this->_port;
    //info.iface = NULL;
    info.protocols = protocols;
//#ifndef LWS_NO_EXTENSIONS
//    info.extensions = lws_get_internal_extensions( );
//#endif

    if( !this->_certPath.empty( ) && !this->_keyPath.empty( ) )
    {
        Util::log( "Using SSL certPath=" + this->_certPath + ". keyPath=" + this->_keyPath + "." );
        info.ssl_cert_filepath        = this->_certPath.c_str( );
        info.ssl_private_key_filepath = this->_keyPath.c_str( );
    }
    else
    {
        Util::log( "Not using SSL" );
        info.ssl_cert_filepath        = NULL;
        info.ssl_private_key_filepath = NULL;
    }
    info.gid = -1;
    info.uid = -1;
    info.options = 0;

    // keep alive
    info.ka_time = 60; // 60 seconds until connection is suspicious
    info.ka_probes = 10; // 10 probes after ^ time
    info.ka_interval = 10; // 10s interval for sending probes
    this->_context = lws_create_context( &info );
    if( !this->_context )
        throw "libwebsocket init failed";
    Util::log( "Server started on port " + Util::toString( this->_port ) );

    // Some of the libwebsocket stuff is define statically outside the class. This
    // allows us to call instance variables from the outside.  Unfortunately this
    // means some attributes must be public that otherwise would be private.
    self = this;
}

WebSocketServer::~WebSocketServer() {
    for (auto it=connections.begin();it!=connections.end();it++) {
        _removeConnection(it->first);
    }
}

void WebSocketServer::run(uint64_t timeout) {
    while(1) {
        wait(timeout);
    }
}

void WebSocketServer::wait(uint64_t timeout) {
    if( lws_service( this->_context, timeout ) < 0 )
        throw "Error polling for socket activity.";
}

void WebSocketServer::send(int socketID, const string &data) {
    connections[socketID]->buffer.push_back(data);
}

void WebSocketServer::broadcast(const string &data) {
    for(auto it=connections.begin();it!=connections.end();it++)
        send(it->first,data);
}

void WebSocketServer::onConnectWrapper(int socketID) {
    Connection* c = new Connection;
    c->createTime = time(0);
    connections[socketID] = c;
    onConnect(socketID);
}

void WebSocketServer::onDisconnectWrapper(int socketID) {
    onDisconnect(socketID);
    _removeConnection(socketID);
}

void WebSocketServer::onErrorWrapper(int socketID, const string &message) {
    Util::log( "Error: " + message + " on socketID '" + Util::toString( socketID ) + "'" );
    this->onError( socketID, message );
    this->_removeConnection( socketID );
}

void WebSocketServer::_removeConnection( int socketID )
{
    Connection* c = this->connections[ socketID ];
    this->connections.erase( socketID );
    delete c;
}

string WebSocketServer::getValue(int socketID, const string &name) {
    return connections[socketID]->keyValueMap[name];
}

void WebSocketServer::setValue( int socketID, const string& name, const string& value ) {
    connections[socketID]->keyValueMap[name] = value;
}





