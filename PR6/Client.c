#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <conio.h>

#define MAX_MESSAGE_LENGHT 200

int main() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if( result != 0 )
        printf( "Initialization error.\n" );
    SOCKET mainSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if( mainSocket == INVALID_SOCKET )
    {
        printf("Error creating socket: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    struct sockaddr_in service;
    memset(&service, 0, sizeof(service));
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr( "42.0.232.100" );
    service.sin_port = htons( 6969 );
    if( connect( mainSocket,( SOCKADDR * ) &service, sizeof(service) ) == SOCKET_ERROR ) {
        printf( "Failed to connect.\n" );
        WSACleanup();
        return 1;
    }
    char responseStart[MAX_MESSAGE_LENGHT] = "";
    recv(mainSocket, responseStart, MAX_MESSAGE_LENGHT, 0);
    printf("%s\n", responseStart);
    printf("Input your nickname: ");
    char nickname[MAX_MESSAGE_LENGHT];
    fgets(nickname, MAX_MESSAGE_LENGHT, stdin);
    send(mainSocket, nickname, MAX_MESSAGE_LENGHT, 0);
    int exit = 0;
    char command;
    printf("a. Send message\nb. Exit chat\nc. Privet msg \n");
    char sendbuf[MAX_MESSAGE_LENGHT];
    char recvbuf[MAX_MESSAGE_LENGHT];
    while(exit == 0) {
        fd_set readSet;
        FD_ZERO(&readSet);
        FD_SET(mainSocket, &readSet);
        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;
        u_long iMode = 1;
        ioctlsocket(mainSocket, FIONBIO, &iMode);
        int ready = select(0, &readSet, NULL, NULL, &timeout);
        if (ready == SOCKET_ERROR) {
            printf("Error in select(): %d\n", WSAGetLastError());
            break;
        } else if (ready > 0) {
            int received;
            do {
                received = recv(mainSocket, recvbuf, MAX_MESSAGE_LENGHT, 0);
                printf("%s", recvbuf);
                memset(recvbuf, 0, MAX_MESSAGE_LENGHT);
            } while (received > 0);
        }
        iMode = 0;
        ioctlsocket(mainSocket, FIONBIO, &iMode);
        if (_kbhit()) {
            command = _getch();
            if (command == 'a') {
                printf("Input text to send: ");
                fgets(sendbuf, MAX_MESSAGE_LENGHT, stdin);
                send(mainSocket, sendbuf, MAX_MESSAGE_LENGHT, 0);
                memset(sendbuf, 0, MAX_MESSAGE_LENGHT);
            }
            if (command == 'b') {
                send(mainSocket, "end", MAX_MESSAGE_LENGHT, 0);
                exit = 1;
            }
            if(command == 'c'){
                send(mainSocket, "nick", MAX_MESSAGE_LENGHT, 0);
                printf("To who you want to send msg: ");
                fgets(sendbuf, MAX_MESSAGE_LENGHT, stdin);
                send(mainSocket, sendbuf, MAX_MESSAGE_LENGHT, 0);
                memset(sendbuf, 0, MAX_MESSAGE_LENGHT);
                recv(mainSocket, recvbuf, MAX_MESSAGE_LENGHT, 0);
                printf("%s\n", recvbuf);
                if(strcmp(recvbuf, "found") == 0) {
                    memset(recvbuf, 0, MAX_MESSAGE_LENGHT);

                    printf("What you want to send: ");
                    fgets(sendbuf, MAX_MESSAGE_LENGHT, stdin);
                    send(mainSocket, sendbuf, MAX_MESSAGE_LENGHT, 0);
                    memset(sendbuf, 0, MAX_MESSAGE_LENGHT);
                }
            }
        }
      }
    closesocket(mainSocket);
    WSACleanup();
}