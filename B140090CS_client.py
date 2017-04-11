from socket import *
from select import *
import sys

# Information of our server to connect to
HOST = "127.0.0.1"
try:
    PORT = int(sys.argv[1])
    AUTH = int(sys.argv[2])
except:
    print("Enter port and authorizer port as cmdline arg")
    sys.exit()

sock = socket(AF_INET, SOCK_STREAM)
sock.connect((HOST, PORT))
auth = socket(AF_INET, SOCK_STREAM)
auth.connect((HOST, AUTH))

key = input("Enter a keyword: ")
print("Sending key to authorizer for authentication:", key)
auth.send(str.encode(key))
print("Sent")
response = bytes.decode(auth.recv(1024))
print("Got response from authenticator : " + response)

# Client loop
while(True):
	# Let the user enter some data to send
	data = input("Enter url: ")
	read, write, error = select([],[sock],[],0)
	if(len(write)):
		print("Sending key to server for authentication:", key)
		b = sock.send(str.encode(key))
		recvdresp = bytes.decode(sock.recv(1024))
		if response != recvdresp:
			print("Server not authenticated...exiting")
			sys.exit(0)
		else:
			print("Server authenticated")
		# Send the data to the server
		b = sock.send(str.encode(data))

    # The receiving loop
	while(True):
		# When receiving, use the timeout of 1 to receive more data
		read, write, error = select([sock],[],[],1)

		# If there is data, print it
		if(len(read)):
			data = bytes.decode(sock.recv(1024))
			print(data)
		# Exit the loop if no more data
		else:
			break
