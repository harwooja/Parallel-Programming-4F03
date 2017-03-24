#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <rpc/rpc.h>

#include "server_append.h"
#include "server_append_rpc.h"

#define VERIFY_SERVER_PORT 3350
#define MYPORT 3654

// Socket Specific Variables
int incomingSocket, outgoingSocket;
int characterCount = 0;
struct sockaddr_in serverAddress, verifyServerAddress;
char rcvBuf[2];
// String specific variable
struct sstr *strObj;

void initServerAddresses() {
    // Append Server Address (This)
    memset((char *) &serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(MYPORT);
    
    // Verify Server Address
    memset((char *) &verifyServerAddress, 0, sizeof(verifyServerAddress));
    verifyServerAddress.sin_family = AF_INET;
    verifyServerAddress.sin_port = htons(VERIFY_SERVER_PORT);
    inet_aton((const char *) verify_host_name, &verifyServerAddress.sin_addr);
}


// Create TCP Socket this server
void createAppendServerSocket() {
   	incomingSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (incomingSocket == -1)
		exit(0);
	else {        
        printf("Server Append TCP Socket created successfully.\n\n");
    } 

    int bindPort = bind(incomingSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)); 

    if (bindPort < 0) {
        printf("Append Server: Error binding port \n");
        exit(1);
    } else
        printf("Append Server: Bound port %i \n", MYPORT);

    // Listen for incoming requests (non-blocking)
    int listenStatus = listen(incomingSocket, 10);

    if (listenStatus < 0) {
        printf("Append Server: Error, we are not listening for incoming requests \n");
        exit(1);
    } else
        printf("Append Server: We are now listening for incoming requests \n");

}


// Construct the string [chars are from Client]
void constructString(char c) {
    if(characterCount != 0) {
        int charIndex = (*strObj).charIndex;
        (*strObj).charArray[charIndex] = c;
        (*strObj).charIndex = charIndex + 1;
        characterCount--;
    }   
}


void connectVerifyServer() {
    printf("Append Server: Now attempting to connect to the Verify server \n");
    
    outgoingSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (outgoingSocket < 0) {
        printf("Append Server: Error Creating client-based Socket. Exiting. \n");
		exit(0);
    }
        
    int attemptConnection = connect(outgoingSocket, (struct sockaddr *) &verifyServerAddress, (socklen_t)sizeof(verifyServerAddress));

    if (attemptConnection < 0) { 
        printf("Append Server: Connection to verify server has failed. \n");
        exit(0);
    } else {
        printf("Append Server: Connection to verify server was successful. \n");
    }
}


void sendString() {
    int bufferLength = substring_Length * substring_Partitions;
    char sendBuf[bufferLength];
    strcpy(sendBuf, (*strObj).charArray);
    int sendMessage = send(outgoingSocket, sendBuf, sizeof(sendBuf) + 1, 0);
    if (sendMessage < 0)
        printf("Client: Error sending message to Append Server \n");
    else
        printf("Append Server: Send message '%s' to Verify \n", sendBuf);
}


//Remote version of "rpc_initappendserver_1" 
int *rpcinitappendserver_1_svc(AppendArguments *msg,  struct svc_req *svc)
{
    static int result = 1;       /* must be static! */

    
    property_Index = (*msg).property_Index;
    substring_Length = (*msg).substring_Length;
    substring_Partitions = (*msg).substring_Partitions;
    characterCount = substring_Length * substring_Partitions;

    int verify_host_name_length = sizeof( (*msg).verify_host_name );
    verify_host_name = (char*)malloc(sizeof(char) * verify_host_name_length);
    strcpy(verify_host_name, (*msg).verify_host_name);

    if (strObj == NULL) {
        strObj = malloc(sizeof(sstr));
        (*strObj).charIndex = 0;
        (*strObj).charArray = malloc(substring_Length * substring_Partitions + 1);
        (*strObj).charArray[substring_Length * substring_Partitions] = '\0';
    }

    printf("Server Append: Values have been initiated \n");
    return(&result);
}

int *rpcappend_1_svc(char *letter, struct svc_req *svc)
{
    static int result = -10; 

    if (characterCount != 0) {
        constructString(*letter);
        result = 0;
   } else {
        printf("Append Server: Constructed String: %s \n", (*strObj).charArray);
        createAppendServerSocket();
        connectVerifyServer(); 
        sendString();
        result = -1;
    }

    return (&result);
}