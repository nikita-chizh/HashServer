import socket
import random
import string
import sys
from hashlib import sha256, sha1

def randomString(stringLength=10):
    letters = string.ascii_letters
    return ''.join(random.choice(letters) for i in range(stringLength))


messageNum = int(sys.argv[1])
port = int(sys.argv[2])
counter = 0
# min_size = 2096
# max_size = 4096

min_size = int(sys.argv[3])
max_size = int(sys.argv[4])


sha_func = sha256()
while(counter < messageNum):
    print(counter)
    msg_len = random.randint(min_size, min_size)
    ascii_msg = (randomString(msg_len)).encode('ascii')
    sha_func.update(ascii_msg)
    expected_resp = sha_func.hexdigest().upper()
    #
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect(('localhost', port))
    client.send(ascii_msg + '\n'.encode('ascii'))
    response = client.recv(4096)
    str_response = str(response[:-1], 'utf-8')
    sha_func = sha256()
    #
    counter += 1
    if str_response != expected_resp:
        origin = str(ascii_msg, 'utf-8')
        print("ORIGIN=" + origin)
        print(str_response  + '!=' + expected_resp)
        exit(-1)