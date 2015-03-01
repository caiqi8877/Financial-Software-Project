#!/usr/bin/python
import socket
import string
import math

HOST = socket.gethostname()
PORT = 0x1234
MSGSIZE = 8192
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))

s.send('lgddf 0')
data = s.recv(MSGSIZE)
msg = data.split()
print 'LGD change is:', msg[1]

s.send('vardf 99')
data = s.recv(MSGSIZE)
msg = data.split()
print 'VaR change is:', msg[1]

s.send('wtime 0')
data = s.recv(MSGSIZE)
msg = data.split()
print 'The time of the server is:'
print 'Real Time:',msg[1]
print 'User Time:',msg[2]
print 'Sys Time: ',msg[3]

s.close()
