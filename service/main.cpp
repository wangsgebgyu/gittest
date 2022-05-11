#include <sys/poll.h>
#include "service.h"

using namespace std;
#define MAX 10

int main(int len, char *args[]) {

    int l_socket;
    //create a listen socket:l_socket
    l_socket = Service::create_service_socket(0, args[1], TCP, 2);

    //select_pack pack = Select::Init_select(l_socket);
    //struct timeval tv = {5, 0};
    //Select::Apply_select(pack,tv,Service::send_msg,Service::get_msg);

    //poll_pack pollPack = Poll::Init_poll(l_socket, 10);
    //Poll::Aplly_poll(pollPack,tv,Service::send_msg,Service::get_msg);

    Epoll::Init_epoll(l_socket,10,10,Service::send_msg,Service::get_msg);

    return 0;
}

