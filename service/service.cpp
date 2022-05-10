//
// Created by hezhnegbo on 2022/5/10.
//


#include "service.h"

int Service::create_service_socket(const string *ip, const string port, const int type, const int que_len) {
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
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(soc, (const sockaddr *) &addr, sizeof(addr)) == -1) {
        cout << "bind addr fail" << endl;
        perror("bind");
        close(soc);
        return -1;
    }

    if (listen(soc, que_len) == -1) {
        cout << "set listen fail" << endl;
        perror("listen");
        close(soc);
        return -1;
    }

    return soc;
}

svc_client Service::get_client_socket(const int l_socket) {
    struct svc_client client;
    struct sockaddr_in addr;
    client.addr = addr;
    int len = sizeof(client);
    if ((client.client_socket = accept(l_socket, (struct sockaddr *) &client.addr, (socklen_t *) &len)) == -1) {
        cout << "accept fial" << endl;
        perror("accept");
        client.flag = -1;
        return client;
    }
    return client;
}

msg_package Service::get_msg(const int sock,int msg_size) {
    msg_package massage;
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

int Service::send_msg(const int clien_sock, string massage) {
    if (send(clien_sock,massage.c_str(),sizeof(massage),0)  == -1){
        perror("send");
        return -1;
    }
    return 0;
}



