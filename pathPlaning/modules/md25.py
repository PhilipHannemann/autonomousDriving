from .md25serial import md25 as serial

SERIAL_PORT = ""


class MD25_read:
    def __init__(self):
        self.ser = serial(SERIAL_PORT)

    def run(self, ctx):
        ctx["encoders"] = self.ser.getEncoders()


class MD25_write:
    def __init__(self):
        self.ser = serial(SERIAL_PORT)
        self.ser.enableTimeOut()
        self.stopReceived = False

    def run(self, ctx):
        if(ctx["mode"] == 1):
            self.ser.setSpeed(ctx["speed"][0], ctx["speed"][1])
            self.stopReceived = False
        else:
            if not self.stopReceived:
                self.ser.setSpeed(128, 128)
                self.stopReceived = True
        #self.ser.setSpeed2(ctx.dic["speed"][1])
