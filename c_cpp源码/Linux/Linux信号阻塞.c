#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{
    sigset_t mysig_set;
    sigaddset(&mysig_set, SIGINT);
    sigaddset(&mysig_set, SIGQUIT);
    sigaddset(&mysig_set, SIGTSTP);
    sigaddset(&mysig_set, SIGKILL);
    sigprocmask(SIG_BLOCK, &mysig_set, NULL);
    int for_num = 0;
    while (1)
    {
        sigpending(&mysig_set);
        for (int i = 0; i < 31; i++)
        {
            int ret = sigismember(&mysig_set, i + 1);
            if (ret == 1)
                printf("1");
            else if (ret == 0)
                printf("0");
            else
            {
                perror("sigismember is fail.\n");
                exit(1);
            }
        }
        sleep(2);
        printf("\n");
        for_num++;
        if (for_num == 40)
            break;
    }
    return 0;
}

