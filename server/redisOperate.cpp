/*************************************************************************
	> File Name: redisOperate.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年07月16日 星期一 13时05分51秒
 ************************************************************************/

#include<iostream>
#include"redisOperate.h"
using namespace std;

RedisOperate::RedisOperate()
{
    appName = NULL;
    version = NULL;
    pRedisReply = 0;
    pRedisContext = 0;
}

RedisOperate::RedisOperate(string appNamePara,string versionPara)
{
   appName = new string(appNamePara);
   version = new string(versionPara);
}

RedisOperate::~RedisOperate()
{
    if(appName)
    {
        delete appName;
        appName = 0;
    }
    if(version)
    {
        delete version;
        version = 0;
    }
    if(pRedisReply)
    {
        delete pRedisReply;
        version = 0;
    }
    if(pRedisContext)
    {
        delete pRedisContext;
        pRedisContext = 0;
    }
}

void RedisOperate::setParameter(string appNamePara,string versionPara)
{
    appName = new string(appNamePara);
    version = new string(versionPara);   
}

void RedisOperate::serRedisAddress(string redisIPPara,int portPara)
{
    redisIP = new string(redisIPPara);
    redisPort = portPara;
}

int RedisOperate::connectToRedis()
{
    struct timeval timeOut = {2,0};
    pRedisContext = (redisContext*)redisConnectWithTimeout((this -> redisIP) -> c_str(), this -> redisPort,timeOut);
    if(pRedisContext == NULL || pRedisContext -> err)
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

string RedisOperate::execRedisCommand(string command)
{
    cout << command.c_str() << endl;
    pRedisReply = (redisReply *)redisCommand(pRedisContext,command.c_str());
    if(pRedisReply)
    {
        //cout << pRedisReply -> str << endl;
        if(pRedisReply -> str)
        {
            string resultCommand(pRedisReply -> str);
            if(!pRedisReply)
            {
                delete pRedisReply;
                pRedisReply = 0;
            }
            return resultCommand;
        }
        else
        {
            cout << "return code is NULL\n";
            return string("");
        }
    }
    else
    {
        cout << "NULL is it?" << endl;
        return string("");
    }
}
