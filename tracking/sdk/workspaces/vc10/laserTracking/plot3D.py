from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
from matplotlib import cm
import numpy as np
from PIL import Image


fig = plt.figure()

x = np.array([])
y = np.array([])
z = np.array([])

mesh = np.array([])

Ox = 0.0
Oy = 0.0
Ophi = 0.0

def append(X, Y, Z):
    global x, y, z
    x = np.append(x, X)
    y = np.append(y, Y)
    z = np.append(z, Z)

def reset():
    global x, y, z
    x = np.array([])
    y = np.array([])
    z = np.array([])
    plt.clf()
    plt.cla()
    plt.close()

def plot3d():
    p = fig.add_subplot(111, projection='3d')
    p.scatter(x, y, z, c='r', marker='o')
    p.set_xlabel('X Achse')
    p.set_ylabel('Y Achse')
    p.set_zlabel('Z Achse')
    plotBoundingBox(p)
    plt.show()

def plotBoundingBox(p):
    max_range = np.array([x.max()-x.min(), y.max()-y.min(), z.max()-z.min()]).max()
    Xb = 0.5*max_range*np.mgrid[-1:2:2,-1:2:2,-1:2:2][0].flatten() + 0.5*(x.max()+x.min())
    Yb = 0.5*max_range*np.mgrid[-1:2:2,-1:2:2,-1:2:2][1].flatten() + 0.5*(y.max()+y.min())
    Zb = 0.5*max_range*np.mgrid[-1:2:2,-1:2:2,-1:2:2][2].flatten() + 0.5*(z.max()+z.min())

    for xb, yb, zb in zip(Xb, Yb, Zb):
       p.plot([xb], [yb], [zb], 'w')

def setup2d():
    plt.figure(1)
    plt.plot(x, y, "gx")
    plt.grid(True)
    plt.title('Plot')
    plt.axis('equal')

    plt.plot(Ox, Oy, "rx")

    #plt.gca().yaxis.set_minor_formatter(NullFormatter())
    # Adjust the subplot layout, because the logit one may take more space
    # than usual, due to y-tick labels like "1 - 10^{-3}"
    plt.subplots_adjust(top=0.92, bottom=0.08, left=0.10, right=0.95, hspace=0.25,
                        wspace=0.35)

def plot2dDefault(x, y, labelX, labelY):
    plt.figure(1)
    plt.plot(x, y)
    plt.grid(True)
    plt.title('Plot')
    if labelX != "t":
        plt.axis('equal')
    if labelX == "t":
        plt.gcf().autofmt_xdate()
    if labelY == "t":
        plt.gcf().autofmt_ydate()
    plt.show()

def setMesh(x, y):
    plt.plot(x, y, "r-")

def plot2d():
    setup2d()
    plt.show()

def safe2d(name):
    setup2d()
    plt.savefig(name+'.png')
