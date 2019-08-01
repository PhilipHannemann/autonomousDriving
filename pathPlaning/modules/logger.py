import numpy as np
import matplotlib.pyplot as plt

FILE_NAME = "./diagram/diagram.png"


class Logger:
    def __init__(self, filename):
        self.f = open(filename, 'w')
        self.NiOverTime = [[], []]
        self.NsOverTime = [[], []]
        self.time = []

    def __del__(self):
        self.f.close()

    def run(self, ctx):
        self.f.write("{:.2f}, {}, {}, {}, error:{}\n".format(ctx["t"], ctx["speed"], ctx["n_s"], ctx["n_i"], ctx["error"]))
        self.NiOverTime[0].append(ctx["n_i"][0])
        self.NiOverTime[1].append(ctx["n_i"][1])
        self.NsOverTime[0].append(ctx["n_s"][0])
        self.NsOverTime[1].append(ctx["n_s"][1])
        self.time.append(ctx["t"])

    def plotSpeed(self):
	    plt.subplot(211)
	    plt.plot(self.time, self.NiOverTime[0], 'b-')
	    plt.plot(self.time, self.NsOverTime[0], 'r-')
	    plt.grid(True)
	    plt.title('Motor 1')

	    plt.subplot(212)
	    plt.plot(self.time, self.NiOverTime[1], 'b-')
	    plt.plot(self.time, self.NsOverTime[1], 'r-')
	    plt.grid(True)
	    plt.title('Motor 2')

	    plt.savefig(FILE_NAME)
