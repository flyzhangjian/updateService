/*************************************************************************
	> File Name: ServerMain.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年07月11日 星期三 09时37分24秒
 ************************************************************************/

#include<iostream>
#include"ServerService.h"
using namespace std;

int main()
{
    ServerService *SeverLib = new ServerService();
    if(SeverLib -> BindStart() < 0)
    {
        cout << "启动失败" << endl;
    }
    else
    {
        ;
    }
}
