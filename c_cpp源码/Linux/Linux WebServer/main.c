#include "server.h"
#include <string.h>
#define SERVER_PORT 7890
#define SERVER_HOM_PATH "/home/mohui/code/webserver/home"

int main(int argc, char** argv)
{
    pid_t pid = 0;
    int fd = 0;
    int port = 0;
    char server_homepath[128] = {0};
    switch (argc)
    {
    case 1:
        port = SERVER_PORT;
        memcpy(server_homepath, SERVER_HOM_PATH, strlen(SERVER_HOM_PATH));
        break;
    case 2:
        port = atoi(argv[1]);
        memcpy(server_homepath, SERVER_HOM_PATH, strlen(SERVER_HOM_PATH));
        break;
    case 3:
    default:
        port = atoi(argv[1]);
        memcpy(server_homepath, (char*)&argv[2], strlen((char*)&argv[2]));
        break;
    }
    pid = fork();
    if(pid>0)
        exit(0);
    pid = setsid();
    if(!pid)
    {
        printf("%s:%d 子进程提升会话失败\n", __FILE__, __LINE__);
        exit(0);
    }
    umask(0777);
    printf("server starup home[%s], port=%d\n", getcwd(server_homepath, sizeof(server_homepath)), port);
    fd = open("/dev/null", O_RDWR);
    dup2(fd, STDIN_FILENO);
    dup2(fd, STDERR_FILENO);
    chdir(server_homepath);
    start_server(port);
}