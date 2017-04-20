// File Server written in C
// By James Sandoval
// CS 372 Project #2
// 
// 
// References: 
// http://www.cplusplus.com/reference/
// http://www.cs.dartmouth.edu/~campbell/cs60/socketprogramming.html
// http://pubs.opengroup.org/onlinepubs/7908799/xsh/dirent.h.html

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <dirent.h>

#define BACKLOG 10

// Create a new socket, in our case 2: a control socket & data socket
int newSocket(int port);

// lists home directory
int directoryListing(int sockfd);

/*********************************
* Function Main
* Handles main functionality:
* command line arguments, from both
* server and client, as well as 
* handling clients: ie. directory request
* or file request.
*********************************/

int main(int argc , char *argv[]){

	// File handling variables:
	int lsize;
	char * buffer;
	char lsizechar[32];
	size_t result;
	FILE * fileToSend;

	// port assigned initially by server
	int port, dataport;
	int controlfd,datafd;// socket descriptors,
	// token -> converting string received to seperate variables
	// str -> string used for sending 
	// client_reply -> used for receiving on the socket
	char * token, * filename, str[1024], client_reply[1024];

	if (argc != 2){

		printf("%s", "USAGE : server <port>\n\n");
		return 0;

	}else{

		sscanf(argv[1], "%d", &port);

		// validate that the port is within the correct range 
		if(port < 1000 || port > 65535){

		printf("Invalid Port, choose a port number between 1000 and 65535.");
		return 0;

		}
	}


	// Delay for some time to let the kernel release the socket
	controlfd = newSocket(port);
	puts("Main server: Connected to client on control conection.");

		while(int sizeOfMessage = recv(controlfd, client_reply, 1024, 0) > 0){

			if(strncmp(client_reply, "-l ", 3) == 0){

				puts(client_reply);

				printf("%s","The client requested the directory structure.\n");
				printf("%s","Establishing Data Connection.......\n");

				// Everything between quotes is a character delimiter.  
				token = strtok(client_reply," ");
				int i = 0;

				while (token != NULL){

					if(i == 1){

						// no validation needed...handled on client side.
    					dataport = atoi(token);
    					printf ("The data port specificed from client is: %i \n", dataport);

    					printf("this should only be the port: %d\n",dataport);

    					}	
    			

		  				token = strtok (NULL, " ");
			    		i++;

		  				}

		  				strcpy(str, "File Directory requested...");
		  				send(controlfd, str, strlen(str), 0);

						datafd = newSocket(dataport);

						// Send the directory structure on datafd
						directoryListing(datafd);
						close(datafd);
						close(controlfd);

			}

			if(strncmp(client_reply, "-g ", 3) == 0){
  				printf("%s","The client requested a file.\n");
 
				// Everything between quotes is a character delimiter.  
				token = strtok(client_reply," ");
				int i = 0;

				// Obtain port and filename from client
				while (token != NULL){
					if(i == 1){

						// no validation needed...handled on client side.
    					dataport = atoi(token);
    					printf ("The data port specificed from client is: %i \n", dataport);

    					}	
    			
					if(i == 2){

	    				char * filename = token;
	    				printf("The file name specified from client is: %s \n", filename);

	    				// check to see if the file opens as specified by the client
	    				// if not, send message back to client that filename is incorrect.

	    				fileToSend = fopen(filename,"rb");

  						if (fileToSend != NULL){
    						//fclose (fileToSend);
    						puts("Filename is valid...preparing to send file.");

  						}else{

  							char invalidFile[32] = "Invalid File Name.";
  							send(controlfd, invalidFile, 18, 0);
  							puts("Invalid File Name.");
  							return 0;
  						}
  					}

  				token = strtok (NULL, " ");
	    		i++;

  				}

  				/************************
  				* File Handling operation
  				* Obtains file size, allocates memory,
  				* writes to the buffer,
  				* Sends size to client to receive
  				* and finally sends file.
  				*************************/

				 // obtain file size:
  				fseek (fileToSend, 0 , SEEK_END);
  				lsize = ftell (fileToSend);
  				rewind (fileToSend);

  				// Allocate memory to contain the whole file:
  				buffer = (char*) malloc (sizeof(char)*lsize); 
 				if (buffer == NULL){
 					fputs ("File Memory Allocation error",stderr); exit (2);
 				}

 				printf("The file size is %d\n", lsize);

 				sprintf(lsizechar,"%d", lsize);

 				// Send the file size to the client so it knows what to receive
 				send(controlfd, lsizechar, strlen(lsizechar), 0);

 				// copy the file into the buffer:
  				result = fread(buffer,1,lsize,fileToSend);
  				if (result != lsize) {
  					fputs ("Reading error",stderr); exit (3);
  				}

	  			printf("Establishing data connection on port: %i\n", dataport);
				datafd = newSocket(dataport);

				printf("Sending... \n");

   			         /* If read was success, send data. */
			         if(result > 0){
			            write(datafd, buffer, result);
			         }

					 if (result < lsize){
					    if (feof(fileToSend)){
					       printf("End of file\n");
					       break;
					  	}
						if (ferror(fileToSend)){
						     printf("Error reading\n");
						     break;
						}
					}

				puts("File was successfully sent.");
				close(datafd);
				close(controlfd);

			// Returs invalid command and sends message to client 
	    	}else{

				printf("%s","Invalid command received.");
				strcpy(str, "Invalid command sent to server. see usage");
				puts(client_reply);
				send(controlfd, str, strlen(str), 0);
				close(controlfd);
				
		}
	}

close(datafd);
close(controlfd);
return 0;
}
/**********************************
*
* Function new Socket
* parameters: int port specified by server or client
* Returns a connected socket int after accept is complete
* Primarily adapted from Beej's guide
*
***********************************/


