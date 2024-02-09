import socket
import threading

port = 9001
size = 1024

def listen():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.bind(('', port))
    sock.listen(5)
    print('Listinging...')
    while True:
        client, adress = sock.accept()
        client.settimeout(20)
        threading.Thread(target=clientHandler, args=(client, adress)).start()

def clientHandler(client, adress):
    print(f'Got connection from {adress[0]}')
    c_id = client.recv(size).decode()
    print(f'ID: {c_id}')
    while True:
        try:
            data = client.recv(size).decode()
            print(data)

        except Exception as e:
            print(e)
            client.close()
            return False

if __name__ == '__main__':
    listen()