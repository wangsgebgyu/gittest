#include "clien.h"


int main(int len, char *args[]) {

    int soc = Clien::create_connect(args[1], args[2], TCP);

    Clien::send_msg(soc, "11111111111");
    massage msg = Clien::get_msg(soc, 20);
    cout << msg.msg << endl;
    cout << msg.flag << endl;

    int a;
    cin >> a;

    return 0;
}
