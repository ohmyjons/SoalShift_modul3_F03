#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

pthread_t tid[2];

//fungsi thread
void* tProses(void* arv);
void* tZip(void* arv);
void* tRemove(void* arv);
void* tUnzip(void* arv);

int main()
{
    char p1[]={"~/Document/FolderProses1/SimpanProses1.txt"};
    char p2[]={"~/Document/FolderProses2/SimpanProses2.txt"};
    //melakukan saving file
    pthread_create(&tid[0],NULL,tProses,(void*)p1);
    pthread_create(&tid[1],NULL,tProses,(void*)p2);
    //melakukan kompress file ke zip
    pthread_join(tid[0],NULL);
    pthread_join(tid[1],NULL);
    pthread_create(&tid[0],NULL,tZip,(void*)p1);
    pthread_create(&tid[1],NULL,tZip,(void*)p2);
    //melakukan remove file txt
    pthread_join(tid[0],NULL);
    pthread_join(tid[1],NULL);
    pthread_create(&tid[0],NULL,tRemove,(void*)p1);
    pthread_create(&tid[1],NULL,tRemove,(void*)p2);
    //melakukan dekompress zip
    pthread_join(tid[0],NULL);
    pthread_join(tid[1],NULL);
    pthread_create(&tid[0],NULL,tUnzip,NULL);
    pthread_create(&tid[1],NULL,tUnzip,NULL);
    pthread_join(tid[0],NULL);
    pthread_join(tid[1],NULL);
    return 0;
}

void* tProses(void* arv)
{
    char in[200]={"ps -aux | head -n 11 > "};
    strcat(in,(char*)arv);
    printf("%s\n",in);
    system(in);
    return NULL;
}
void* tZip(void* arv)
{
    char in[200]={"zip ~/Document/FolderProses"};
    //jika tid yang pertama
    if (pthread_equal(pthread_self(),tid[0]))
        strcat(in,"1/KompresProses1.zip -j ");
    //jika tid yang kedua
    else if (pthread_equal(pthread_self(),tid[1]))
        strcat(in,"2/KompresProses2.zip -j ");
    strcat(in,(char*)arv);
    printf("%s\n",in);
    system(in);
    return NULL;
}
void* tRemove(void* arv)
{
    char in[200]={"rm "};
    strcat(in,(char*)arv);
    printf("%s\n",in);
    system(in);
    //menunggu selama 15 detik
    printf("Menunggu 15 detik untuk mengekstrak kembali\n");
    sleep(15);
    return NULL;
}
void* tUnzip(void* arv)
{
    char in[200]={"unzip "};
    //jika tid yang pertama
    if (pthread_equal(pthread_self(),tid[0]))
        strcat(in,"~/Document/FolderProses1/KompresProses1.zip -d ~/Document/FolderProses1/");
    //jika tid yang kedua
    else if (pthread_equal(pthread_self(),tid[1]))
        strcat(in,"~/Document/FolderProses2/KompresProses2.zip -d ~/Document/FolderProses2/");
    printf("%s\n",in);
    system(in);
    return NULL;
}