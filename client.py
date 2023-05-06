import socket
import json
import threading

def listen(sock):
    while True:
        out = sock.recv(4000).decode("utf-8")
        if out:
            print(out)
    

tests = '[{"input": "123", "output": "123"}, {"input": "123", "output": "123"}]'
clientsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
clientsocket.connect(('localhost', 1721))
clientsocket.send(tests.encode("utf-8"))
threading.Thread(target=lambda: listen(clientsocket)).start()
