from math import *

class Odometry:
    def run(self, ctx):
        self.ctx = ctx
        speed1 = ctx["n_i"][0] * 0.298  #m/s
        speed2 = ctx["n_i"][1] * 0.298  #m/s

        distance1 = speed1 * ctx["timeIntervall"]
        distance2 = speed2 * ctx["timeIntervall"]
        d = (distance1 + distance2)/2.0

        phi = self.getAngle(distance1, distance2)
        r = self.getRadius(phi, distance1)

        direction = self.getDirection(phi, d)

        x, y = self.transform(r, phi, direction, d)

        ctx["x"] += x
        ctx["y"] += y
        ctx["phi"] += phi

        print("\rpos:", ctx["x"], ctx["y"], ctx["phi"])


    def getAngle(self, d1, d2):
        return (d1 - d2) / 0.28   # 0.28 m = Radabstand

    def getRadius(self, phi, d1):
        if phi == 0:
            return 0
        return d1 / phi - 0.14  # 0.14 m = halber Radius

    def transform(self, r, phi, direction, d):
        dx = r*cos(phi)
        dy = r*sin(phi)

        if direction == "right":
            dx = dx * (-1) + r
        else:
            dx -= r
            dx *= -1

        if r == 0:
            dy = d
            dx = 0

        x = dx *cos(self.ctx["phi"]) - dy * sin(self.ctx["phi"])
        y = dx *sin(self.ctx["phi"]) + dy * cos(self.ctx["phi"])

        return (x, y)

    def getDirection(self, phi, d):
        if phi < 0 and d < 0:
            return "right"
        if phi > 0 and d > 0:
            return "right"

        return "left"
