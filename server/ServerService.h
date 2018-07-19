/*************************************************************************
	> File Name: ServerService.h
	> Author: 
	> Mail: 
	> Created Time: 2018年07月11日 星期三 09时57分41秒
 ************************************************************************/
#include<netinet/in.h>
#include<event.h>
#ifndef _SERVERSERVICE_H
#define _SERVERSERVICE_H
class ServerService{
    private:
        char IP[60] = "127.0.0.1";
        int PortListen = 5001;
        struct sockaddr_in ServerAddr;
        int SocketPoint;
        int QueueNumber = 10;
        struct event_base * BaseEvent;
        struct event *ListenEvent;
    public:
        ServerService();
        ServerService(char *IPArgu,int PortArgu);
        ~ServerService();
        void SetQueueNumber(int Number);
        int GetSocket();
        int BindStart();

    public:
        static void OnAccept(int ,short,void *);
        static void AcceptNewThread(int NewSockPoint);
        static void *ProsessClient(void *SockOfClient);
};
#endif
