from threading import Timer
import time

class Loop:
    def __init__(self, ctx):
        self.mod_100ms = []
        self.ctx = ctx

    def start(self):
        self.timer100ms = RepeatTimer(self.ctx["timeIntervall"], self.run)
        self.timer100ms.start()
        time.sleep(5)

    def stop(self):
        self.timer100ms.cancel()
        for module in self.mod_100ms:
            del(module)

    def run(self):
        self.ctx["t"] += self.ctx["timeIntervall"]

        for module in self.mod_100ms:
            module.run(self.ctx)

class RepeatTimer(Timer):
    def run(self):
        while not self.finished.wait(self.interval):
            self.function(*self.args, **self.kwargs)
