import socket

UDP_IP = "192.168.1.104"
UDP_PORT = 5005

def sendMSG(msg):
    #print("UDP target IP:", UDP_IP)
    #print("UDP target port:", UDP_PORT)
    print("\rmessage:", msg, end = '')

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP
    msgInB = bytes(msg, 'utf-8')
    sock.sendto(msgInB, (UDP_IP, UDP_PORT))
