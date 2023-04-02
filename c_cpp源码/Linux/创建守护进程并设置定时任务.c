#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
void func(int a)
{

    time_t tm = time(NULL);
    struct tm* loc = localtime(&tm);
    char* curtim = asctime(loc);
    int prrt = open("./pptr.log", O_WRONLY | O_CREAT, 0777);
    if (prrt == -1)
    {
        printf("error: No open file pptr.log \n");
        exit(1);
    }
    write(prrt, curtim, strlen(curtim));
    close(prrt);
}

int main()
{
    pid_t ret_pid = fork();
    if (ret_pid > 0)
    {
        exit(0);
    }
    pid_t ret_sid = setsid();
    if (ret_sid == 0)
    {
        printf("子进程提升会话失败\n");
        return -1;
    }
    int fd = open("./error.log", O_WRONLY | O_CREAT, 0777);
    if (fd == -1)
    {
        printf("error: NO open file error.log\n");
        return 0;
    }

    dup2(fd, STDIN_FILENO);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);

    /* 设置捕捉信号函数,当计时器时间到时会发送SIGALRM信号,执行计时任务*/
    struct sigaction sig_tm;
    sig_tm.sa_handler = func;
    sigemptyset(&sig_tm.sa_mask);
    sig_tm.sa_flags = 0;
    sigaction(SIGALRM, &sig_tm, NULL);

    /* 设置计时器的循环周期 */
    struct itimerval time_tm;
    time_tm.it_value.tv_sec = 2;
    time_tm.it_value.tv_usec = 0;
    time_tm.it_interval.tv_sec = 3;
    time_tm.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &time_tm, NULL);

    while (1)
    {

    }
    return 0;
}

