#include <stdio.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#define PORT 9090

pthread_t tid;
key_t key = 1500;

void* tCetak(void* arv);

int main(int argc, char const *argv[]) {
    int server_fd;
    int opt = 1;
    //buat endpoint tuk komunikasi, ipv4 return filedescriptor > 0 jk sukses
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {   //jika return 0 maka ada yg salah
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    //manipulasi opsi pada level API(2), protokol tertentu(3), (4) dan (5) kebutuhan banyak akses, return 0 jk sukses
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    //membuat namespace socket memiliki addr dari struct, return <0 jk gagal
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
        
    // main program
    int *value;
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = shmat(shmid,NULL,0);
    //init stok
    *value = 5;
    // threading
    pthread_create(&tid,NULL,tCetak,NULL);
    // kirim pesan ke socket
    // char *hello = "Hello from server1";
    // send(new_socket , hello , strlen(hello) , 0 );
    // printf("Hello message sent\n");

    while(1)
    {    
        //untuk mendengarkan koneksi pd socket, dgn max length queue pending connection 3, return <0 jk gagal
        if (listen(server_fd, 3) < 0)
        {
            perror("listen");
            exit(EXIT_FAILURE);
        }
        
        int new_socket;
        int addrlen = sizeof(address);
        //menerima koneksi dari socket, return <0 kalo gagal
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        //baca dari koneksi socket
        char buffer[1024] = {0};
        char query[10]={"tambah"};
        char quit[10]={"keluar"};
        read( new_socket , buffer, 1024);
        // printf("%s\n",buffer );

        if ((strcmp(query,buffer)) == 0 )
            *value+=1;
        // buat keluar
        else if ((strcmp(quit,buffer)) == 0 )
            break;
    }
    //detach memory dan destroy
    shmdt(value);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}

void* tCetak(void* arv)
{
    while(1)
    {
        int *val;
        int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
        val = shmat(shmid,NULL,0);
        printf("Stok saat ini %d\n",*val);
        shmdt(val);
        shmctl(shmid, IPC_RMID, NULL);
        sleep(5);
    }
}