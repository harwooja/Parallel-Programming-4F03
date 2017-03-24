/*
This is the client file that accepts all command-line interface parameters from pa4.c.

This file (client.c) speaks to using SOCKET and RPC:
1. server_append.c (Initially sends parameters: )
2. server_verify.c
*/


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <rpc/rpc.h>
#include <omp.h>

#include "client.h"
#include "server_append_rpc.h"
#include "server_verify_rpc.h"

#define MAX_NUM_ARGUMENTS 10
#define BUFLEN 2076

// RPC Specific Variables
CLIENT *cla, *clv;
int *resultA, *resultV;
char *server;

int characterCount = 0;

int main(int argc, char* argv[]) {

    if (argc != MAX_NUM_ARGUMENTS) {
        printf("Invalid number of arguments");
        exit(1);
    }
    
    /*Get parameters from command line */
    property_Index = strtol(argv[1], NULL, 10);
    thread_count = strtol(argv[2], NULL, 10);
    substring_Length = strtol(argv[3], NULL, 10);
    substring_Partitions = strtol(argv[4], NULL, 10);
    c0 = (*argv[5]);
    c1 = (*argv[6]);
    c2 = (*argv[7]);

    characterCount = substring_Length * substring_Partitions;
    resultA = malloc(sizeof(int));
    *resultA = INT_MAX;


    /* Get append and verify host names from command line argv*/
    int append_host_name_length = strlen(argv[8]);
    append_host_name = (char*)malloc(sizeof(char) * append_host_name_length);
    strcpy(append_host_name, argv[8]);

    int verify_host_name_length = strlen(argv[9]);
    verify_host_name = (char*)malloc(sizeof(char) * verify_host_name_length);
    strcpy(verify_host_name, argv[9]);

    runClient();

    free(append_host_name);
    free(verify_host_name);

    return 0;
}



void runClient() {   

    enum SERVER_TYPE value;
    // Initiate RPC instance (verify)
    value = VERIFY;
    createRPC(verify_host_name, value);
    rpcVerifyServer();
    // Initiate RPC instance (append)
    value = APPEND;
    createRPC(append_host_name, value);
    rpcAppendServer();
  
    // Generate threadss
    #pragma omp parallel num_threads(thread_count)
    {
        sendCharsToAppendServer();
        //getSegmentFromVerifyServer();
    }

    printf("Client has ended gg \n");

    // Destroy RPC connection and client Socket
    clnt_destroy(cla);
    clnt_destroy(clv);
}



// Send chars over to append server
void sendCharsToAppendServer() {

        /* DP:
        // NO OMP CRITICAL!
        while (*resultA != -1) {
            usleep()
            rpcappend_1()
        }
        */

        LOOP: usleep(RandomBetween(0.1, 0.5));
        
        printf("Result A : %d \n", *resultA);

        if (*resultA != 1) {        
            int my_rank = omp_get_thread_num();
            char assignedChar = 'a' + my_rank; 
            resultA = rpcappend_1(&assignedChar, cla);
            // An error occurred while calling the server. Print error message and die
            if (resultA == NULL) {
                clnt_perror(cla, "Client: Error sending char value \n");
                exit(1);
            }  
            goto LOOP;
        }
}


void getSegmentFromVerifyServer() {

    /* DP:
        while (segsment != "-") {
            segment = rpcgetseg_1()
            // It's possible that the segment isn't ready at the verify server yet, so handle the case here.
            if (segment == "not ready") {
                continue;
            }

            // Do work on segment
            verify segment
            increment number of valid segments
        }

        return number of valid segments so openmp can do a summation reduction on it
    */

}


float RandomBetween(float smallNumber, float bigNumber)
{
    float diff = bigNumber - smallNumber;
    return (((float) rand() / RAND_MAX) * diff) + smallNumber;
}

/* RPC Related Functions*/ 

void createRPC(char* serverHost, enum SERVER_TYPE value) {
    /* Creare client handle for calling MESSAGEPROG on the server
     * designated on the command line.  We tell the RPC package
     * to use the tcp protocol when contacting the server.
     */   

    if (value == APPEND) {
        cla = clnt_create(serverHost, SERVERAPPENDPROG, SERVERAPPENDVERS, "udp");
        if (cla == NULL) {
            /*
            * Couldn't establish connection with server.
            * Print error message and die.
            */
            clnt_pcreateerror(serverHost);
            exit(1);
        } 
    } else if (value == VERIFY) {
        clv = clnt_create(serverHost, SERVERVERIFYPROG, SERVERVERIFYVERS, "udp");
        if (clv == NULL) {
            /*
            * Couldn't establish connection with server.
            * Print error message and die.
            */
            clnt_pcreateerror(serverHost);
            exit(1);
        } 
    } else {
        printf("Client: ENUM Server_Type value is incorrect -- Exiting \n");
        exit(1);
    }
    printf("RPC Successfully connected with server: %s \n", serverHost);
}

void rpcAppendServer() {
    // Call remote procedure in append server
    
    AppendArguments appendArgs;
    appendArgs.property_Index = property_Index;
    appendArgs.substring_Length = substring_Length;
    appendArgs.substring_Partitions = substring_Partitions;
    appendArgs.verify_host_name = verify_host_name;
    
    resultA = rpcinitappendserver_1(&appendArgs, cla);
    
    // An error occurred while calling the server. Print error message and die
    if (resultA == NULL) {
        clnt_perror(cla, "Client: Error sending init value to Append Server \n");
        exit(1);
    }
    // Successfully called remote procedure
    if (*resultA == 0) {
        //Server was unable to print our message.  Print error message and die.
        printf("Client: Append Server was unable to print our init message \n");
        exit(1);
    }
    // Message delivered to server
     printf("Message delivered to Append Server! \n");
}

void rpcVerifyServer() {
        // Call remote procedure in append server
    
    VerifyArguments verifyArgs;
    verifyArgs.thread_count = thread_count;
    verifyArgs.substring_Length = substring_Length;
    verifyArgs.substring_Partitions = substring_Partitions;
    
    resultV = rpcinitverifyserver_1(&verifyArgs, clv);
    
    // An error occurred while calling the server. Print error message and die
    if (resultV == NULL) {
        clnt_perror(clv, "Client: Error sending init value to Verify Server \n");
        exit(1);
    }
    // Successfully called remote procedure
    if (*resultV == 0) {
        //Server was unable to print our message.  Print error message and die.
        printf("Client: Verify Server was unable to print our init message \n");
        exit(1);
    }
    // Message delivered to server
     printf("Message delivered to Verify Server! \n!");
}



