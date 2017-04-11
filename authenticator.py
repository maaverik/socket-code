from socket import *
from select import *
import sys

# Information of our server to connect to
HOST = "127.0.0.1"
try:
    PORT = int(sys.argv[1])
    n = int(sys.argv[2])
except:
    print("Enter port and integer for encoding as cmdline arg")
    sys.exit()

server = socket(AF_INET, SOCK_STREAM)

server.bind((HOST, PORT))

server.listen(5)
print("Listening...")

clients = []
def getClients():
    to_use = []
    for client in clients:
        to_use.append(client[0])
    return to_use

print("Waiting for connections...")

# Server loop
while(True):
    # Check for connection but not block
    read, write, error = select([server],[],[],0)


    # Check if any connections
    if(len(read)):
        client, address = server.accept()
        clients.append([client, address, []])

    to_use = getClients()

    # Try and read any data from the clients
    try:
        read, write,error = select(to_use,[],[],0)
        if(len(read)):
            for client in read:
                # Get the data from the client
                data = client.recv(1024)
                data = bytes.decode(data)

                client.send(str.encode(data[n:] + data[:n]))
    except:
        pass
