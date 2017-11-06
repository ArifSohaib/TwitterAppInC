#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), sendto(), and recvfrom() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */

#define ECHOMAX 151     /* Longest string to echo */

void DieWithError(char *errorMessage);  /* External error handling function */
typedef struct {

enum {Login, Follow, Post, Request, Search, Delete, Unfollow, Logout }

request_Type;                                    /* same size as an unsigned int */

unsigned int UserID;                                      /* unique client identifier */

unsigned int LeaderID;                                   /* unique client identifier */

char message[140];      }ClientMessage ;
int main(int argc, char *argv[])
{
    int sock;                        /* Socket descriptor */
    struct sockaddr_in echoServAddr; /* Echo server address */
    struct sockaddr_in fromAddr;     /* Source address of echo */
    unsigned short echoServPort;     /* Echo server port */
    unsigned int fromSize;           /* In-out of address size for recvfrom() */
    char *servIP;                    /* IP address of server */
    char *echoString;                /* String to send to echo server */
    char echoBuffer[ECHOMAX+1];      /* Buffer for receiving echoed string */
    ClientMessage echoMsg;
    int echoStringLen;               /* Length of string to echo */
    int respStringLen;               /* Length of received response */
    char requestType;
    char UserID[5];
    char LeaderID[5];

    if ((argc < 6) || (argc > 7))    /* Test for correct number of arguments */
    {
        fprintf(stderr,"Usage: %s <Server IP> <Msg> <RequestType> [<Echo Port>]\n", argv[0]);
        exit(1);
    }


    servIP = argv[1];           /* First arg: server IP address (dotted quad) */
    echoString = argv[2];       /* Second arg: string to echo */
    requestType = argv[3][0];      /* Third arg: requestType */
    strcpy(UserID ,argv[4]);

    strcpy(LeaderID , argv[5]);

    printf("echoString %s\n", argv[2]);
    int msg_size = sizeof(argv[2]);
    char msg[140];
    strcpy(msg, argv[2]);
    printf("msg = %s\n", msg);
    if ((echoStringLen = strlen(echoString)) > ECHOMAX)  /* Check input length */
        DieWithError("Echo word too long");
    if(strlen(UserID) != 5){
      DieWithError("UserID must be 5 digits");
    }
    if(strlen(LeaderID) != 5){
      DieWithError("LeaderID must be 5 digits");
    }
    echoStringLen = 152;
    strcpy(echoString, "");

    if (argc == 7)
        echoServPort = atoi(argv[6]);  /* Use given port, if any */
    else
        echoServPort = 7;  /* 7 is the well-known port for the echo service */
    printf("sending to id %s\n", servIP);
    printf("sending to port %i\n", echoServPort);
    /* Create a datagram/UDP socket */
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        DieWithError("socket() failed");

    /*Construct the message*/
    //clear the message string
    memset(echoString, 0, 152); //size of string to convert
    printf("memsetdone");
    //first set a test request_Type
    // strcpy(echoString, "4");
    // strcpy(echoString, (char)requestType);
    echoString[0] = requestType;
    //then add a 5 digit userID
    printf("UserID %s", UserID);
    strcpy(echoString+1, UserID);
    //then a 5 digit leaderID
    strcpy(echoString+6, LeaderID);
    //then the message
    printf("argv[2] %s\n", msg);
    strcpy(echoString+11,msg);
    printf("echoString+11 =  %s\n", (echoString+11));
    memset(&echoMsg, 0, sizeof(echoMsg));
    // strcpy(echoMsg.message,argv[2]);
    //convert the echoString to the structure
    char * substr[140];
    memset(*substr,0, 140 );
    echoMsg.request_Type = atoi(strncpy(*substr, (echoString), 1));
    memset(*substr,0, 140 );
    echoMsg.UserID = atoi(strncpy(*substr, (echoString+1), 5));
    memset(*substr,0, 140 );
    echoMsg.LeaderID = atoi(strncpy(*substr, (echoString+6), 5));
    printf("testing echoMsg.UserID %d\n", echoMsg.UserID);
    printf("testing echoMsg.request_Type %d\n", echoMsg.request_Type);
    printf("testing echoMsg.LeaderID %d\n", echoMsg.LeaderID);
    strncpy(echoMsg.message, (echoString+11), sizeof(argv[2]));
    printf("size of message = %lu\n",sizeof(argv[2]));
    printf("echo string: %s\n", echoString);
    printf("testing echoMsg.message %s\n", echoMsg.message);
    /* Construct the server address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));    /* Zero out structure */
    echoServAddr.sin_family = AF_INET;                 /* Internet addr family */
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);  /* Server IP address */
    echoServAddr.sin_port   = htons(echoServPort);     /* Server port */

    /* Send the string to the server */
    if (sendto(sock, echoString, echoStringLen, 0, (struct sockaddr *)
               &echoServAddr, sizeof(echoServAddr)) != echoStringLen)
        DieWithError("sendto() sent a different number of bytes than expected");

    /* Recv a response */
    fromSize = sizeof(fromAddr);
    if ((respStringLen = recvfrom(sock, echoString, ECHOMAX, 0,
         (struct sockaddr *) &fromAddr, &fromSize)) != echoStringLen)
        DieWithError("recvfrom() failed");

    if (echoServAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr)
    {
        fprintf(stderr,"Error: received a packet from unknown source.\n");
        exit(1);
    }

    /* null-terminate the received data */
    // echoMsg[respStringLen] = '\0';
    printf("Received: %s\n", echoString);    /* Print the echoed arg */
    close(sock);
    exit(0);
}
