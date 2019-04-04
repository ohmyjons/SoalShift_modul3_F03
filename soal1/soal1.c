#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#define llu unsigned long long

pthread_t tid[50];
llu result[50] = {0};

void* fact_dp(void* arv);

int main(int argc, char* argv[])
{
    //invalid cmd argument
    if (argc <2)
    {
        printf("minimal satu argumen\n");
        exit(1);
    }
    //processing factorial by threading
    int flag[50] = {0};
    int c=0;
    int* arg=malloc(sizeof(int*));
    while(argc>1)
    {
        *arg=(atoi(argv[c+1]));
        pthread_create(&tid[c],NULL,fact_dp,(void*)arg);
        flag[*arg]=1;
        argc--;
        c++;
        pthread_join(tid[c-1],NULL);
        //join until any thread eliminate
    }
    int l=0;
    //display the output from array sort asc
    while(l<50)
    {
        if (flag[l] != 0)
            printf("%d! = %llu\n",l,result[l]);
        l++;
    }
    return 0;
}

void* fact_dp(void* arv)
{
    int *n = (int*)arv;
    // printf("n=%d\n",*n);
    if (*n >= 0) 
    {
        if (result[*n] != 0)
            return NULL;
        else
        {
            result[0] = 1;
            for (int i = 1; i <= *n; ++i) 
                result[i] = i * result[i - 1];
            return NULL;
        }
    }
}