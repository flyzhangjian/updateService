/*************************************************************************
	> File Name: ServerService.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年07月11日 星期三 10时05分31秒
 ************************************************************************/

#include<iostream>
#include<string.h>
#include<sys/select.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<pthread.h>
#include"ServerService.h"
#include"ProcessClientMessage.h"
using namespace std;

ServerService::ServerService()
{
    ;
}

ServerService::ServerService(char *IPArgu,int PortArgu)
{
    strcpy(this -> IP,IPArgu);
    this -> PortListen = PortArgu;
}

ServerService::~ServerService()
{
    if(BaseEvent)
    {
        free(BaseEvent);
    }
    if(ListenEvent)
    {
        delete ListenEvent;
    }
}

int ServerService::GetSocket()
{
   int FSocket = socket(AF_INET,SOCK_STREAM,0);
   this -> SocketPoint = FSocket;
   return FSocket;
}

void ServerService::SetQueueNumber(int Number)
{
    this -> QueueNumber = Number;
}
int ServerService::BindStart()
{
    GetSocket();
    if(this -> SocketPoint < 0)
    {
        cout << "haihdia"<<endl;
        return -1;
    }
    memset(&ServerAddr,0,sizeof(struct sockaddr_in));
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_addr.s_addr = inet_addr(this -> IP);
    ServerAddr.sin_port = htons(this -> PortListen);
    int BindResult = bind(this -> SocketPoint,(struct sockaddr *)(&(this -> ServerAddr)),sizeof(struct sockaddr));
    if(BindResult < 0)
    {
        return -1;
    }
    listen(this->SocketPoint,this -> QueueNumber);
    BaseEvent = event_base_new();
    ListenEvent = new event();
    event_set(ListenEvent, SocketPoint, EV_READ|EV_PERSIST, OnAccept, NULL);
    event_base_set(BaseEvent,ListenEvent);
    event_add(ListenEvent,NULL);
    event_base_dispatch(BaseEvent);
    return 0;
}

void ServerService::OnAccept(int PreSock,short,void*)
{
    cout << "accepttd begin" << endl;
    struct sockaddr_in RemoteAddr;
    int Size = sizeof(struct sockaddr_in);
    int NewSock = accept(PreSock,(struct sockaddr *)(&RemoteAddr),(socklen_t*)(&Size));
    if(NewSock<0)
    {
        cout << "accept error"<<endl;
    }
    AcceptNewThread(NewSock);
}

void ServerService::AcceptNewThread(int NewSockPoint)
{
    pthread_t NewThread;
    pthread_create(&NewThread,NULL,ProsessClient,(void *)(&NewSockPoint));
    pthread_detach(NewThread);
}

void *ServerService::ProsessClient(void *SockOfClient)
{
    int NewSockToClient = *(int *)SockOfClient;
    if(NewSockToClient < 0)
    {
        cout << "error when process internal "<<endl;
    }
    ProcessClientMessage * ProcessMessage = new ProcessClientMessage(NewSockToClient);
    ProcessMessage -> ProcessMessageBefore();
    return 0;
}
