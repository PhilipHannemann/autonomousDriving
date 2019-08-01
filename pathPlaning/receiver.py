from modules.logger import *
from modules import md25
from modules.sensor import *
from modules.udp import *
from core import *
import time


if __name__ == '__main__':
    ctx = {
        "timeIntervall": 0.1,
        "t": 0,
        "encoders" : [0, 0],
        "n_i" : [0, 0],
        "speed" : [128, 128],
        "n_s" : [0, 0],
        "error" : [0,0],
        "mode" : 0}

    udp = UDP(
        ["n_i"],
        ["speed","mode"],
        "192.168.1.106",
        "192.168.1.104")

    md25.SERIAL_PORT = '/dev/ttyUSB0'

    loop = Loop(ctx)

    loop.mod_100ms = [
        md25.MD25_read(),
        Sensor(),
        md25.MD25_write(),
        Logger('log.txt'),
        udp
    ]

    loop.start()

    while input('x to end:') != 'x':
        pass

    loop.stop()
