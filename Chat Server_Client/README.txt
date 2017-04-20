James Sandoval
CS 372
Fall 2016
Project 1: Chat Server & Client

+++++++++++++
Instructions 
+++++++++++++

To test both the chat client(written in c) and chat server(written in python), 
you must first open the chat server in an independent window, and seperately make 
and run the chat client (written in c).

1. Open a new Terminal window and run the following in the directory where server.py is located:

$python server.py <port>

the python server will run on the localmachine and port specified. It will listen until a client connects.

2. Open a seperate Terminal window and run the make command where the client.c, and makefile are located:

  $ make

+++++++++++++
Usage
+++++++++++++

server:
  $ python server.py <port>

client:
  $ client <hostname> <port number>


Server takes a port number to listen for connections from
Client on the localhost machine (127.0.0.1).

Once connected, Client and Server initiate a handshake phase
where the client must enter a username less than 10 characters.
Once a username is established the server then waits for the client to 
send a message. After the client sends a message the server must respond
before the client may reply. 

If server enters the command:

 $ quit

Both programs will exit to the command line. If client quits the server
will close the connection and listen on the socket for another connection.

Both Client and Server can send messages exceeding 500 characters.

+++++++++++++
Testing
+++++++++++++

Environments tested:

127.0.0.1 (MacOSX.platform/Developer/SDKs/MacOSX10.10.sdk/usr/include/c++/4.2.1) (CLANG)
flip1.access.engr.oregonstate.edu  (gcc version 4.8.5 20150623 (Red Hat 4.8.5-4)(GCC))

+++++++++++++
Resources
+++++++++++++

http://beej.us/guide/bgnet/
http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html
https://docs.python.org/2/howto/sockets.html
https://pymotw.com/2/socket/tcp.html

