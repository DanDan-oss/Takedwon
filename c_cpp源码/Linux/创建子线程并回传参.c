#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
        struct Test
    {
        int age;
        char name[20];
    };

    struct Test t;

    void* callBack(void* number)
    {
        printf("callBack 传入参数%d\n", *(int*)number);
        printf("当前子线程ID: %ld\n", pthread_self());
        t.age = 10;
        strcpy(t.name, "YiKang");
        pthread_exit(&t);		// 线程退出
    }

    int main()
    {
        pthread_t tid;   //保存子线程创建成功后子线程的ID
        int number = 10;
        int ret = pthread_create(&tid, NULL, callBack, (void*)&number);
        if (ret != 0)
        {
            // char* serror = strerror(ret);
            printf("子线程创建失败, 错误号:%s\n ", strerror(ret));
            exit(0);
        }
        printf("子线程线程ID: %ld , 父线程ID: %ld\n", tid, pthread_self());
        struct Test* test_ptr;
        pthread_join(tid, (void**)&test_ptr);
        printf("ptr->age: %d, ptr->name: %s\n", test_ptr->age, test_ptr->name);
        return 0;
    }