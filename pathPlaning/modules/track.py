import time


class Track:

    def run(self, ctx):
        ctx["mode"] = 1
        if ctx["t"] >= 1.0 and ctx["t"] <= 4.0:
            ctx["n_s"][0] = (1.79/3.0)/0.298
            ctx["n_s"][1] = (1.35/3.0)/0.298
        elif ctx["t"] > 4.0 and ctx["t"] <= 8.0:
            ctx["n_s"][0] = 2.0
            ctx["n_s"][1] = 2.0
        else:
            ctx["n_s"][0] = 0.0
            ctx["n_s"][1] = 0.0
