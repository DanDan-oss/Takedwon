#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void wait_function(int a)
{
    while (1)
    {
        pid_t pid_a = waitpid(-1, NULL, WNOHANG);

        if (pid_a == -1)
        {
            printf("所有的子进程以回收\n");
            exit(0);
        }
        else if (pid_a > 0)
        {
            printf("已回收pid：%d的进程\n", pid_a);
        }
    }

}

int main()
{
    pid_t ret_pid = 0;
    /* 创建20个子进程 */
    for (int i = 0; i < 20; i++)
    {
        ret_pid = fork();
        if (ret_pid == 0)
        {
            break;
        }
    }
    struct sigaction act;
    act.sa_handler = wait_function;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    if (ret_pid == 0)
    {
        printf("子进程PID: %d\n", getpid());
    }
    else if (ret_pid > 0)
    {
        sigaction(SIGCHLD, &act, NULL);
        printf("父进程pid:%d\n", getpid());
        while (1)
        {
            sleep(3);
        }
    }
    return 0;
}


