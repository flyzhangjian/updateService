#include<string>
#include<hiredis/hiredis.h>
using namespace std;
class RedisOperate{
    public:
        void setParameter(string appNamePara,string versionPara);
        void serRedisAddress(string redisIPPara,int portPara);
        int connectToRedis();
        string execRedisCommand(string command);
        RedisOperate(string appNamePara,string versionPara);
        RedisOperate();
        ~RedisOperate();
    private:
        string *appName;
        string *version;
        string *redisIP;
        int redisPort;
        redisContext *pRedisContext;
        redisReply *pRedisReply;
};
