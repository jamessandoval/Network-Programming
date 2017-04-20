#/**********************************
#*
#*    James Sandoval
#*    08NOV2016 
#*    Project #1 - CS372 Introduction to Networking
#*    References: Python Software Foundation Reference:
#*    https://docs.python.org/2/howto/sockets.html
#*    https://pymotw.com/2/socket/tcp.html
#*         
#***********************************/

import socket  
import sys     

HOST = ''             # all available interfaces...ie localhost/127.0.0.1
RECV_BUFFER = 4096    # 

# 
# Function sendMsg
# Handles send on the connection
# Parameters: the connection
# 

def sendMsg(connection):

		reply = raw_input('Server>')

		#Check to see if the user has quit
		if reply == "quit":

			reply = "Server>" + reply + "\0"

			connection.send(reply)

			print "Connection closed.\n\nGoodbye."
			connection.close()
			sys.exit(0)
			
		reply = "Server>" + reply + "\0"
		connection.send(reply)

# 
# Function recvMsg
# Handles receive on the connection
# Parameters: the Socket, the connection, and 
# username handle
# 


def recvMsg(connection, server_socket, username):

		#Check the receive buffer
		data = connection.recv(RECV_BUFFER)

		if data == "quit\n":
			print("Client has quit.\nwaiting for another client...")
			connection.close()
			clientChat(server_socket)
				
		data = username + data

		print(data.rstrip())

# 
# Function clientChat 
# Listens on the socket for Connections
# Handles Initial Handshake to obtain user name
# handles send/receive processes during connection
# Parameters: the Socket
# 


def clientChat(server_socket):

	print ("Listening on port: " + str(port)) 
	server_socket.listen(1)                                                 
	#accept connection and print the address

	connection, address = server_socket.accept()
	print "Connected to " + address[0] + ':' + str(address[1])

	handshake = 0 

	print "chat server is running on localhost via port: " + str(port) # 
			
	while(1):	
		
		if handshake == 0:
			#first connect, get username
			username = connection.recv(RECV_BUFFER)
			print ("The clients user name is " + username)
			print ("\nWaiting for client response...") 
			connection.send("Your user name is " + username + "\nChat away!\n")
			# Remove the space after the string and concatenate
			username = username.rstrip() + ">"
			# flag is set to show chat is initiated
			handshake = 1

		recvMsg(connection,server_socket,username)

		sendMsg(connection)

# 
# Main Function
# Checks to ensure command line arguments are in place
# defines the tcp connection, binds the socket, then calls
# the clientChat function to start listening for a connection
#
			

if __name__ == '__main__':

	if(len(sys.argv) < 2) :
		print 'USAGE : python chat_client.py port'
		sys.exit(0)

	port = int(sys.argv[1])

	server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)       # Tcp Connection 	
	server_socket.bind((HOST, port))                                        # Bind the Socket to the host:port combination

	print ("\nChat server is running...")

	clientChat(server_socket)


	

