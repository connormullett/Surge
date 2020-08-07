#!/usr/bin/env python3

import socket

HOST = '127.0.0.1'
PORT = 5250

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    s.sendall(b'\tget\tfoo\n')
    data = s.recv(1024)

printf(data)
