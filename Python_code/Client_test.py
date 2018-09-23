# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""

import serial

ser = serial.Serial('/dev/ttyp0')  # open serial port
print(ser.name)         # check which port was really used
#ser.write(b'hello')     # write a string
ser.close()             # close port


ser = serial.Serial()
ser.baudrate = 9600
ser.port = '/dev/ttyp3'
ser
ser.open()



"""
import socket

# Create a TCP/IP socket
sock = socket.create_connection(('192.168.1.9', 21))
sock.sendall(bytes('Distance', 'utf-8'))
sock.send('e'.encode()) 
sock.close()
"""



"""
import socket
import sys

def get_constants(prefix):
    # Create a dictionary mapping socket module constants to their names.
    return dict( (getattr(socket, n), n)
                 for n in dir(socket)
                 if n.startswith(prefix)
                 )

families = get_constants('AF_')
types = get_constants('SOCK_')
protocols = get_constants('IPPROTO_')

# Create a TCP/IP socket
sock = socket.create_connection(('localhost', 10000))

print >>sys.stderr, 'Family  :', families[sock.family]
print >>sys.stderr, 'Type    :', types[sock.type]
print >>sys.stderr, 'Protocol:', protocols[sock.proto]
print >>sys.stderr

try:
    
    # Send data
    message = 'This is the message.  It will be repeated.'
    print >>sys.stderr, 'sending "%s"' % message
    sock.sendall(message)

    amount_received = 0
    amount_expected = len(message)
    
    while amount_received < amount_expected:
        data = sock.recv(16)
        amount_received += len(data)
        print >>sys.stderr, 'received "%s"' % data

finally:
    print >>sys.stderr, 'closing socket'
    sock.close()
"""