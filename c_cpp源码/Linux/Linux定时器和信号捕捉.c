#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>

void pprint(int test)
{
    printf("ÒÑ²¶×½µÄÐÅºÅ: %d\n", test);
    return;
}

int main()
{
    struct itimerval time_newItem;
    time_newItem.it_value.tv_sec = 2;
    time_newItem.it_value.tv_usec = 0;

    time_newItem.it_interval.tv_sec = 3;
    time_newItem.it_interval.tv_usec = 0;

    struct sigaction myset_sig;
    myset_sig.sa_handler = pprint;
    sigemptyset(&myset_sig.sa_mask);
    myset_sig.sa_flags = 0;
    sigaction(SIGALRM, &myset_sig, NULL);
    sigaction(SIGINT, &myset_sig, NULL);
    sigaction(SIGKILL, &myset_sig, NULL);
    sigaction(SIGQUIT, &myset_sig, NULL);
    sigaction(SIGTSTP, &myset_sig, NULL);
    sigaction(SIGSTOP, &myset_sig, NULL);

    setitimer(ITIMER_REAL, &time_newItem, NULL);
    while (1) { }
    return 0;
}
