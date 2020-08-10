#!/usr/bin/env python3

import socket
import time

HOST = '127.0.0.1'
PORT = 5250

set_request = b'set\tfoo\tbar\n'
get_request = b'get\tfoo\n'
del_request = b'del\tfoo\n'


def make_connection(request):

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        s.sendall(request)
        data = s.recv(1024)
    return data

set_data = make_connection(set_request)

assert set_data.decode('utf-8') == "OK"

get_data = make_connection(get_request)
assert get_data.decode('utf-8') == "bar"

del_data = make_connection(del_request)
assert del_data.decode('utf-8') == "OK"
