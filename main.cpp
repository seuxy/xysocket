#include <iostream>
#include "sonWebSocketServer.h"
#include "Util.h"

int main() {
    cout << "http://10.211.55.7/index.html" <<endl;
    sonWebSocketServer son(9000);
    son.run();
//    vector<string> res = Util::split("type#dwj#123456#",'#');

    return 0;
}