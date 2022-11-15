#ifndef MY_FIRST_SERVER_H
#define MY_FIRST_SERVER_H
#include <sys/socket.h>

class my_first_server
{
public:
    my_first_server();
    ~my_first_server();
    void start();
    void close();


private:
    int socket_server;
    struct sockaddr_in6 *addr_info;
    struct sockaddr *addr_infoforsystem;

    void begintoconnect();
//    void prosessconnect(int socket_work,sockaddr_in6 *socket_remote);
};

#endif // MY_FIRST_SERVER_H
