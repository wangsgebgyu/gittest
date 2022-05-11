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

msg_package Service::get_msg(const int sock, int msg_size) {
    msg_package massage;
    char arr[msg_size + 1];
    int len = -1;
    if ((len = recv(sock, arr, msg_size, 0)) == -1) {
        perror("recv");
        massage.flag = -1;
        massage.msg = "";
        return massage;
    }
    massage.msg = string(arr).substr(0, len);
    massage.flag = 1;
    return massage;
}

int Service::send_msg(const int clien_sock, string massage) {
    if (send(clien_sock, massage.c_str(), sizeof(massage), 0) == -1) {
        perror("send");
        return -1;
    }
    return 0;
}

int Service::close_sock(int sock) {
    close(sock);
    return 1;
}

select_pack Select::Init_select(const int svc_fd) {
    select_pack pack;
    pack.l_sock = svc_fd;
    pack.max_fd = svc_fd;
    fd_set read_set;
    FD_ZERO(&read_set);
    FD_SET(svc_fd, &read_set);
    pack.f_set = read_set;
    return pack;
}

void Select::Apply_select(select_pack &pack, const timeval &tv, int (*send_msg)(const int sock, string massage),
                          msg_package (*get_msg)(const int sock, int mag_size)) {
    select_pack temp_set = pack;
    while (1) {
        int count_fd = select(temp_set.max_fd + 1, &temp_set.f_set, NULL, NULL, NULL);
        if (count_fd == 0) continue; //time out
        //cout << "ready read sock number:" << count_fd << endl;
        for (int i = 0; i <= temp_set.max_fd; ++i) {
            if (FD_ISSET(i, &temp_set.f_set) <= 0) continue;
            if (i == temp_set.l_sock) {
                svc_client clien = Service::get_client_socket(temp_set.l_sock);
                FD_SET(clien.client_socket, &pack.f_set);
                if (clien.client_socket > pack.max_fd) pack.max_fd = clien.client_socket;
                cout << "add a new clien sock to fd_set:" << clien.client_socket << endl;
                continue;
            } else {
                //If mag_size is lower to a small num, maybe the same as sock will be actived twice.
                msg_package msg = get_msg(i, 512);
                if (msg.msg == "") {
                    close(i);
                    FD_CLR(i, &pack.f_set);//remove a socket from fd_set
                    if (temp_set.max_fd == i) {
                        for (int j = pack.max_fd; j >= 0; j--) {
                            if (FD_ISSET(j, &temp_set.f_set)) {
                                pack.max_fd = j;
                                break;
                            }
                        }
                    }
                    cout << "close a sock:" << i << endl;
                    continue;
                }
                cout << "From sockeet: " << i;
                cout << " Massage: " << msg.msg << endl;
                send_msg(i, "I receve your massage!");
                continue;
            }
        }
        temp_set = pack;
    }
}

poll_pack Poll::Init_poll(const int l_socket, const int fd_poll_size) {
    poll_pack pollPack;
    pollPack.l_socket = l_socket;
    pollPack.fd_poll_size = fd_poll_size;
    pollPack.len_fd = 1;
    pollPack.fds[fd_poll_size];

    for (int i = 0; i < fd_poll_size; ++i) {
        pollPack.fds[i].fd = -1;
        pollPack.fds[i].revents = -1;
    }

    pollPack.fds[0].fd = l_socket;
    pollPack.fds[0].events = POLLIN;

    return pollPack;
}

void Poll::Aplly_poll(poll_pack &pp, const timeval &tv, int (*send_msg)(const int, string),
                      msg_package (*get_msg)(const int, int)) {
    while (1) {
        int infds = poll(pp.fds, pp.len_fd, tv.tv_sec * 10000);//secondmillion
        if (infds < 0) {
            cout << "poll() failed." << endl;
            perror("poll():");
            break;
        }
        // 超时。
        if (infds == 0) {
            cout << "poll() timeout.." << endl;
            continue;
        }

        int len_fd = pp.len_fd;
        for (int i = 0; i < len_fd; ++i) {
            if ((pp.fds[i].revents & POLLIN) == 0) continue;
            if (pp.fds[i].fd == pp.l_socket) {
                pp.fds[i].revents = 0;
                svc_client clien = Service::get_client_socket(pp.l_socket);
                pp.fds[pp.len_fd].fd = clien.client_socket;
                pp.fds[pp.len_fd].events = POLLIN;
                pp.len_fd += 1;
                cout << "get a new clien socket" << endl;
                continue;
            } else {
                pp.fds[i].revents = 0;
                msg_package msg = get_msg(pp.fds[i].fd, 512);

                if (msg.msg == "") {
                    close(pp.fds[i].fd);
                    pp.fds[i].fd = -1;
                    pp.fds[i].events = 0;
                    pp.fds[i] = pp.fds[pp.len_fd - 1];
                    pp.len_fd -= 1;
                    cout << "close a socket" << endl;
                    continue;
                }
                cout << "form socket : " << pp.fds[i].fd;
                cout << " massage : " << msg.msg << endl;
                send_msg(pp.fds[i].fd, "We receve your massage!");
            }
        }
    }
}

void
Epoll::Init_epoll(const int l_socket, const int fd_epoll_size, const int max_evnet, int (*send_msg)(const int, string),
                  msg_package (*get_msg)(const int, int)) {
    int epoll_fd = epoll_create(fd_epoll_size);
    struct epoll_event ev;
    ev.data.fd = l_socket;
    ev.events = EPOLLIN;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, l_socket, &ev);
    while (1) {
        struct epoll_event events[fd_epoll_size];
        int infds = epoll_wait(epoll_fd, events, max_evnet, -1);
        if (infds < 0) {
            cout << "epoll_wait() failed." << endl;
            perror("epoll_wait()");
            break;
        }
        if (infds == 0) {
            cout << "epoll_wait() timeout" << endl;
            continue;
        }

        for (int ii = 0; ii < infds; ii++) {
            if ((events[ii].data.fd == l_socket) && (events[ii].events & EPOLLIN)) {
                svc_client clien = Service::get_client_socket(l_socket);
                cout << "get a new clien socket" << endl;

                ev.data.fd = clien.client_socket;
                ev.events = EPOLLIN;
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, clien.client_socket, &ev);

                continue;
            } else if (events[ii].events & EPOLLIN) {
                msg_package msg = get_msg(events[ii].data.fd, 512);
                if (msg.msg == "") {
                    close(events[ii].data.fd);
                    cout << "close a socket" << endl;
                    ev.data.fd = events[ii].data.fd;
                    ev.events = EPOLLIN;
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[ii].data.fd, &ev);
                    continue;
                }
                cout << "form socket : " << events[ii].data.fd;
                cout << " massage : " << msg.msg << endl;
                send_msg(events[ii].data.fd, "We receve your massage!");
            }
        }
    }

}
