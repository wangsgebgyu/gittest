#include <unistd.h>
#include "clien.h"


int main(int len, char *args[]) {

    int soc = Clien::create_connect(args[1], args[2], TCP);

    Clien::send_msg(soc, "11111");

    massage msg = Clien::get_msg(soc, 512);
    cout << msg.msg << endl;
    cout << msg.flag << endl;

    sleep(60);

    close(soc);

    cout << "close sock" << endl;

    return 0;
}
