#include<iostream>
#include <winsock2.h>
using namespace std;
int main(int argc, char const *argv[])
{
    WSADATA wsaData;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2,2);
    wsaerr = WSAStartup(wVersionRequested,&wsaData);
    if (wsaerr != 0){
        cout<<"The Winsock dll not found!"<<endl;
        return 0;
    }
    else{
        cout<<"The Winsock dll found"<<endl;
        cout<<"Status: "<<wsaData.szSystemStatus<<endl;
    }
    return 0;
}

#include<iostream>
#include <winsock2.h>
using namespace std;
int main(int argc, char const *argv[])
{
    WSADATA wsaData;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2,2);
    wsaerr = WSAStartup(wVersionRequested,&wsaData);
    if (wsaerr != 0){
        cout<<"The Winsock dll not found!"<<endl;
        return 0;
    }
    else{
        cout<<"The Winsock dll found"<<endl;
        cout<<"Status: "<<wsaData.szSystemStatus<<endl;
    }
    return 0;
}
