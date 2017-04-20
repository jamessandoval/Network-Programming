# File Client written in python
# By James Sandoval
# CS 372 Project #2
# 
# References: 
# https://docs.python.org/2/library/socket.html
# 


import socket  
import select  
import sys

HOST = ''

if __name__ == '__main__':

	if(len(sys.argv) < 5 or len(sys.argv) > 6 ) :
		print 'USAGE : python client.py <SERVER_ADDRESS> <SERVER_PORT> <COMMAND> <DATA_PORT>'
		print 'command usage:'
		print '<-l>  -- obtain directory contents from server'
		print '<-g FILENAME>) -- get file from server' 
		sys.exit(0)

	server_address = sys.argv[1]
	server_port = int(sys.argv[2])
	command = sys.argv[3]

	if(len(sys.argv) == 6) :
		print 'you\'ve entered the get command'
		filename = sys.argv[4]
		data_port = int(sys.argv[5])
		data_port_str = sys.argv[5]	


	else : 
		data_port = int(sys.argv[4])
		data_port_str = sys.argv[4]

	# Port Validation
	if(server_port < 1000 or server_port > 65535 ):
		print "invalid server port number, choose a valid port."
		sys.exit(0)

	if(data_port < 1000 or data_port > 65535 ):
		print "invalid server port number, choose a valid port."
		sys.exit(0)

	#Create the TCP Socket
	controlsock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	datasock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

	# Control Socket
	server_addr = (server_address, server_port)

	# Data Socket
	server_data_addr = (server_address, data_port)

	# Connect to control
	print >>sys.stderr, 'starting up on %s control socket: %s' % server_addr
	controlsock.connect(server_addr)

	# if length of command line arguments is 5 and user selected
	if (len(sys.argv) == 5) :

		message = command + ' ' + data_port_str + '\0'
		print >>sys.stderr, 'sent -l "%s"' % message
		controlsock.send(message)

		# Remember this is blocking
		data = controlsock.recv(512)

		# validate server response
		if(data == "Invalid command sent to server."):
			print(data)
			controlsock.close()
			datasock.close()
			sys.exit(0)

		print >>sys.stderr, 'opening data socket on: %s' % data_port_str
		datasock.connect(server_data_addr)

		data = datasock.recv(512);
		print(data)

		controlsock.close()
		datasock.close()
		sys.exit()

	if (len(sys.argv) == 6) :
		message = command +  ' ' + data_port_str + ' ' + filename + '\0'
		print >>sys.stderr, 'sent -g "%s"' % message
		controlsock.send(message)

		# Receive the file size and convert to an integer
		data = controlsock.recv(32)

		# validate server response
		if(data == "Invalid command sent to server."):
			print(data)
			controlsock.close()
			datasock.close()
			sys.exit(0)

		# if server sends invalid message: print and
		if(data == "Invalid File Name."):	
			print (data)
			controlsock.close()
			datasock.close()
			sys.exit(0)

		# If filename is valid, continue file operation.
		# and convert file size to an integer

		filesize = int(data)
		print("File name is valid. Preparing to receive.")

		#now attempt the data connection
		print >>sys.stderr, 'opening data socket on: %s' % data_port_str
		datasock.connect(server_data_addr)
		with open('%s' % filename, 'wb') as f:
			print 'file opened'
			while True:
				print('receiving data...')
				data = datasock.recv(filesize)
				#print('data=%s', (data))
				if not data:
					print('File receive failed...')
					break
				print('writing data...')
				f.write(data)
				f.close()
				break
			print >>sys.stderr, 'Successfully received file: saved as %s' % filename
			datasock.close()
			controlsock.close()

controlsock.close()
sys.exit(0)

