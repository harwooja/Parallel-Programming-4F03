#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <rpc/rpc.h>
#include <pthread.h>
#include "server_verify.h"
#include "server_verify_rpc.h"

#define MYPORT 3350

// Socket Specific Variables
int sock, acceptSock;
struct sockaddr_in serv_addr;
// String specific variable
struct sstr *strObj;

void initServerAddresses(){
    memset((char *) &serv_addr, 0, sizeof(serv_addr));    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(MYPORT);
}

// Create TCP Socket this server
void createVerifyServerSocket() {
   	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == -1)
		exit(0);
	else         
        printf("Server Verify TCP Socket created successfully.\n\n");
    
    int bindPort = bind(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)); 

    if (bindPort < 0) {
        printf("Verify Server: Error binding port \n");
        exit(1);
    } else
        printf("Verify Server: Bound port %i \n", MYPORT);

    // Listen for incoming requests (non-blocking)
    int listenStatus = listen(sock, 10);

    if (listenStatus < 0) {
        printf("Verify Server: Error, we are not listening for incoming requests \n");
        exit(1);
    } else
        printf("Verify Server: We are now listening for incoming requests \n");
}


// Accept any incoming connections from our client
void acceptConnection() {
     printf("Verify Server: Now accepting incoming connections \n");
     // Accept any incoming connections (blocking)
    acceptSock = accept(sock, (struct sockaddr *) NULL, NULL);
    if (acceptSock < 0) { 
        printf("Verify Server: Error with accepting request for connection \n");
        exit(0);
    } else 
        printf("Verify Server: Incoming request accepted \n");
}


void receiveString() {    
    int bufferLength = substring_Length * substring_Partitions;
    char rcvBuf[bufferLength];

    int recvMessage = recv(acceptSock, &rcvBuf, sizeof(rcvBuf), 0);
    if (recvMessage < 0)
        printf("Verify Server: Error receiving Message \n");
    else
        (*strObj).charArray = rcvBuf;
        
    printf("Verify Server: Accepted and saved string: %s \n", (*strObj).charArray);
}



//Remote version of "rpc_initappendserver_1" 
int * rpcinitverifyserver_1_svc(VerifyArguments *msg,  struct svc_req *svc) {
    static int result = 1;       /* must be static! */

    thread_count = (*msg).thread_count;
    substring_Length = (*msg).substring_Length;
    substring_Partitions = (*msg).substring_Partitions;

    if (strObj == NULL) {
        strObj = malloc(sizeof(sstr));
        (*strObj).charIndex = 0;
        (*strObj).charArray = malloc(substring_Length * substring_Partitions + 1);
    }
    
    pthread_t* thread_handles;
    thread_handles = malloc(thread_count*sizeof(pthread_t));
    pthread_create(&thread_handles[0], NULL, socketWork, NULL);
    free(thread_handles);
 
 
    printf("Server Verify: Values have been initiated \n");
    
    return(&result);
}

int * rpcgetseg_1_svc(void *a, struct svc_req *svc){
    static int result = 1;       /* must be static! */

    return(&result);
}

void *socketWork() {
    initServerAddresses();
    createVerifyServerSocket();
    acceptConnection();    
    receiveString();
    return NULL;
}


