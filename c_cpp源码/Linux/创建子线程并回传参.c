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
        printf("callBack �������%d\n", *(int*)number);
        printf("��ǰ���߳�ID: %ld\n", pthread_self());
        t.age = 10;
        strcpy(t.name, "YiKang");
        pthread_exit(&t);		// �߳��˳�
    }

    int main()
    {
        pthread_t tid;   //�������̴߳����ɹ������̵߳�ID
        int number = 10;
        int ret = pthread_create(&tid, NULL, callBack, (void*)&number);
        if (ret != 0)
        {
            // char* serror = strerror(ret);
            printf("���̴߳���ʧ��, �����:%s\n ", strerror(ret));
            exit(0);
        }
        printf("���߳��߳�ID: %ld , ���߳�ID: %ld\n", tid, pthread_self());
        struct Test* test_ptr;
        pthread_join(tid, (void**)&test_ptr);
        printf("ptr->age: %d, ptr->name: %s\n", test_ptr->age, test_ptr->name);
        return 0;
    }