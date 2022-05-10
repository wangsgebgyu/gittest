#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;


int main(int len, char *args[]) {
    cout << args[0] << endl;
    cout << args[1] << endl;
    cout << args[2] << endl;

    int soc = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(args[2]));
    addr.sin_addr.s_addr = inet_addr(args[1]);




    if (-1 == connect(soc, (const sockaddr *) &addr, sizeof(addr))) {
        cout << "Connet fail!!!!!" << endl;
        perror("connect");
        return -1;
    }
    char *buf[21];
    while (1) {
        if (-1 == recv(soc, buf, sizeof(buf), 0)) {
            cout << "Receve fail!!!!!" << endl;
            return -1;
        }
        cout << *buf << endl;
    }


    return 0;
}
