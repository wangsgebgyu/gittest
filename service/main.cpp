#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include "service.h"

using namespace std;

int main(int len, char *args[]) {

    int l_socket;

    l_socket = Service::create_service_socket(0, args[1], TCP, 10);

    svc_client clien = Service::get_client_socket(l_socket);

    msg_package massage = Service::get_msg(clien.client_socket, 20);

    cout << massage.msg << endl;
    cout << massage.flag << endl;

    int fl = Service::send_msg(clien.client_socket, "22222222");

    cout << fl << endl;


    int a;
    cin >> a;
    return 0;
}
