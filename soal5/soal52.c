#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <termios.h>
#include <unistd.h>

key_t kunci = 1400;

/* reads from keypress, doesn't echo */
int getch(void);

int main()
{
    int *item;
    int shmid = shmget(kunci, sizeof(int), IPC_CREAT | 0666);
    item = shmat(shmid,NULL,0);
    while(1)
    {
        system("clear");
        printf("Shop\n");
        printf("Food stock\t: %d\n",*item);
        printf("Choices\n");
        printf("1. Restock\n2. Exit\n");
        char c;
        c=getch();
        if (c=='1')
        {
            *item+=1;
        }
        else if (c=='2')
        {
            shmdt(item);
            shmctl(shmid, IPC_RMID, NULL);
            exit(5);
        }
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