//
// Created by cxy on 2023/2/21.
//
#include <arpa/inet.h>
#include <netinet/in.h>
#include "sys/socket.h"
#include "sys/epoll.h"
#include "sys/procfs.h"
#include "iostream"
#include <unistd.h>
#define MAXconnection 1000
#define MAXevents 4000
#define  buff_size 4096

void start_server(uint16_t port,in_addr_t ipaddr){
    char buff[buff_size];
    sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);
    serveraddr.sin_addr.s_addr = ipaddr;

    int server_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(server_sock<0){
        std::cerr<<"creat sock failed"<<std::endl;
        exit(0);
    }
    if(bind(server_sock,(sockaddr *)&serveraddr,sizeof (sockaddr))<0){
        std::cerr<<"bind failed"<<std::endl;
        exit(-1);
    }
    listen(server_sock,MAXconnection);
    int epollfd  = epoll_create(MAXevents);
    if(!epollfd){
        std::cerr<<"creat epollfd failed"<<std::endl;
        exit(-1);
    }
    epoll_event events[MAXevents],ev;
    ev.events = EPOLLIN;
    ev.data.fd = server_sock;
    if(epoll_ctl(epollfd,EPOLL_CTL_ADD,server_sock,&ev)==-1){
        std::cerr<<"add EPOLLIN failed"<<std::endl;
        exit(-1);
    }
    int evevtnums;
    while(1){
        //-1表示永不超时
        evevtnums = epoll_wait(epollfd,events,MAXevents,-1);
        for(int i = 0;i<evevtnums;i++){
            //如果是服务器sock收到了连接请求
            if(events[i].data.fd==server_sock){
                sockaddr_in clientaddr;
                unsigned long addrlen =  sizeof(sockaddr_in);
                int con_sock = accept(server_sock,(sockaddr*)&clientaddr, (socklen_t*)&addrlen);
                if(con_sock<0){
                    std::cerr<<"accept failed"<<std::endl;
                    exit(-1);
                }
                ev.data.fd = con_sock;
                if(epoll_ctl(epollfd,EPOLL_CTL_ADD,con_sock,&ev)<0){
                    std::cerr<<"add new EPOLLIN failed"<<std::endl;
                    exit(-1);
                }else std::cout<<"accept client. ip :"<<ntohl(clientaddr.sin_addr.s_addr)<<" port :"<<ntohs(clientaddr.sin_port)<<std::endl;
            }else{//客户端发来了消息
                int buff_bytes = read(events[i].data.fd,buff,buff_size);
                if(buff_bytes==0){//此时代表对端 关闭连接
                    epoll_ctl(epollfd,EPOLL_CTL_DEL,events[i].data.fd,NULL);
                    close(events[i].data.fd);
                    std::cout<<"peer close"<< std::endl;
                    break;
                }
                std::cout<<"messages from sock :"<<events[i].data.fd<<std::endl;
                for(int i = 0;i<buff_bytes;i++){
                    std::cout<<buff[i];
                }
            }
        }
    }

}

