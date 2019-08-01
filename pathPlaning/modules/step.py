
class Step:
    def run(self, ctx):
        ctx["mode"] = 1
        if ctx["t"] >= 1.0:
            ctx["n_s"][0] = 2.0
            ctx["n_s"][1] = 2.0
        else:
            ctx["n_s"][0] = 0.0
            ctx["n_s"][1] = 0.0
