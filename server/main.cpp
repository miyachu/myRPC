//
// Created by cxy on 2023/2/21.
//
#include "server.cpp"

int main(int argc,char * argv[]){
    if(argc!=3){
        std::cerr<<"Missing parameter"<<std::endl;
        exit(-1);
    }
    start_server(atoi(argv[1]),inet_addr(argv[2]));
}