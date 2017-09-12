#include <stdio.h> //printf
#include <string.h>    //strlen
#include <sys/socket.h>    //socket
#include <arpa/inet.h> //inet_addr 
#include <sys/time.h>   
#include <stdlib.h>
#include <unistd.h> // for close

#define MAXTEST 100
#define MESSAGESIZE 100 // in byte
#define MAXMESSAGESIZE 200000
FILE *file;
 
int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char server_reply[MAXMESSAGESIZE];
     
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    server.sin_addr.s_addr = inet_addr("192.168.1.99");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );
 
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
     
    puts("Connected\n"); 

    int test = 0;
    char *message = malloc(MESSAGESIZE + 1);
    memset(message, 'a', MESSAGESIZE);
    message[MESSAGESIZE] = 0;

    struct timeval tval_before, tval_after, tval_result;
    file = fopen("timer.txt", "a");
    //keep communicating with server
    while(test++ < MAXTEST)
    {
        // Timer set
        gettimeofday(&tval_before, NULL);

        //Send some data
        if( send(sock , message , strlen(message) , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
         
        //Receive a reply from the server
        if( recv(sock , server_reply , MAXMESSAGESIZE , 0) < 0)
        {
            puts("recv failed");
            break;
        }

        gettimeofday(&tval_after, NULL);
        timersub(&tval_after, &tval_before, &tval_result);
        fprintf(file, "%ld.%06ld ms\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);

        //puts("Server reply :");
        //puts(server_reply);
    }
     
    close(sock);
    return 0;
}
