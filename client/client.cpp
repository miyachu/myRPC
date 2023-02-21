
//
// Created by cxy on 2023/2/21.
//
#include "arpa/inet.h"
#include <netinet/in.h>
#include "sys/socket.h"
#include "sys/epoll.h"
#include "sys/procfs.h"
#include "iostream"
#include <unistd.h>
#include "cstring"
#define  buff_size 4096

void start_client(uint16_t port,in_addr_t ipaddr){
    char buff[buff_size];
    sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);
    serveraddr.sin_addr.s_addr = ipaddr;

    int sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(sock<0){
        std::cerr<<"creat sock failed"<<std::endl;
        exit(-1);
    }

    if(connect(sock,(sockaddr*)&serveraddr, sizeof(serveraddr))<0){
        std::cerr<<"connect failed"<<std::endl;
        exit(-1);
    }

    while(1){
        std::cout<<"pelase enter messages"<<std::endl;
        fgets(buff,buff_size,stdin);
        int len = write(sock,buff,strlen(buff));
        std::cout<<"successfully sent "<<len<<"bytes"<<std::endl;
    }

}

