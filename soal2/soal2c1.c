#include <stdio.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#define PORT 9090

int main(int argc, char const *argv[]) {
    //main program
    while(1)
    {
        int sock = 0;
        //buat endpoint tuk komunikasi, ipv4 return filedescriptor > 0 jk sukses
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            printf("\n Socket creation error \n");
            return -1;
        }

        struct sockaddr_in serv_addr;
        memset(&serv_addr, '0', sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(PORT);

        //konversi dari string ke binary form dari src 192 sekian ke dst serv_addr.sin_addr
        //return <= 0 jk gagal, 1 jk berhasil
        if(inet_pton(AF_INET, "192.168.56.1", &serv_addr.sin_addr)<=0)
        {
            printf("\nInvalid address/ Address not supported \n");
            return -1;
        }
        //membuat koneksi socket di kiri ke addr di kanan, return <0 jk gagal, 0 jk berhasil
        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
            printf("\nConnection Failed \n");
            return -1;
        }
        //menunggu inputan
        char hello[100];
        scanf("%s",hello);
        char quit[10]={"keluar"};

        if ((strcmp(quit,hello)) == 0 )
        {
            send(sock , quit , strlen(quit) , 0 );
            break;
        }
        // kirim pesan ke socket
        send(sock , hello , strlen(hello) , 0 );
        // printf("Hello message sent\n");
    }
    return 0;
}