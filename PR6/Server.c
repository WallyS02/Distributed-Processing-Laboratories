#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#define MAX_MESSAGE_LENGHT 200
#define MAX_CLIENTS 10

HANDLE semaphore;

typedef struct clientSocket {
    SOCKET socket;
    char name[MAX_MESSAGE_LENGHT];
} clientSocket;

clientSocket clientSockets[MAX_CLIENTS];
int indexs[MAX_CLIENTS];
int numClients = 0;

int find(){
    for(int i=0; i<MAX_CLIENTS; i++){
        if(indexs[i] == 0){
            return i;
        }
    }
    return -1;
}

int compare(char a[], char b[], int l){
    for(int j = 0; j < l; j++){
        if(a[j] && b[j] && a[j]!=b[j]){
            return 0;
        }
    }
    return 1;
}

DWORD WINAPI handler(LPVOID data) {
    SOCKET acceptSocket = *(SOCKET *) data;
    char firstResponse[MAX_MESSAGE_LENGHT] = "Server says hello!\n";
    send(acceptSocket, firstResponse, MAX_MESSAGE_LENGHT, 0);
    char nickname[MAX_MESSAGE_LENGHT];
    recv(acceptSocket, nickname, MAX_MESSAGE_LENGHT, 0);
    int l = 0;
    while (nickname[l] != '\0')
        l++;
    nickname[l - 1] = '\0';
    WaitForSingleObject(semaphore, INFINITE);
    for (int i = 0; i < numClients; i++) {
        if (clientSockets[i].socket == acceptSocket) {
            for(int j = 0; j < l; j++) {
                clientSockets[i].name[j] = nickname[j];
            }
        }
    }
    for (int i = 0; i < numClients; i++) {
        if (clientSockets[i].socket != acceptSocket) {
            send(clientSockets[i].socket, nickname, MAX_MESSAGE_LENGHT, 0);
            send(clientSockets[i].socket, " connects to chat\n", MAX_MESSAGE_LENGHT, 0);
        }
    }
    ReleaseSemaphore(semaphore, 1, NULL);
    int k = 0;
    while (nickname[k] != '\0')
        k++;
    nickname[k] = ':';
    nickname[k + 1] = ' ';
    nickname[k + 2] = '\0';
    char recvbuf[MAX_MESSAGE_LENGHT];
    do {
        memset(recvbuf, 0, MAX_MESSAGE_LENGHT);
        recv(acceptSocket, recvbuf, MAX_MESSAGE_LENGHT, 0);
        if (strcmp(recvbuf, "end") != 0) {
            if(strcmp(recvbuf, "nick") == 0){
                char nick[MAX_MESSAGE_LENGHT];
                recv(acceptSocket, nick, MAX_MESSAGE_LENGHT, 0);
                int l = 0;
                while (nick[l] != '\0')
                    l++;
                nick[l - 1] = '\0';
                //printf("szukany nick: %s\n", nick);
                int flag = 0;
                WaitForSingleObject(semaphore, INFINITE);
                for (int i = 0; i < numClients; i++) {
                    //printf("jest: %s\n", clientSockets[i].name);
                    if (compare(nick, clientSockets[i].name, l) == 1) {
                        send(acceptSocket, "found", MAX_MESSAGE_LENGHT, 0);
                        flag++;
                        //printf("znaleziono: %s\n", clientSockets[i].name);
                        memset(recvbuf, 0, MAX_MESSAGE_LENGHT);
                        recv(acceptSocket, recvbuf, MAX_MESSAGE_LENGHT, 0);
                        //printf("Klient wyslsl wiadomosc: %s\n", recvbuf);
                        send(clientSockets[i].socket, nickname, MAX_MESSAGE_LENGHT, 0);
                        send(clientSockets[i].socket, recvbuf, MAX_MESSAGE_LENGHT, 0);
                    }
                }
                ReleaseSemaphore(semaphore, 1, NULL);
                if (flag == 0)
                    send(acceptSocket, "not found", MAX_MESSAGE_LENGHT, 0);
            }
            else if (strcmp(recvbuf, "end") != 0) {
                WaitForSingleObject(semaphore, INFINITE);
                for (int i = 0; i < numClients; i++) {
                    if (clientSockets[i].socket != acceptSocket) {
                        send(clientSockets[i].socket, nickname, MAX_MESSAGE_LENGHT, 0);
                        send(clientSockets[i].socket, recvbuf, MAX_MESSAGE_LENGHT, 0);
                    }
                }
                ReleaseSemaphore(semaphore, 1, NULL);
            }
        }
    } while(strcmp(recvbuf, "end") != 0);
    WaitForSingleObject(semaphore, INFINITE);
    for (int i = 0; i < numClients; i++) {
        if (clientSockets[i].socket != acceptSocket) {
            send(clientSockets[i].socket, nickname, MAX_MESSAGE_LENGHT, 0);
            send(clientSockets[i].socket, "leaves chat\n", MAX_MESSAGE_LENGHT, 0);
        }
        else{
            clientSockets[i].socket = 0;
            strcpy(clientSockets[i].name, "none");
            indexs[i] = 0;
        }
    }
    ReleaseSemaphore(semaphore, 1, NULL);
    closesocket( acceptSocket );
}


int main() {
    semaphore = CreateSemaphore(NULL, 10, 10, NULL);
    if (semaphore == NULL) {
        printf("Błąd podczas tworzenia semafora\n");
        return 1;
    }
    for(int i = 0; i<MAX_CLIENTS; i++){
        indexs[i] = 0;
    }
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if( result != 0 )
        printf( "Initialization error.\n" );
    SOCKET mainSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if( mainSocket == INVALID_SOCKET ) {
        printf("Error creating socket: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    struct sockaddr_in service;
    memset(&service, 0, sizeof(service));
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = INADDR_ANY;;
    service.sin_port = htons( 6969 );
    if( bind( mainSocket,( SOCKADDR * ) &service, sizeof( service ) ) == SOCKET_ERROR ) {
        printf( "bind() failed.\n" );
        closesocket( mainSocket );
        return 1;
    }
    if( listen( mainSocket, 5 ) == SOCKET_ERROR )
        printf( "Error listening on socket.\n" );
    SOCKET acceptSocket;
    while((acceptSocket = accept(mainSocket, NULL, NULL))) {
        if(numClients == MAX_CLIENTS-1){
            numClients=0;
            while(numClients < MAX_CLIENTS){
                if(!strcmp(clientSockets[numClients].name, "none")){
                    break;
                }
                numClients++;
            }
        }
        clientSockets[numClients++].socket = acceptSocket;
        HANDLE thread = CreateThread(NULL, 0, handler, &acceptSocket, 0, NULL);
        CloseHandle(thread);
    }
}