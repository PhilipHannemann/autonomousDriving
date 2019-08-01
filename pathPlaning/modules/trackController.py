import time
import math

class TrackController:

    def run(self, ctx):
        for points in ctx[""]:
            pass
        # aus weg geschwindigkeiten und drehzahlen berrechen
        #die daraus berrechen, wo sich der Roboter aktuell aufhält über die zeit



class AccelerationController:
    acceleration = 0.5
    first = True

    def run(self, ctx):
        if first:
            self.f_s = self.getDistance(ctx["dEnd"], ctx["tEnd"])
            self.f_v = self.getVelocity(ctx["dEnd"], ctx["tEnd"])
            ctx["n_s"] = [f_v[1][1]/0.298, f_v[1][1]/0.298]
            first = False
        else:
            for t, v_s in self.f_v:
                if math.abs(t - ctx["t"]) <= ctx["timeIntervall"]/2.0:
                    v_i = ctx["n_i"]*0.298
                    deltaV = v_s - v_i
                    ctx["n_s"] += deltaV / 2.0
                    break



    def getDistance(self, d, t):
        a = self.acceleration
        t2 = math.sqrt((a * t^2 - 4*d) / a)  # negative Lösung wird ignoriert, da keine negative Zeit möglich
        t1 = (t-t2)/2

        distanceOverTime = [[],[]]
        for time in range(0, t1*2 + t2, 0.1):
            s = 0
            if time <= t1:
                s = a/2 * time^2
            elif time <= t1+t2:
                s = a/2 * t1^2 + a*t1*(time-t1)
            else:
                s = a/2 * t1^2 + a*t1*t2 - a/2 *(time - t1-t2)^2

            distanceOverTime[0].append(s)
            distanceOverTime[1].append(time)

        return distanceOverTime


    def getVelocity(self, d, t):
        a = self.acceleration
        t2 = math.sqrt((a * t^2 - 4*d) / a)  # negative Lösung wird ignoriert, da keine negative Zeit möglich
        t1 = (t-t2)/2

        speedOverTime = [[],[]]
        for time in range(0, t1*2 + t2, 0.1):
            v = 0
            if time <= t1:
                v = a * time
            elif time <= t1+t2:
                v = a * t1
            else:
                v = a * t1 - a *(time - t1-t2)

            speedOverTime[0].append(v)
            speedOverTime[1].append(time)

        return speedOverTime
