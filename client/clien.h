//
// Created by hezhnegbo on 2022/5/10.
//

#ifndef UNTITLED2_CLIEN_H
#define UNTITLED2_CLIEN_H
#include <string>
#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
using namespace std;

#define TCP 1
#define UDP 2

struct massage{
    string msg;
    int flag;
};

class Clien{
public:
    static int create_connect(const string ip,const string port,int type);

    static int send_msg(const int clien_sock, string massage);

    static massage get_msg(const int sock, int msg_size);

    static int close_sock(int sock);
};

#endif //UNTITLED2_CLIEN_H