int newSocket(int port) {

  struct sockaddr_in connection;
  struct sockaddr_storage their_addr;
  socklen_t addr_size;

  int sockfd;
  int yes = 1;

  //open socket for connection
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
    perror("Error Creating Socket");

  } 

  memset((char*)&connection, 0, sizeof(connection));

  connection.sin_family = AF_INET;
  connection.sin_port = htons(port);
  connection.sin_addr.s_addr = INADDR_ANY; 

  // Allows the port to be reused
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof(yes));

  //bind socket connection
  if (bind(sockfd, (struct sockaddr*)&connection, sizeof(connection)) == -1)
    perror("Bind failed");

  puts("Listening for connection.");

if(listen(sockfd, BACKLOG) == -1) {
 	perror("Error: listen");

	}

	addr_size = sizeof their_addr;
	if((sockfd = accept(sockfd,(struct sockaddr *)&their_addr, &addr_size)) == -1){

		perror("server: accept error");
	}

	printf("Connection Established on %i\n", port);

  return sockfd;
}

/**********************************
*
* Function Directory listing
* parameters: int socket to use 
* to send directory on returns 
* 1 if successful and 0 if not.
// http://pubs.opengroup.org/onlinepubs/7908799/xsh/dirent.h.html
***********************************/

int directoryListing(int sockfd){

	DIR *dir; // This is a data type representing a directory stream.
	struct dirent *ent; // This is a read directory structure
	char cwd[1024];  

	char *directorystring = (char *)malloc(sizeof(char) * 256);

	if (getcwd(cwd, sizeof(cwd)) == NULL){
   		 perror("current working directory.");

   	}else{

   		if ((dir = opendir(cwd)) != NULL){

   			while ((ent = readdir(dir)) != NULL){

   				strcat(directorystring, ent->d_name);
   				strcat(directorystring, "\n");

     		 }
     		 puts(directorystring);
     		 send(sockfd, directorystring, strlen(directorystring), 0);

     	 closedir(dir);

   		}else{
   			perror("Directory Unavalaible");
   			return 0;
   		}
   	}
   	return 1;

}







