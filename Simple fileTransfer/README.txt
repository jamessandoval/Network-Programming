James Sandoval
CS 372
Fall 2016
Project 2: FILE SERVER Server & Client

+++++++++++++
Instructions 
+++++++++++++

To test both the file server(written in c) and file client(written in python), 
you must first open the file server(after using the command make) in an independent terminal window, and separately run the file client in another terminal window.


1. Open a new Terminal window and run the following in the directory where fileserver.c is located:

   $make
 
   $fileserver <port number>

- file server will print “listening” in the terminal window and is waiting for connection.

2. Open a separate Terminal window and run the following:

  $ python fileclient.py <serverport> <command> <FILENAME> <dataport>

+++++++++++++
Usage
+++++++++++++

server:
 $fileserver <port number>

client:
  $ client <hostname> <port number>


Server takes a port number to listen for connections from
client on the localhost machine (127.0.0.1).

Once connected, file server evaluates whether the command line arguments are correct,
if -l the file server sends directory contents to the client, if -g is sent along with a valid filename, the file is sent. If the server receives an invalid command the program exits to the command line, and after the command has been executed the program exits to the command line.

+++++++++++++
Testing
+++++++++++++

Environments tested:

127.0.0.1 (MacOSX.platform/Developer/SDKs/MacOSX10.10.sdk/usr/include/c++/4.2.1) (CLANG)
flip1.access.engr.oregonstate.edu  (gcc version 4.8.5 20150623 (Red Hat 4.8.5-4)(GCC))

+++++++++++++
Resources
+++++++++++++

https://docs.python.org/2/library/socket.html
http://www.cplusplus.com/reference/
http://www.cs.dartmouth.edu/~campbell/cs60/socketprogramming.html
http://pubs.opengroup.org/onlinepubs/7908799/xsh/dirent.h.html

