import socket
import json
import copy
import threading
import time


class UDP:
	def __init__(self, toSend, toReceive, IP_SENDING, IP_RECEIVING, port = 5005, RECEIVING_TIMEOUT = 5):
		self.sending_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP
		self.receiving_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP
		self.receiving_sock.settimeout(0.0)
		self.connOpen = False
		self.parser = MSGParser()
		self.lastpackageTime = time.time()

		self.toSend = toSend
		self.toReceive = toReceive
		self.IP_SENDING = IP_SENDING
		self.IP_RECEIVING = IP_RECEIVING
		self.port = port
		self.RECEIVING_TIMEOUT = RECEIVING_TIMEOUT

	def __del__(self):
		self.sending_sock.close()
		self.receiving_sock.close()

	def run(self, ctx):
		msg = self.parser.getMSG(ctx, self.toSend)
		self.sendMSG(msg)

		msg = self.getLastMSG()
		if(msg != ""):
			self.lastpackageTime = time.time()
			self.parser.unpackMSG(ctx, self.toReceive, msg)
		elif(time.time() - self.lastpackageTime >= self.RECEIVING_TIMEOUT):
			ctx["mode"] = 0


	def sendMSG(self, msg):
		msgInB = bytes(msg, 'utf-8')
		self.sending_sock.sendto(msgInB, (self.IP_SENDING, self.port))

	def getLastMSG(self):
		bufferEmpty = False
		lastMSG = ""
		while not bufferEmpty:
			msg = self.getMSG()
			if msg == "":
				bufferEmpty = True
			else:
				lastMSG = msg
		return lastMSG

	def getMSG(self):
		if not self.connOpen :
			self.openConnection()
		data = ""
		try:
			data, addr = self.receiving_sock.recvfrom(1024) # buffer size is 1024 bytes
		except:
			return ""
		return data.decode("utf-8")

	def openConnection(self):
		self.receiving_sock.bind((self.IP_RECEIVING, self.port))
		self.connOpen = True



class MSGParser:
	msgID = 0

	def getMSG(self, ctx, toSend):
		msg = {}
		for varName in toSend:
			msg[varName] = ctx[varName]
		return json.dumps(msg)

	def unpackMSG(self, ctx, toReceive, raw_msg):
		msg = json.loads(raw_msg)
		for varName in toReceive:
			try:
				ctx[varName] = msg[varName]
			except:
				print("error: "+varName+" missing in received message")
