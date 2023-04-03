#include "server.h"

int main(int argc, char** argv)
{
    int port = 0;
    //if(argc<2)
    //{
    //    printf("未设置监听端口\n");
    //    exit(0);
    //}

    //port = atoi(argv[1]);
    port = 7890;
    start_server(port);
}