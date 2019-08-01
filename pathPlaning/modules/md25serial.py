import serial
import threading

MD25_GET_SPEED1 = 0x21
MD25_GET_SPEED2 = 0x22
MD25_GET_ENCODER1 = 0x23
MD25_GET_ENCODER2 = 0x24
MD25_GET_ENCODERS = 0x25
MD25_GET_VOLTS = 0x26
MD25_GET_CURRENT1 = 0x27
MD25_GET_CURRENT2 = 0x28
MD25_GET_VERSION = 0x29
MD25_GET_ACCELERATION = 0x2A #beschleunigung
MD25_GET_MODE = 0x2B
MD25_GET_VI = 0x2C

MD25_SET_SPEED1 = 0x31
MD25_SET_SPEED2_TURN = 0x32
MD25_SET_ACCELERATION = 0x33
MD25_SET_MODE = 0x34

MD25_CMD_RESET_ENCODERS = 0x35
MD25_CMD_DISABLE_REGULATOR = 0x36
MD25_CMD_ENABLE_REGULATOR = 0x37
MD25_CMD_DISABLE_TIMEOUT = 0x38
MD25_CMD_ENABLE_TIMEOUT = 0x39

SYNC_BYTE = 0x00


class md25:
	ser = ()
	lock = threading.Lock()

	def __init__(self, portDetail):
		if type(self).ser == ():
			type(self).ser = serial.Serial(portDetail, 19200, bytesize=8, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_TWO, timeout=500)
		self.setMode(turn=0)

	def __del__(self):
		self.ser.close()

	def writeReg(self, commandAddr, value):
		command = bytes(bytearray([SYNC_BYTE, commandAddr, value]))

		type(self).lock.acquire()
		self.ser.write(command)
		type(self).lock.release()

	def resetReg(self, commandAddr):
		command = bytes(bytearray([SYNC_BYTE, commandAddr]))

		type(self).lock.acquire()
		self.ser.write(command)
		type(self).lock.release()


	def readReg(self, hexCode, returnSize):
		command = bytes(bytearray([SYNC_BYTE, hexCode]))

		type(self).lock.acquire()
		self.ser.write(command)
		received = self.ser.read(returnSize)
		type(self).lock.release()
		return received


	def setMode(self, signed=0, turn=0):
		mode = 0
		if turn:
			mode = 2
		if signed:
			mode += 1
		self.writeReg(MD25_SET_MODE, mode)

	#Speed
	def setSpeed(self, speed):
		self.setSpeed(speed, speed)

	def setSpeed(self, speed1, speed2):
		self.setSpeed1(speed1)
		self.setSpeed2(speed2)

	def setSpeed1(self, speed1):
		self.writeReg(MD25_SET_SPEED1, self.setBounds(speed1))

	def setSpeed2(self, speed2):
		self.writeReg(MD25_SET_SPEED2_TURN, self.setBounds(speed2))

	def setTurn(self, turn):
		r = self.setBounds(turn)
		self.writeReg(MD25_SET_SPEED2_TURN, r)

	def setMotion(self, speed, turn):
		m1_percent = 1
		m2_percent = 1
		turn = -1*(turn-128)

		if turn > 0:
			m1_percent = 1 - (turn/128.0)
		elif turn < 0:
			m2_percent = 1 + (turn/128.0)

		speed1 = ((speed-128)*m1_percent)+128
		speed2 = ((speed-128)*m2_percent)+128

		self.writeReg(MD25_SET_SPEED1, self.setBounds(speed1))
		self.writeReg(MD25_SET_SPEED2_TURN, self.setBounds(speed2))

	def resetEncoders(self):
		self.resetReg(MD25_CMD_RESET_ENCODERS)

	def getVersion(self):
		return self.readReg(MD25_GET_VERSION, 1)

	def getBatteryV(self):
		battery = self.readReg(MD25_GET_VOLTS, 1)
		return ord(battery)/10.0

	def getCurrent(self):
		current1 = self.readReg(MD25_GET_CURRENT1, 1)
		current2 = self.readReg(MD25_GET_CURRENT2, 1)
		return ord(current1), ord(current2)

	def getSpeed(self):
		speed1 = self.readReg(MD25_GET_SPEED1, 1)
		speed2 = self.readReg(MD25_GET_SPEED2, 1)
		return ord(speed1), ord(speed2)

	def getEncoders(self):
		encoder1 = self.readReg(MD25_GET_ENCODER1, 4)
		encoder2 = self.readReg(MD25_GET_ENCODER2, 4)
		return int(encoder1.hex(), 16), int(encoder2.hex(), 16)

	def setBounds(self, value):
		if value > 255:
			value = 255
		if value < 0:
			value = 0
		value = round(value)
		return int(value)

	def disableTimeOut(self):
		self.resetReg(MD25_CMD_DISABLE_TIMEOUT)

	def enableTimeOut(self):
		self.resetReg(MD25_CMD_ENABLE_TIMEOUT)

	def disableRegulator(self):
		self.resetReg(MD25_CMD_DISABLE_REGULATOR)

	def enableRegulator(self):
		self.resetReg(MD25_CMD_ENABLE_REGULATOR)
