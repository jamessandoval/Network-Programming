/**********************************
*
*    James Sandoval
*    08NOV2016 
*    Project #1 - CS372 Introduction to Networking
*    References: Beej's Guide to Networking Programming
*
*         
***********************************/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define MAXDATASIZE 4096
#define MAXUSERNAMESIZE 10

/************************
* Function to Verify valid Ip address
* returns 0 if not valid, 1 if valid, 
* and 3 if localhost is the string.
* Parameters: command line arg for ip address
* inputted by user and the structure for ipv 
* addresses. Returns an integer 0 if not valid
* Returns 1 if valid, and returns 3 if the user 
* inputted localhost as hostname
*************************/

int VerifyIpAddress(char *ipAddress, struct sockaddr_in sa)
{
	if(strncmp(ipAddress, "localhost", 9) == 0){
		return 3;
	}

    return inet_pton(AF_INET, ipAddress, &(sa.sin_addr));
} 

/************************
* Function to Handle Messages
* passes the socket file descriptor
* as a parameter, evaluates if the user
* has quit, otherwise prints recv message.
*************************/

int recv_msg(int sockfd){

	char client_reply[MAXDATASIZE];
	char quitString[12] = "Server>quit";

	if(recv(sockfd, client_reply, MAXDATASIZE, 0) < 0){

	    puts("recv failed");
	    return 0;
	       		
	    // if server sends quit then close the socket and return to command line
   	}else if(strcmp(client_reply, quitString) == 0){

   		puts("Server is offline.");
   		printf("%s", "Goodbye.\n");
   		return 0;	

   	}else{

   		puts(client_reply);
   		return 1;
   	}

   	return 1;
}

/************************
* Function to Handle Sending
* passes the socket file descriptor and
* the username as a parameter, evaluates if the user
* server has quit, otherwise prints and sends server
* message to a connected host.
*************************/

int send_msg(int sockfd, char *username){

	char server_reply[MAXDATASIZE];
	/*Read chars from console into 'str' */
	printf("%s>",username);
   	fgets(server_reply,MAXDATASIZE, stdin);

   	if(strncmp(server_reply, "quit", 4) == 0){

   		send(sockfd, server_reply, strlen(server_reply), 0);
   		printf("%s", "Goodbye.\n");

   		return 0;
   	}

  	send(sockfd, server_reply, strlen(server_reply), 0);
  	return 1;

}

/************************
* Main Function to determine if command line arguments
* are present and validates ip address.
* Handles the creation of a socket, connects to the server
* and obtains 10 character username
*************************/


int main(int argc , char *argv[]){

	int port;
	int sockfd; 
	struct sockaddr_in server;

	char username[MAXUSERNAMESIZE], server_reply[MAXDATASIZE];
	char hostname[128];

	if (argc != 3){

		printf("%s", "USAGE : <HOSTNAME> ./chat port\n\n");
		return 0;
	}

	// Verify Ip Address & convert hostname to 127.0.0.1 if localhost
	if(VerifyIpAddress(argv[1], server) == 0){
		printf("%s", "\nInvalid IP Address\n");
		return 0;

	}else if(VerifyIpAddress(argv[1], server) == 3){

		strcpy(hostname, "127.0.0.1");

	}else{

		sscanf(argv[1], "%s", hostname);
	}

	// convert user input 
	sscanf(argv[2], "%d", &port);
	
	// Define Socket Descriptor
    sockfd = socket(AF_INET , SOCK_STREAM , 0); 
 
    if (sockfd < 0){
        puts("Socket has failed.");
        return 0;
    }

    server.sin_addr.s_addr = inet_addr(hostname);    
    server.sin_family = AF_INET;                     // TCP 
    server.sin_port = htons(port);                   // Specify the port server resides on

    printf("Attempting to connect on %s at %s\n", argv[1], argv[2]);

	if(connect(sockfd, (struct sockaddr *)&server , sizeof(server)) < 0 ){
		puts("connection error.");
	 	printf("*** Attempted *** to connect on %s at %s\n", argv[1], argv[2]);
	 	return 1;
	}

	 /// Handshake Phase 

	 puts("connected to server.\n");
	 printf("Please input a %d character username:\n", MAXUSERNAMESIZE);
	 printf("%s", "input:");

	 fgets(username, MAXUSERNAMESIZE, stdin);

	 while(strlen(username) > MAXUSERNAMESIZE){

	   	printf("You are limited to %d characters. try again.\n", MAXUSERNAMESIZE);
	    printf("%s", "input: ");
	    fgets(username, MAXUSERNAMESIZE, stdin);
	   
	}
	    
	// Remove newline from string and send username to the server
	strtok(username, "\n"); 
	send(sockfd, username, strlen(username), 0);
	recv(sockfd, server_reply, MAXDATASIZE, 0);
	puts(server_reply);

	// Handshake Phase Ends and normal chat dialogue begins

	// Loop to maintain chat Dialogue
	 while(1){

	  	//Receive a reply from the server
	  	if(send_msg(sockfd, username) == 0){

	  		return 0;
	  	}
	 	recv_msg(sockfd);
    	
	}
    return 0;
}

