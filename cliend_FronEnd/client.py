#!/usr/bin/python
import socket

HOST = 'Jiaweis-MacBook-Pro.local'
PORT = 0x1234
MSGSIZE = 8192
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# print 'Start connectiing'
s.connect((HOST, PORT))
# print 'Start sending'
s.send('pnl 0')
data = s.recv(MSGSIZE)
s.close()
print data
