import socket
import threading

port = 9001
size = 1024

# Dictionary to hold the mapping of client IDs to their socket connections
clients = {}

def listen():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.bind(('', port))
    sock.listen(5)
    print('Listening...')
    while True:
        client, address = sock.accept()
        client.settimeout(20)
        # Start a thread for handling the client connection
        threading.Thread(target=clientHandler, args=(client, address)).start()

def clientHandler(client, address):
    print(f'Got connection from {address[0]}')
    # Expecting the first message to be the client ID
    c_id = client.recv(size).decode()
    print(f'ID: {c_id}')
    # Store the client connection in the clients dictionary
    clients[c_id] = client
    while True:
        try:
            data = client.recv(size).decode()
            if not data:
                break  # connection was closed by the client
            print(data)
            # Extract receiver ID from the message
            receiver_id = data[:2]
            # Forward the message to the intended receiver
            if receiver_id in clients:
                clients[receiver_id].sendall(data[2:].encode())  # Send the message without the ID
            else:
                print(f"Receiver ID {receiver_id} not found.")

        except Exception as e:
            print(e)
            client.close()
            # Remove the client from the dictionary when disconnecting
            if c_id in clients:
                del clients[c_id]
            return False

if __name__ == '__main__':
    listen()
