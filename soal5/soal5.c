#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/ipc.h>
#include <sys/shm.h>

pthread_t tid[50];
key_t kunci = 1400;

//status
int* hunger;  
int* hygiene; 
int* health;  
int* bath;
int* food;
int* item;
int shmidd;

//name
char name[50];

//fungsi tuk thread
void* tHunger(void* arv);
void* tHygiene(void* arv);
void* tHealth(void* arv);
void* tBath(void* arv);

//fungsi non thread
void tControl(int src, int dst);
char battleMode();
char standbyMode();
char shopMode();
/* reads from keypress, doesn't echo */
int getch(void);

int main()
{
    //init nama monster
    printf("Masukkan nama monster: ");
    scanf("%s",name);
    //alokasi argumen
    hunger=malloc(sizeof(int*));
    hygiene=malloc(sizeof(int*));
    health=malloc(sizeof(int*));
    bath=malloc(sizeof(int*));
    food=malloc(sizeof(int*));
    // item=malloc(sizeof(int*));
    *hunger=200;
    *hygiene=100;
    *health=300;
    *bath=20;
    *food=0;
    // *item=0;
    // int *value;
    int shmid = shmget(kunci, sizeof(int), IPC_CREAT | 0666);
    shmidd = shmid;
    item = shmat(shmid,NULL,0);
    //init stok
    *item = 5;
    tControl(0,0);
    //main program
    char menu='2';
    while(1)
    {
        if (menu=='2')
            menu=standbyMode();
        else if (menu=='3')
            menu=battleMode();
        else if (menu=='4')
            menu=shopMode();
    }
    return 0;
}

void tControl(int src, int dst)
{
    if (src==0 && dst==0)
    {
        //thread dari init state ke standby mode
        pthread_create(&tid[0],NULL,tHealth,(void*)health);
        pthread_create(&tid[1],NULL,tHunger,(void*)hunger);
        pthread_create(&tid[2],NULL,tHygiene,(void*)hygiene);
        pthread_create(&tid[3],NULL,tBath,(void*)bath);
    }
    else if (src==0 && dst==1)
    {
        //thread dari standby mode ke battle mode
        pthread_cancel(tid[0]);
        pthread_cancel(tid[1]);
        pthread_cancel(tid[2]);
    }
    else if (src==0 && dst==2)
    {
        //thread dari standby mode ke shop mode
        pthread_cancel(tid[0]);
    }
    else if (src==1 && dst==0)
    {
        //thread dari battle mode ke standby mode
        pthread_create(&tid[0],NULL,tHealth,(void*)health);
        pthread_create(&tid[1],NULL,tHunger,(void*)hunger);
        pthread_create(&tid[2],NULL,tHygiene,(void*)hygiene);
    }
    else if (src==2 && dst==0)
    {
        //thread dari shop mode ke standby mode
        pthread_create(&tid[0],NULL,tHealth,(void*)health);
    }
}
int getch(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}
void* tHunger(void* arv)
{
    int* hu = (int*)arv;
    while(1)
    {
        if (*hu-5 > 0)
        {
            *hu=*hu-5;
            sleep(10);    
        }
        else if (*hu > 200)
            *hu=200;
        else
        {
            printf("\nKalah karena hunger 0");
            exit(1);
            //detach
            shmdt(item);
            shmctl(shmidd, IPC_RMID, NULL);
        }
    }
}
void* tHygiene(void* arv)
{
    int* hy = (int*)arv;
    while(1)
    {
        if (*hy-10 > 0)
        {
            *hy=*hy-10;
            sleep(30);    
        }
        else
        {
            printf("\nKalah karena hygiene 0");
            exit(2);
            //detach
            shmdt(item);
            shmctl(shmidd, IPC_RMID, NULL);
        }
    }
}
void* tHealth(void* arv)
{
    int* he = (int*)arv;
    while(1)
    {
        *he=*he+5;
        sleep(10);
    }
}
void* tBath(void* arv)
{
    int* ba = (int*)arv;
    while(1)
    {
        if(*ba>0)
        {
            *ba=*ba-1;
            sleep(1);
        }
    }
}
char standbyMode()
{
    while(1)
    {
        system("clear");
        printf("Standby Mode\n");
        printf("Health\t\t: %d\n",*health);
        printf("Hunger\t\t: %d\n",*hunger);
        printf("Hygiene\t\t: %d\n",*hygiene);
        printf("Food left\t: %d\n",*food);
        if (*bath != 0)  printf("Bath will be ready in %ds\n",*bath);
        else printf("Bath is ready\n");
        printf("Choices\n");
        printf("1. Eat\n2. Bath\n3. Battle\n4. Shop\n5. Exit\n");
        char c;
        c=getch();
        if (c=='1')
        {
            if (*food>=1)
            {
                *hunger=*hunger+15;
                *food=*food-1;
            }
        }
        else if (c=='2')
        {
            if (*bath <=0)
            {
                *hygiene=*hygiene+30;
                *bath=20;
            }
        }
        else if (c=='3')
        {
            tControl(0,1);
            return c;
        }
        else if (c=='4')
        {
            tControl(0,2);
            return c;
        }
        else if (c=='5')
        {
            exit(3);
            //detach memory
            shmdt(item);
            shmctl(shmidd, IPC_RMID, NULL);
        }
    } 
}
char battleMode()
{
    int* enemy;
    enemy=malloc(sizeof(int*));
    *enemy=100;
    while(1)
    {
        if (*enemy==0)
        {
            tControl(1,0);
            return '2';
        }else if (*health<=0)
        {
            printf("Kalah karena health 0\n");
            exit(4);
            //detach memory
            shmdt(item);
            shmctl(shmidd, IPC_RMID, NULL);
        }
        system("clear");
        printf("Battle Mode\n");
        printf("%s's Health\t\t: %d\n", name,*health);
        printf("Enemy's Health\t\t: %d\n",*enemy);
        printf("Choices\n");
        printf("1. Attack\n2. Run\n");
        char c;
        c=getch();
        if (c=='1')
        {
            *enemy=*enemy-20;
            *health=*health-20;
        }
        else if (c=='2')
        {
            tControl(1,0);
            return c;
        }
    }
}
char shopMode()
{
    while(1)
    {
        system("clear");
        printf("Shop Mode\n");
        printf("Shop food stock\t: %d\n",*item);
        printf("Your food stock\t: %d\n",*food);
        printf("Choices\n");
        printf("1. Buy\n2. Back\n");
        char c;
        c=getch();
        if (c=='1')
        {
            if (*item - 1 >= 0)
            {
                *item-=1;
                *food=*food+1;   
            }
        }
        else if (c=='2')
        {
            tControl(2,0);
            return c;
        }
    }
}