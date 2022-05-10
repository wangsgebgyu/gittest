#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

int main(int len, char *args[]) {

    int l_socket;

    if ((l_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        cout << "!!!!!!Create socket fail!!!!!!" << endl;
        return -1;
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(args[1]));
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int on = 1;

    setsockopt(l_socket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    if (bind(l_socket, reinterpret_cast<const sockaddr *>(&addr), sizeof(addr)) != 0) {
        cout << "!!!!!!Addr bind fail!!!!!!" << endl;
        return -1;
    }
    if (listen(l_socket, 10) != 0) {
        cout << "!!!!!!Listen fail!!!!!!" << endl;
        return -1;
    }

    int s_socket = 0;
    struct sockaddr_in s_addr;
    int addr_len = sizeof(struct sockaddr_in);

    s_socket = accept(l_socket, (struct sockaddr *) &s_addr, (socklen_t *) &addr_len);
    if (s_socket == -1) {
        cout << "!!!!!!Accept socket fail!!!!!!" << endl;
        return -1;
    }
    int lenth = 0;

    cout << s_addr.sin_addr.s_addr << endl;
    cout << s_addr.sin_port << endl;

    while (1) {
        char *buff[20];
        lenth = recv(s_socket, buff, sizeof(buff), 0);
        if (lenth == -1) {
            cout << "!!!!!!Receve fail!!!!!!" << endl;
            return -1;
        }

        cout << *buff << endl;

        string in;
        cout << "put:";
        cin >> in;

    }
    return 0;
}
