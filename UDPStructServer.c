#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket() and bind() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */

#define ECHOMAX 255     /* Longest string to echo */

void DieWithError(char *errorMessage);  /* External error handling function */

typedef struct {

enum {Login, Follow, Post, Request, Search, Delete, Unfollow, Logout }

request_Type;                                    /* same size as an unsigned int */

unsigned int UserID;                                      /* unique client identifier */

unsigned int LeaderID;                                   /* unique client identifier */

char message[140];      }ClientMessage ;                                 /* text message */

typedef struct{
  int UserID;
  int LeaderID;
} Follow_Pair;

int main(int argc, char *argv[])
{
    int sock;                        /* Socket */
    struct sockaddr_in echoServAddr; /* Local address */
    struct sockaddr_in echoClntAddr; /* Client address */
    unsigned int cliAddrLen;         /* Length of incoming message */
    char echoBuffer[ECHOMAX];        /* Buffer for echo string */
    ClientMessage echoMsg;
    Follow_Pair followList[400];         /* List to store leader-follower pairs*/
    int follow_count = 0;
    char echoString[ECHOMAX];

    int users[200];
    int num_users = 0;

    unsigned short echoServPort;     /* Server port */
    int recvMsgSize;                 /* Size of received message */

    if (argc != 2)         /* Test for correct number of parameters */
    {
        fprintf(stderr,"Usage:  %s <UDP SERVER PORT>\n", argv[0]);
        exit(1);
    }

    echoServPort = atoi(argv[1]);  /* First arg:  local port */

    /* Create socket for sending/receiving datagrams */
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        DieWithError("socket() failed");
    /* Construct the message to struct */
    // memset(&echoMsg, 0, sizeof(echoMsg));
    /* Construct local address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));   /* Zero out structure */
    echoServAddr.sin_family = AF_INET;                /* Internet address family */
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    echoServAddr.sin_port = htons(echoServPort);      /* Local port */

    /* Bind to the local address */
    if (bind(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        DieWithError("bind() failed");
    char substr[140];
    for (;;) /* Run forever */
    {
        memset(&echoMsg, 0, 151);

        // strcpy(substr,"empty");
        /* Set the size of the in-out parameter */
        cliAddrLen = sizeof(echoClntAddr);

        /* Block until receive message from a client */
        if ((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr *) &echoClntAddr, &cliAddrLen)) < 0){
            DieWithError("recvfrom() failed");
          }
        // strncpy(echoString, echoBuffer, sizeof(echoString));
        printf("echoBuffer initial %s\n", echoBuffer);

        printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));
        // strcpy(echoString, echoBuffer);

        echoMsg.request_Type = atoi(strncpy(substr, (echoBuffer), 1));
        memset(&substr,0, 140 );
        echoMsg.UserID = atoi(strncpy(substr, (echoBuffer+1), 5));
        memset(&substr,0, 140 );
        echoMsg.LeaderID = atoi(strncpy(substr, (echoBuffer+6), 5));
        memset(&substr,0, 140 );

        printf("testing echoMsg.UserID %d\n", echoMsg.UserID);
        printf("testing echoMsg.request_Type %d\n", echoMsg.request_Type);
        printf("testing echoMsg.LeaderID %d\n", echoMsg.LeaderID);
        strncpy(echoMsg.message, (echoBuffer+11), sizeof(argv[2]));
        printf("size of message = %lu\n",sizeof(argv[2]));
        printf("echo string: %s\n", echoBuffer);
        printf("testing echoMsg.message %s\n", echoMsg.message);
        //clear the echo buffer;
        // memset(echoBuffer, 0, ECHOMAX);
        int idx;
        /* Send received datagram back to the client */
        if (sendto(sock, echoMsg.message, recvMsgSize, 0,
                   (struct sockaddr *) &echoClntAddr, sizeof(echoClntAddr)) != recvMsgSize)
            DieWithError("sendto() sent a different number of bytes than expected");

        switch (echoMsg.request_Type) {
          case 1:
          printf("handling login request\n");
          //handle login request_Type
          users[num_users] = echoMsg.UserID;
          if (num_users < 200){
            num_users += 1;
            for(idx = 0; idx < num_users; idx++){
              printf("user[%i]  = %i\n", idx, users[idx]);
            }
            printf("\n");

          }
          else{
            DieWithError("Reached max users\n");
          }

          break;

          case 2:

          printf("handling follow request\n");
          Follow_Pair pair;
          memset(&pair, 0, sizeof(pair));
          //set new pair
          pair.UserID = echoMsg.UserID;
          pair.LeaderID = echoMsg.LeaderID;

          //handle follow request
          if(follow_count < 400){
            followList[follow_count] = pair;
            follow_count += 1;
            for(idx = 0; idx < num_users; idx++){
              printf("user[%i]  = %i, %i\n", idx, followList[idx].UserID, followList[idx].LeaderID);
            }
            printf("\n");
          }
          break;

          case 3:

          printf("handling post request\n");
          //handle post msg request
          //open file

          //write to file
          //close file
          break;


          case 4:
          printf("handling get request\n");
            break;


          //recieve all messages from loaderID being followed
          case 5:
          printf("handling search request\n");
          break;

          case 6:
          printf("handling delete request\n");
          break;

          case 7:
          printf("handling unfollow request\n");
          break;

          case 8:
          printf("handling logout request\n");

          break;
          //delete a posted message
          default:
          //send invalid request error
          printf("invalid request\n");
        }
      }

    }
