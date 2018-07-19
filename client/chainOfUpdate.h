#include<string>

#ifndef _CHAINOFUPDATE_H
#define _CHAINOFUPDATE_H

typedef struct chainOfUpdate{
    std::string name;
    std::string version;
    std::string downloadPath;
    std::string installPath;
    std::string command;
} updateChain;

#endif