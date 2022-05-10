//
// Created by hezhnegbo on 2022/5/10.
//

#ifndef UNTITLED2_SERVICE_H
#define UNTITLED2_SERVICE_H

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>

using namespace std;

#define TCP 1
#define UDP 2

struct svc_client{
    int client_socket;
    struct sockaddr_in addr;
    int flag;
};

struct msg_package{
    int flag;
    string msg;
};

class Service {
public:

    //return a active listen socket
    static int create_service_socket(const string ip[], const string port, const int type, const int que_len);
    //get a client socket
    static svc_client get_client_socket(const int l_socket);
    //get a massage from client socket
    static msg_package get_msg(const int sock,int mag_size);
    //send a massage to client
    static int send_msg(const int sock,string massage);

private:

    string Ip;
    string Port;
};

#endif //UNTITLED2_SERVICE_H





