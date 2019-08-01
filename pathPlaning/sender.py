from modules.logger import *
from modules.controller import *
from modules.track import *
from modules.odometry import *
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
        "mode" : 0,
        "x" : 0,
        "y" : 0,
        "phi" : 0}

    udp = UDP(
        ["speed","mode"],
        ["n_i"],
        "192.168.1.104",
        "192.168.1.106",
        RECEIVING_TIMEOUT = 10)

    loop = Loop(ctx)

    loop.mod_100ms = [
        Track(),
        Controller(),
        Odometry(),
        Logger('log.txt'),
        udp
    ]

    loop.start()

    while input('x to end:') != 'x':
        pass

    ctx["mode"] = 0

    loop.stop()
