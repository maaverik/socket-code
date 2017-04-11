from socket import *
from select import *
import sys

# Use these for binding our server
HOST = "127.0.0.1"
try:
    PORT = int(sys.argv[1])
    AUTH = int(sys.argv[2])
except:
    print("Enter server port and authorizer port as cmdline arg")
    sys.exit()


server = socket(AF_INET, SOCK_STREAM)

server.bind((HOST, PORT))

server.listen(5)
print("Listening...")

auth = socket(AF_INET, SOCK_STREAM)
auth.connect((HOST, AUTH))

clients = []
dnsDict = {"google.com": "172.217.26.174", "nitc.ac.in": "124.124.70.4", "facebook.com": "157.240.7.35"}

# Create a list of clients to use on select
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

                print("Received key from client, sending to authenticator for response")
                auth.send(str.encode(data))
                data = bytes.decode(auth.recv(1024))

                print("Sending response to client")
                client.send(str.encode(data))


                print("Receiving data from client...")
                data = client.recv(1024)
                data = bytes.decode(data)

                # Print the data so the server gets to see it
                print("Received url: " + data)
                if(data == 0 or len(data) == 0):
                    print("Client removed")
                    for c in clients:
                        if c[0] == client:
                            clients.remove(c)
                            break
                else:
                    # Get response
                    if "www." in data:
                        data = data[4:]
                    if data in dnsDict:
                        print("Getting required data from cache...")
                        print(dnsDict[data])
                        client.send(str.encode(dnsDict[data]))
                    else:
                        try:
                            dnsDict[data] = gethostbyname(data)
                            print("Looking up address...")
                            print(dnsDict[data])
                            print("Address added to cache")
                            client.send(str.encode(dnsDict[data]))

                        except:
                            print("Cannot resolve hostname")
                            client.send(str.encode("Cannot resolve hostname"))
    except:
        pass

