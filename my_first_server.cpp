#include "my_first_server.h"
#include <iostream>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <QSqlDatabase>
#include <stdlib.h>
#include <unistd.h>

void *prosessconnect(void *socket_work_addr);

my_first_server::my_first_server()
{

}

void my_first_server::start()
{
    std::cout<<"服务器已启动！正在准备..."<<std::endl;
    addr_info = new sockaddr_in6;
    *addr_info = {};
    socket_server = socket(AF_INET6,SOCK_STREAM,0);
    addr_info->sin6_family = AF_INET6;
//    inet_pton(AF_INET6,IN6ADDR_LOOPBACK_INIT,&addr_info->sin6_addr);
    addr_info->sin6_addr = IN6ADDR_LOOPBACK_INIT;
    addr_info->sin6_port = htons(8888);
    std::cout<<"服务器准备完成！套接字绑定中..."<<std::endl;
    if(bind(socket_server,(sockaddr *)addr_info,sizeof(sockaddr_in6))<0)
    {
        this->close();
        std::cout<<"套接字绑定失败！已经退出..."<<std::endl;
        return;
    }
    std::cout<<"套接字绑定成功!开始监听..."<<std::endl;
    if(listen(socket_server,10)<0)
    {
        this->close();
        std::cout<<"监听错误！已经退出..."<<std::endl;
        return;
    }
    begintoconnect();
}

void my_first_server::close()
{

}

void my_first_server::begintoconnect()
{
    int i = 5;
    while(i--)
    {
        std::cout<<"正在监听..."<<std::endl;
        sockaddr_in6 *socket_remote = new sockaddr_in6;
        *socket_remote = {};
        int size_sock = sizeof(sockaddr_in6);
        int socket_work;
        if((socket_work = accept(socket_server,(sockaddr *)socket_remote,(socklen_t *)&size_sock))<0)
        {
            delete socket_remote;
            this->close();
            std::cout<<" 连接错误！已经退出..."<<std::endl;
            return;
        }
        ++i;
        pthread_t thread[5];
        char socket_work_addr[sizeof(int) + sizeof(sockaddr_in6)];
        *(int *)socket_work_addr = socket_work;
        *(sockaddr_in6 *)((int *)socket_work_addr+1) = *socket_remote;
        delete socket_remote;
        pthread_create(&thread[i-1],NULL,prosessconnect,(void *)socket_work_addr);
        pthread_detach(thread[i-1]);
    }
}

void *prosessconnect(void *socket_work_addr)
{
    char ch[128];
    int socket = *(int *)socket_work_addr;
    inet_ntop(AF_INET6,&((sockaddr_in6 *)((int *)socket_work_addr+1))->sin6_addr,ch,128);
    int i = 0;
    while(true)
    {
        char buf[4096] = {};
        if(recv(socket,buf,4096,0)>0)
        {
            std::cout<<ch<<"   :"<<buf<<"正在连接..."<<std::endl;
            sleep(1);
        }
        while(recv(socket,buf,4096,0) <= 0)
        {
            i++;
            sleep(1);
            if(i==5)
            {
                std::cout<<ch<<"   :"<<buf<<"连接超时,重试..."<<std::endl;
            }
            if(i==10)
            {
                std::cout<<ch<<"   :"<<buf<<"客户端超时..."<<std::endl;
                pthread_exit(NULL);
            }
        }
    }
}

my_first_server::~my_first_server()
{
    ::close(socket_server);

    delete addr_info;
    delete addr_infoforsystem;

    addr_info = NULL;
    addr_infoforsystem = NULL;
}
