
class function:

    def run(self, ctx):


    def safeFunction(self):
        content = ""
        for u, speed in self.functionM1:
            content += str(u)+";"+str(speed)+"\n"
        f = open("NoR_per_speed_function1.csv", "w")
        f.write(content)
        f.close()

        content = ""
        for u, speed in self.functionM2:
            content += str(u)+";"+str(speed)+"\n"
        f = open("NoR_per_speed_function2.csv", "w")
        f.write(content)
        f.close()

    def createFunction(self):
        speed = 128
        self.md.disableTimeOut()
        while speed >= 0:
            self.md.setSpeed(speed, speed)
            time.sleep(1)
            NumberOfRevolutions = self.getNumberOfRevolutions()
            print("Drehzahlen: ", NumberOfRevolutions, "u/s")
            self.functionM1.append((NumberOfRevolutions[0], speed))
            self.functionM2.append((NumberOfRevolutions[1], speed))
            speed -= INCREMENT_INTERVALL
        self.safeFunction()
        self.md.enableTimeOut()
        self.md.setSpeed(128, 128)
        self.safeFunction()
