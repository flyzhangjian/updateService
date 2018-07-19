/*************************************************************************
	> File Name: client_main.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年07月10日 星期二 12时23分10秒
 ************************************************************************/

#include<string.h>
#include<iostream>
#include<unistd.h> 
#include<pthread.h>
#include<string>
#include<stdlib.h>
#include"my_con_lib.h"

using namespace std;

int main(int argc,char **argv)
{
    cout << "test start\n";
    my_con_lib *con = new my_con_lib();
    char *server_ip;
    int port;
    if(argc == 1)
    {
        server_ip = (char *)malloc(60);
        cout << "please input the server IP and PORT"<<endl;
        cin >> server_ip >> port;
    }
    else
    {
        for(int i = 1;i < argc;i += 2)
        {
            switch(argv[i][1])
            {
                case 'p':
                {
                    if(i+1 >= argc)
                    {
                        cout << "please input the port of the server"<<endl;
                        return 0;
                    }
                    else
                    {
                        port = atoi(argv[i+1]);
                    }
                    break;
                }
                case 's':
                {
                    if(i+1 >= argc)
                    {
                        cout << "error please check the cmmand"<<endl;
                        return 0;
                    }
                    else
                    {
                        server_ip = argv[i + 1];
                    }
                    break;
                }
                default:
                {
                    cout << "未知参数"<<endl;
                    return 0;
                }
            }
        }
    }
    (*con).set_parameter(server_ip,port);
    int SockPointOfClient = (*con).connect_to_server();
    if(SockPointOfClient < 0)
    {
        cout << "连接失败" << endl;
        exit(0);
    }
    pthread_t newThreadNumber = (*con).InitNewThread();
    while(true)
    {
        char Buffer[60] = "hello world 123";
        cout << "请输入:"<<endl; 
        cin >> Buffer;
        int Number = write(SockPointOfClient,Buffer,strlen(Buffer));
        cout << "bytes to write" << Number << endl;
    }
    for(std::vector<updateChain>::iterator it = (con -> chainOfUpdateApp).begin();it != (con -> chainOfUpdateApp).end();it++)
    {
        cout << it -> name << it -> version << it -> downloadPath << it -> installPath << it -> command << endl;
    }
    return 0;
}
