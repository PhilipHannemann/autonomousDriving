

ERROR_MULTIPLIER = 0.1


class Controller:
    error = [0, 0]

    def __init__(self):
        self.readFunction()

    def run(self, ctx):
        self.calculateError(ctx["n_i"], ctx["n_s"])
        ctx["speed"] = self.concadinateSpeedAndError(self.error, ctx["n_s"])
        ctx["error"] = self.error

    def calculateError(self, n_i, n_s):
        self.error[0] += (n_s[0] - n_i[0])*ERROR_MULTIPLIER
        self.error[1] += (n_s[1] - n_i[1])*ERROR_MULTIPLIER

    def concadinateSpeedAndError(self, error, n_s):
        n  = (n_s[0] + error[0], n_s[1] + error[1])
        speed = [self.getSpeedForMotor(n[0], 0), self.getSpeedForMotor(n[1], 1)]
        return speed

    def readFunction(self):
        m1 = open("functions/speedTableMotor1.csv", "r").read()
        m2 = open("functions/speedTableMotor2.csv", "r").read()

        self.functionM1 = self.csvToFunction(m1)
        self.functionM2 = self.csvToFunction(m2)

    def csvToFunction(self, csv):
        f = []
        for lines in csv.split('\n'):
            elem = lines.split(';')
            if len(elem) == 2:
                f.append((float(elem[0]), float(elem[1])))
        return f

    def getSpeedForMotor(self, n, motor):
        if len(self.functionM1) == 0 or len(self.functionM2) == 0:
            print("error: speedFunction is empty")
            return

        direction = 1 if n > 0 else -1
        n = abs(n)
        f = self.functionM1 if motor == 1 else self.functionM2
        v = -1
        i = 0
        for NumberOfRevolutions, speed in f:
            if NumberOfRevolutions < n:
                i += 1
                continue

            deltaN = f[i-1][0] - NumberOfRevolutions
            deltaS = f[i-1][1] - speed
            nd = n - f[i-1][0]

            v = nd/deltaN * deltaS + f[i-1][1]
            break
        if v == -1:
            v = 0
        v = ((v - 128) * direction) + 128
        return v
