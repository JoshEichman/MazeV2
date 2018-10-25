# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""

# Populate empty database
door_num = 144
import mysql.connector
mydb = mysql.connector.connect(
  host="localhost",
  user="MazeDoor",
  passwd="rooDezaM*2",
  database="MazeV2")
mycursor = mydb.cursor()
sql = "INSERT INTO data1 (Cell,distance,Door_set) VALUES (%s, %s, %s)"
for i0 in range(door_num): 
    val = (i0+1,0,0)
    mycursor.execute(sql, val)
    mydb.commit()
    #print(mycursor.rowcount, "record inserted.")
mycursor.close()
mydb.close()

# Pull results from the database    localhost
import mysql.connector
import time
import numpy as np

mydb = mysql.connector.connect(
  host="127.0.0.1",
  user="MazeDoor",
  passwd="rooDezaM*2",
  database="MazeV2")
#query = ("SELECT * FROM data1;")
query = ("SELECT Distance FROM data1 WHERE Cell=1;")
c0=0;
result1 = [0];
while 1==1:
    mycursor = mydb.cursor()
    mycursor.execute(query)
    result = mycursor.fetchone()   # fetchmany()   fetchone()
    print("result:",result)
    mycursor.close()
    if result1[c0] != result[0]:    # Check if the value changed and if so record
        c0=c0+1
        result1[c0] = result[0]
    time.sleep(0.1) 
    mydb.reset_session()   
        
mydb.close()
    
 
from MySQLdb.cursors import SSCursor
conn = MySQLdb.connect("localhost", "test", "test", "test")


while True:
    input = raw_input("Enter anything: ")
    if input == "exit":
        break
    c = conn.cursor()
    conn.begin()
    c.execute("SELECT COUNT(*) FROM test")
    res = c.fetchone()[0]
    #c.commit()
    c.close()    
    

# Update data points
import mysql.connector
mydb = mysql.connector.connect(
  host="127.0.0.1",
  user="MazeDoor",
  passwd="rooDezaM*2",
  database="MazeV2")
mycursor = mydb.cursor()
query_update = ("UPDATE mazev2.data1 SET Distance=3 WHERE Cell=1;")
mycursor.execute(query_update)
mydb.commit()
mycursor.close()
mydb.close()


# Check version
import mysql.connector
mydb = mysql.connector.connect(
  host="127.0.0.1",
  user="MazeDoor",
  passwd="rooDezaM*2",
  database="MazeV2")
mycursor.execute("SELECT VERSION()")
ver = mycursor.fetchone()
print("Database version : %s " % ver)


# Pull results from the database    localhost   Nuria-MacBook-Pro.remote
# To log into remote MySQL mac server you can create a user with hostname '%' then use the computer IP as the host. Alternatively you can tunnel into computer then run as local 
mydb = mysql.connector.connect(
  host="192.168.1.5",
  user="user1",
  passwd="password1",
  database="MazeV2mac")
mycursor = mydb.cursor()
query = ("SELECT * FROM table1;")
mycursor.execute(query)
result = mycursor.fetchall()   # fetchmany()   fetchone()
print("result:",result)
mycursor.close()
mydb.close()




sql_select_Query = ("select * from python_developers")
cursor = connection.cursor(buffered=True) #need to set buffered=True to avoid MySQL Unread result error
cursor.execute(sql_select_Query)
fetching_size = 2
records = cursor.fetchmany(fetching_size)

mycursor.close()
mydb.close()



# Echo client program
import socket

HOST = '192.168.1.80'     # The remote host
PORT = 8002               # The same port as used by the server
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))
data = s.recv(4096)
s.sendall(bytes('Distance', 'utf-8')) #'Hello, world')

s.close()
print('Received', repr(data))



import socket

TCP_IP = '192.168.1.9' #Local Host
TCP_PORT = 3007        #Listening Port
BUFFER_SIZE = 12

SERVER_IP = '192.168.1.80'  #Arduino IP
SERVER_PORT = 80            #Server Port

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.bind((TCP_IP,TCP_PORT))
#s.listen(1)

#(conn,addr) = s.accept()
#print('Connection address: ',addr)
while True:
    print('Sending message')
    s.sendto('ECHO',(SERVER_IP,SERVER_PORT))
    print('Waiting for response')
    data,addr = s.recvfrom(BUFFER_SIZE)
    if not data: break
    print('received data: ',data)
    print('data recieved from: ',addr)
    conn.close()
s.close()



# Setup Apache24 and got it working on port 800 but not sure how to interface with python
# Was able to move to port 80 by disabling "World Wide Web Publishing" in services



import http.server
import socketserver

PORT = 8000

Handler = http.server.SimpleHTTPRequestHandler

with socketserver.TCPServer(("", PORT), Handler) as httpd:
    print("serving at port", PORT)
    httpd.serve_forever()







# Used with wifi_server_connect_python_01.ino (arduino file)
import requests

# Now, we call the get method defined in this requests module and pass the URL where we want to make the request as argument. Remember that our URL will be in the format http://IP/path. In our case, the IP will be the value printed on the serial console when we run the code on the ESP8266 and the path will be “/Python”.
# We will store the result of the invocation of this method in a variable called response. Check the code bellow. Take in consideration that the IP that I’m using is the local IP of the ESP8266 on my WiFi network. Yours will most likely be different.
response = requests.get('http://192.168.1.16/Python')

# Finally, we use the text method on the response object to get the answer from the web server
for i in range(100):
    print(response.text)






import serial
import serial.tools.list_ports as port_list
ports = list(port_list.comports())
for p in ports:
    print (p)
    
ser = serial.Serial('COM1')  # open serial port
print(ser.name)         # check which port was really used
#ser.write(b'hello')     # write a string
ser.close()             # close port




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