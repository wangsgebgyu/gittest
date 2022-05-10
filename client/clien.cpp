//
// Created by hezhnegbo on 2022/5/10.
//


#include "clien.h"


int Clien::create_connect(const string ip, const string port, int type) {
    int soc = 0;
    switch (type) {
        case 1: {
            if ((soc = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
                cout << "create socket fail" << endl;
                return -1;
            }
            break;
        }
        case 2: {
            if ((soc = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
                cout << "create socket fail" << endl;
                return -1;
            }
            break;
        }
        default: {
            cout << "type error" << endl;
            return -1;
        }
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(port.c_str()));
    addr.sin_addr.s_addr = inet_addr(ip.c_str());

    if (-1 == connect(soc, (const sockaddr *) &addr, sizeof(addr))) {
        cout << "Connet fail!!!!!" << endl;
        perror("connect");
        return -1;
    }

    return soc;
}

massage Clien::get_msg(const int sock,int msg_size) {
    massage massage;
    char arr[msg_size+1];
    int len = -1;
    if ((len = recv(sock,arr,msg_size,0)) == -1){
        perror("recv");
        massage.flag = -1;
        massage.msg = "";
        return massage;
    }
    massage.msg = string(arr).substr(0,len);
    massage.flag = 1;
    return massage;
}

int Clien::send_msg(const int clien_sock, string massage) {
    if (send(clien_sock,massage.c_str(),sizeof(massage),0)  == -1){
        perror("send");
        return -1;
    }
    return 0;
}
