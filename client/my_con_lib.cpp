#include"my_con_lib.h"
#include<string.h>
#include<sys/socket.h>
#include<stdio.h>
#include<sys/select.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<event.h>
#include<stdlib.h>
#include<iostream>
#include<unistd.h>
#include"chainOfUpdate.h"
#include<pthread.h>
#include<algorithm>

using namespace std;

int my_con_lib::processOfUpdateApp = 0;

my_con_lib::my_con_lib()
{
    strcpy(this -> ip,"127.0.0.1");
    this -> port = 8080;
}

my_con_lib::my_con_lib(char *ip_1,int port_1)
{
    strcpy(this -> ip,ip_1);
    this -> port = port_1;
}

void my_con_lib::set_parameter(char *ip_1,int  port_1)
{
    this -> port = port_1;
    strcpy(this -> ip,ip_1);
}

int my_con_lib::connect_to_server()
{
    int fd = socket(AF_INET,SOCK_STREAM,0);
    if(fd == -1)
    {
        return -1;
    }
    else
    {
        struct sockaddr_in remote_addr;
        memset(&remote_addr,0,sizeof(struct sockaddr_in));
        remote_addr.sin_family=AF_INET;
        remote_addr.sin_port = htons(this -> port);
        remote_addr.sin_addr.s_addr = inet_addr(this -> ip);
        int con_result = connect(fd,(struct sockaddr *)(&remote_addr),sizeof(struct sockaddr));
        if(con_result == -1)
        {
            close(fd);
            return -1;
        }
        else
        {
            this -> SocketPoint = fd;
        }
    }
    return fd;
}

pthread_t my_con_lib::InitNewThread()
{
    struct ParameterOfNewThread *Temp = (ParameterOfNewThread *)malloc(sizeof(struct ParameterOfNewThread));
    Temp -> NewThis = this;
    Temp -> SocketPoint = this -> SocketPoint;
    pthread_t NewThreadID;
    pthread_create(&NewThreadID,NULL,InitReadEvent,(void *)(Temp));
    pthread_detach(NewThreadID);
}

void* my_con_lib::InitReadEvent(void *Arg)
{
    struct ParameterOfNewThread * SourceData = (struct ParameterOfNewThread *)Arg;
    int Sock = SourceData -> SocketPoint;
    SourceData -> NewThis ->  Base = event_base_new();
    struct event* Read = (struct event *)malloc(sizeof(struct event));
    event_set(Read,Sock,EV_READ|EV_PERSIST, OnRead,Arg);
    event_base_set(SourceData -> NewThis -> Base,Read);
    event_add(Read,NULL);
    event_base_dispatch(SourceData -> NewThis -> Base);
    return 0;
}

void my_con_lib::OnRead(int Sock,short Event,void *Arg)
{
    char key[6][20] = {"appName","version","pathDownload","installPath","commandNeedExec","end"};
    struct ParameterOfNewThread * SourceData = (struct ParameterOfNewThread *)Arg;
    my_con_lib *newThis = SourceData -> NewThis;
    int bufferSize = 512;
    char Buffer[512];
    memset(Buffer,0,512);
    char *check = Buffer;
    int size = read(Sock,Buffer,512);
    if(0 == size)
    {
        newThis -> dealUpdate();
        cout << "更新完成\n";
        exit(0);
    }
    while(*(check + size - 1) != '$')
    {
        check += size;
        bufferSize -= size;
        size = read(Sock,check,bufferSize);
    }
    //cout << size << "  " << Buffer << endl;
    string resultApp(Buffer);
    //cout << resultApp << endl;
    unsigned int position;
    for (int i = 0;i < 5;i++)
    {
        position = resultApp.find(key[i]);
        if(position < 0)
        {
            cout << "Error for string pattern";
            break;
        }
        int position1 = resultApp.find(key[i + 1]);
        cout <<  position << "  " << position1 << endl;
        string desSting = resultApp.substr(position + strlen(key[i]) + 1,position1 - position - (strlen(key[i]) + 3));
        if(i == 0)(newThis -> oneOfUpdate).name = desSting;
        else if(1 == i) (newThis -> oneOfUpdate).version = desSting;
        else if(2 == i) (newThis -> oneOfUpdate).downloadPath = desSting;
        else if(3 == i) (newThis -> oneOfUpdate).installPath = desSting;
        else if(4 == i) (newThis -> oneOfUpdate).command = desSting;
        if(4 == i)
        {
            (newThis -> chainOfUpdateApp).push_back(newThis -> oneOfUpdate);
        }
    }
}

void my_con_lib::dealUpdate()
{
    for(std::vector<updateChain>::iterator it = (this -> chainOfUpdateApp).begin();it != (this -> chainOfUpdateApp).end();it++)
    {
        cout << it -> name << it -> version << it -> downloadPath << it -> installPath << it -> command << endl;
        FILE *fp;
        if(it -> command != "NULL")
        {
            if((fp = popen((it -> command).data(),"r")) != NULL)
            {
                char line[50];
                cout << it -> command << endl;
                while (fgets(line, sizeof(line)-1, fp) != NULL)
                {
                    cout << line;
                }
                pclose(fp);
            }
            else
            {
                cout << "执行命令出错" << endl;
            }
        }
        if(it -> downloadPath != "NULL")
        {
            string getAddress("wget " + it -> downloadPath);
            if((fp = popen(getAddress.data(),"r")) != NULL)
            {
                char line[50];
                cout << it -> command << endl;
                while (fgets(line, sizeof(line)-1, fp) != NULL)
                {
                    cout << line;
                }
                pclose(fp);
            }
            else
            {
                cout << "执行下载命令出错" << endl;
            }
        }
        else
        {
            cout << "无下载路径 跳过" << endl;
            break;
        }
    }
}