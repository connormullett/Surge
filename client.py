#!/usr/bin/env python3

import socket
import time

HOST = '127.0.0.1'
PORT = 5250


def parse_command(command):
    command = command.split()
    out = ""

    # get and del
    if command[0] == "get" or command[0] == "del":
        out = f"{command[0]}\t{command[1]}\n"
    # set
    elif command[0] == "set":
        value = " ".join(command[2:])
        out = f"{command[0]}\t{command[1]}\t{value}\n"
    else:
        print("Invalid arguments")
        exit()
    return out


def make_connection(request):
    request = request.encode()

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        s.sendall(request)
        data = s.recv(1024)
    return data.decode('utf-8')


def main():

    while 1:
        command = input('> ')

        if command == "quit":
            exit()

        request = parse_command(command)
        data = make_connection(request)

        print(data)


if __name__ == '__main__':
    main()
