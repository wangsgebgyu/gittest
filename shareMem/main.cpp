#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

using namespace std;


class person{
public:
    person(int age,int flag):m_age(age),m_flag(flag){}
public:
    int m_age;
    int m_flag ;
};

int main() {
    //ipcs -m
    //ipcrm -m shm_id
    int shm_id = shmget(0x655,sizeof(person)*2,0666|IPC_CREAT);
    person *p = 0;
    p = (person *) shmat(shm_id,NULL,0);

    person pp(11,1);
    p[0].m_age = pp.m_age;
    p[0].m_flag = pp.m_flag;

    cout << p[0].m_age << endl;
    cout << p[0].m_flag << endl;

    sleep(300);
    shmdt(p);
    shmctl(shm_id,IPC_RMID,NULL);
    return 0;

}
