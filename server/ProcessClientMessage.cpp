/*************************************************************************
	> File Name: ProcessClientMessage.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年07月11日 星期三 13时15分36秒
 ************************************************************************/

#include<iostream>
#include<pthread.h>
#include<unistd.h>
#include<string.h>
#include<string>
#include"ProcessClientMessage.h"
#include"redisOperate.h"
using namespace std;

ProcessClientMessage::ProcessClientMessage(int Sock)
{
    this -> SockPoint = Sock;
    Base = event_base_new();
    ReadEvent = (struct event*)malloc(sizeof(struct event));
    WriteEvent = (struct event*)malloc(sizeof(struct event));
}

ProcessClientMessage::~ProcessClientMessage()
{
    if(Base)
    {
        free(Base);
        Base = 0;
    }
    if(ReadEvent)
    {
        delete ReadEvent;
        ReadEvent = 0;
    }
    if(WriteEvent)
    {
        delete WriteEvent;
        WriteEvent = 0;
    }
}

void ProcessClientMessage::ProcessMessageBefore()
{
    event_set(this -> ReadEvent,this -> SockPoint,EV_READ|EV_PERSIST,ReadMessage,(void *)this);
    event_base_set(this -> Base,this -> ReadEvent);
    event_add(ReadEvent,NULL);
    event_base_dispatch(Base);
}

void ProcessClientMessage::ReadMessage(int ,short ,void *Arg)
{
    char *Buffer = new char[120];
    ProcessClientMessage *NewThis = (ProcessClientMessage *)Arg;
    int Size = read(NewThis -> SockPoint,Buffer,120);
    if(Size == 0)
    {
        close(NewThis -> SockPoint);
        bool bReuseaddr =  true;
        setsockopt (NewThis -> SockPoint,SOL_SOCKET ,SO_REUSEADDR,(const char*)&bReuseaddr,sizeof(bool));
        pthread_exit(NULL);
        return;
    }
    cout << Buffer << endl;
    NewThis -> OnWrite(NewThis->SockPoint,5,NULL);
    //event_set(NewThis -> WriteEvent, NewThis -> SockPoint , EV_WRITE, OnWrite, NULL);
    //event_base_set(NewThis -> Base, NewThis -> WriteEvent);
    //event_add(NewThis -> WriteEvent, NULL);
}

void ProcessClientMessage::OnWrite(int Sock,short,void *)
{
    cout << "write back" << endl;
    char Buffer[160] = "hello world";
    int write_num = write(Sock,Buffer,strlen(Buffer));
    RedisOperate *redisToMyApp = new RedisOperate();
    char redisIP[50] = redisIPAddress;
    int redisPort = redisPortServer;
    redisToMyApp -> serRedisAddress(redisIP,redisPort);
    int isOK = redisToMyApp -> connectToRedis();
    if(isOK < 0)
    {
        cout << "error for connect redis";
        return;
    }
    string command;
    string commandExec("lindex updateApp ");
    int index = 0;
    while(true)
    {

        command = commandExec + to_string(index); 
        string s = redisToMyApp -> execRedisCommand(command);
        if(s.size() > 0)
        {
            //write(Sock,"begin",20);
            string appName = "begin&&appName=" + s;
            //cout << "yyyyyyy" << write(Sock,appName.c_str(),strlen(appName.c_str())) << "  " << appName <<endl;
            cout << "App " << appName << endl;
            string commandVersionLatest("hget " + s + " versionLatest");
            string versionLatest = redisToMyApp -> execRedisCommand(commandVersionLatest);
            if(versionLatest.size() != 0)
            {
                appName += "&&version=" + versionLatest;
                //write(Sock,versionLatest.c_str(),versionLatest.size());
            }
            else
            {
                appName += "&&version=NULL";
                //write(Sock,"NULL",5);
            }
            cout << "version " + versionLatest << endl;
            string commandPathDownload("hget " + s + " pathDownload");
            string pathDownload = redisToMyApp -> execRedisCommand(commandPathDownload);
            if(pathDownload.size() != 0)
            {
                appName += "&&pathDownload=" + pathDownload;
                //write(Sock,pathDownload.c_str(),pathDownload.size());
            }
            else
            {
                appName += "&&pathDownload=NULL";
                //write(Sock,"NULL",5);
            }
            cout << "pathDownload:" + pathDownload << endl;
            string commandInstallPath("hget " + s + " installPath");
            string installPath = redisToMyApp -> execRedisCommand(commandInstallPath);
            if(installPath.size() != 0)
            {
                appName += "&&installPath=" + installPath;
                //write(Sock,installPath.c_str(),installPath.size());
            }
            else
            {
                appName += "&&installPath=NULL";
               // write(Sock,"NULL",5);
            }
            cout << "installPath:" << installPath << endl; 
            string commandNeed("hget " + s + " commandNeed");
            string commandNeedExec = redisToMyApp -> execRedisCommand(commandNeed);
            if(commandNeedExec.size() != 0)
            {
                appName += "&&commandNeedExec=" + commandNeedExec;
                //write(Sock,commandNeedExec.c_str(),commandNeedExec.size());
            }
            else
            {
                appName += "&&commandNeedExec=NULL";
                //write(Sock,"NULL",5);
            }
            cout << "commandNeed:" + commandNeedExec << endl;
            appName += "&&end$";
            write(Sock,appName.c_str(),appName.size());
            cout << endl;
        }
        else
        {
            cout << "return NULL end";
            break;
        }
        index++;
    }
    close(this -> SockPoint);
    bool bReuseaddr =  true;
    setsockopt (this -> SockPoint,SOL_SOCKET ,SO_REUSEADDR,(const char*)&bReuseaddr,sizeof(bool));
    pthread_exit(NULL);
    delete redisToMyApp;
}
