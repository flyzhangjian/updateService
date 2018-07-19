/*************************************************************************
	> File Name: my_con_lib.h
	> Author: 
	> Mail: 
	> Created Time: 2018年07月10日 星期二 13时39分36秒
 ************************************************************************/
#include<vector>
#include "chainOfUpdate.h"
#ifndef _MY_CON_LIB_H
#define _MY_CON_LIB_H
class my_con_lib
{
    private:
        char ip[60];
        int port;
        int SocketPoint;
        struct ParameterOfNewThread
        {
            my_con_lib *NewThis;
            int SocketPoint;
        };
        static int processOfUpdateApp;      
        updateChain oneOfUpdate;
        struct event_base * Base;
    public:
        std::vector<updateChain> chainOfUpdateApp;
        ~my_con_lib();
        my_con_lib();
        my_con_lib(char *ip_1,int port_1);
        void set_parameter(char *ip_1,int port_1);
        int connect_to_server();
        pthread_t InitNewThread();
        void dealUpdate();
        static void* InitReadEvent(void *Arg);
        static void OnRead(int Sock,short Event,void *Arg);
};
#endif
