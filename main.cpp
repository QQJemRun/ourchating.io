#include <iostream>
#include "my_first_server.h"

int main()
{
    my_first_server S;
    int i = 0;
    while(true)
    {
        std::cout<<"1.打开服务器"<<std::endl;
        std::cout<<"2.关闭服务器"<<std::endl;
        std::cout<<"3.退出服务器"<<std::endl;
        std::cin>>i;
        switch (i)
        {
            case 1: S.start(); break;
            case 2: S.close(); break;
            default: return 1;
        }
    }
    return 0;
}
