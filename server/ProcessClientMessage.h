/*************************************************************************
	> File Name: ProcessClientMessage.h
	> Author: 
	> Mail: 
	> Created Time: 2018年07月11日 星期三 13时15分28秒
 ************************************************************************/
#include<sys/socket.h>
#include<sys/select.h>
#include<event.h>
#ifndef _PROCESSCLIENTMESSAGE_H
#define _PROCESSCLIENTMESSAGE_H
#define redisIPAddress "127.0.0.1"
#define redisPortServer 6379
class ProcessClientMessage{
    private:
        struct event_base *Base;
        struct event *ReadEvent;
        struct event *WriteEvent;
        int SockPoint;
    public:
        ProcessClientMessage(int Sock);
        ~ProcessClientMessage();
        void ProcessMessageBefore();
    public:
        static void ReadMessage(int,short,void *Arg);
        void OnWrite(int Sock,short,void*);
};
#endif
