#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <termios.h>

pthread_t tid[4];

//status
int* WakeUp_Status;  
int* Spirit_Status; 
int* timerWUS;
int* timerSS;

//variabel pembuat disabled
int cAAB;
int cIAT;

//fungsi tuk thread
void* tWUS(void* arv);
void* tSS(void* arv);
void* tDisabledWUS(void* arv);
void* tDisabledSS(void* arv);

//fungsi non thread
char allStatusMode();
char menuMode();
/* reads from keypress, doesn't echo */
int getch(void);

int main()
{
    //alokasi argumen
    WakeUp_Status=malloc(sizeof(int*));  
    Spirit_Status=malloc(sizeof(int*)); 
    timerWUS=malloc(sizeof(int*));
    timerSS=malloc(sizeof(int*));
    *WakeUp_Status=0;
    *Spirit_Status=100;
    *timerWUS=10;
    *timerSS=10;
    cAAB=cIAT=1;
    //threading
    pthread_create(&tid[0],NULL,tWUS,(void*)WakeUp_Status);
    pthread_create(&tid[1],NULL,tSS,(void*)Spirit_Status);
    pthread_create(&tid[2],NULL,tDisabledWUS,(void*)timerWUS);
    pthread_create(&tid[3],NULL,tDisabledSS,(void*)timerSS);
    //main program
    char menu='2';
    while(1)
    {
        if (menu=='1')
            menu=allStatusMode();
        else if (menu=='2')
            menu=menuMode();
    }
    return 0;
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
void* tWUS(void* arv)
{
    int* w = (int*)arv;
    while(1)
    {
        if (*w >= 100)
        {
            system("clear");
            printf("Agmal Terbangun,mereka bangun pagi dan berolahraga\n");
            exit(1);
        }
    }  
}
void* tSS(void* arv)
{
    int* s = (int*)arv;
    while(1)
    {
        if (*s <= 0)
        {
            system("clear");
            printf("Iraj ikut tidur, dan bangun kesiangan bersama Agmal\n");
            exit(2);
        }
    }
}
void* tDisabledWUS(void* arv)
{   //disabled AAB
    int* disw = (int*)arv;
    int f;
    while(1)
    {
        if (cIAT % 4 == 0)
        {
            f=cIAT;
            while(*disw>=1)
            {
                *disw-=1;
                sleep(1);
            }
        }
        if (*disw == 0)
        {
            //kalo sequence ditunggu BBB nunggu baru milih
            if (f==cIAT)
                cIAT=1;
            //kalo gak ditunggu BBBAB langsung
            *disw=10;
        }
    }
}
void* tDisabledSS(void* arv)
{   //disabled IAT
    int* diss = (int*)arv;
    int f;
    while(1)
    {
        if (cAAB % 4 == 0)
        {
            f=cAAB;
            while(*diss>=1)
            {
                *diss-=1;
                sleep(1);
            }
        }
        if (*diss == 0)
        {
            //kalo sequence ditunggu AAA nunggu baru milih
            if (f==cAAB)
                cAAB=1;
            //kalo gak ditunggu AAABA langsung
            *diss=10;
        }
    }
}
char menuMode()
{
    while(1)
    {
        system("clear");
        printf("Menu Mode\n");
        printf("1. All Status\n2. Agmal Ayo Bangun\n3. Iraj Ayo Tidur\n");
        //cek pengaksesan fitur AAB
        if (*timerWUS != 10)  printf("Agmal Ayo Bangun disabled %d s\n",*timerWUS);
        //cek pengaksesan fitur IAT
        if (*timerSS != 10)  printf("Fitur Iraj Ayo Tidur disabled %d s\n",*timerSS);
        char c;
        c=getch();
        if (c=='1')
            return c;
        else if (c=='2')
        {
            if (cIAT % 4 != 0 && *timerWUS == 10)
            {
                *WakeUp_Status+=15;
                cAAB=cAAB+1;
            }
        }
        else if (c=='3')
        {
            if (cAAB % 4 != 0 && *timerSS == 10)
            {
                *Spirit_Status-=20;
                cIAT=cIAT+1;
            }
        }
        else
            continue;
    } 
}
char allStatusMode()
{
    while(1)
    {
        system("clear");
        printf("All Status Mode\n");
        printf("Agmal\tWakeUp_Status = %d\n",*WakeUp_Status);
        printf("Iraj\tSpirit_Status = %d\n",*Spirit_Status);
        printf("Choices\n");
        printf("1. Menu\n");
        char c;
        c=getch();
        if (c=='1') return '2';
    }
}