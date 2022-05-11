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
#include <sys/poll.h>
#include <sys/epoll.h>

using namespace std;

#define TCP 1
#define UDP 2

struct svc_client {
    int client_socket;
    struct sockaddr_in addr;
    int flag;
};

struct msg_package {
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
    static msg_package get_msg(const int sock, int mag_size);

    //send a massage to client
    static int send_msg(const int sock, string massage);

    static int close_sock(int sock);

};

struct select_pack {
    fd_set f_set;
    int max_fd;
    int l_sock; //listen socket
};

class Select {
public:
    static select_pack Init_select(const int svc_fd);

    static void Apply_select(select_pack &pack, const struct timeval &tv, int (*send_msg)(const int sock, string massage),
                             msg_package (*get_msg)(const int sock, int mag_size));
};

struct poll_pack{
    int fd_poll_size;
    int l_socket;
    int len_fd;
    struct pollfd fds[];
};

class Poll{
public:
    static poll_pack Init_poll(const int l_socket,const int fd_poll_size);

    static void Aplly_poll(poll_pack& pp,const struct timeval &tv, int (*send_msg)(const int sock, string massage),
                           msg_package (*get_msg)(const int sock, int mag_size));
};

class Epoll{
public:
    static void Init_epoll(const int l_socket,const int fd_epoll_size,const int max_evnet,int (*send_msg)(const int, string),
            msg_package (*get_msg)(const int, int));
};

#endif //UNTITLED2_SERVICE_H





