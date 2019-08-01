
class Sensor:
    valid = False
    e = [0, 0]

    def run(self, ctx):
        if not self.valid:
            self.valid = True
            self.e = ctx["encoders"]
            return

        deltaE1 = self.getDeltaE(ctx["encoders"][0], self.e[0])
        deltaE2 = self.getDeltaE(ctx["encoders"][1], self.e[1])

        ctx["n_i"][0] = (deltaE1/360) / ctx["timeIntervall"]
        ctx["n_i"][1] = (deltaE2/360) / ctx["timeIntervall"]
        self.e = ctx["encoders"]


    def getDeltaE(self, e1, e2):
        max = int(b'\xff\xff\xff\xff'.hex(), 16)

        e1_high = self.isHigh(e1)
        e2_high = self.isHigh(e2)
        e1_low = self.isLow(e1)
        e2_low = self.isLow(e2)

        deltaE = e2 - e1

        if(e1_high and e2_low):
            deltaE = e2 - e1 + max
        if(e1_low and e2_high):
            deltaE = e2 - e1 - max

        return deltaE


    def isHigh(self, e):
        max = int(b'\xff\xff\xff\xff'.hex(), 16)
        if (max - e) < 10000:
            return True
        return False

    def isLow(self, e):
        if e < 10000:
            return True
        return False
