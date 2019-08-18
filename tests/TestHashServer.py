import socket
import random
import string
import sys

def randomString(stringLength=10):
    letters = string.ascii_lowercase
    return ''.join(random.choice(letters) for i in range(stringLength))


messageNum = int(sys.argv[1])
counter = 0

while(counter < messageNum):
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect(('localhost', 10000))

    msg_len = random.randint(1, 4096)
    msg = randomString(msg_len) + '\n'
    client.send(msg.encode('ascii'))
    response = client.recv(4096)
    print(response)




