import time
from datetime import datetime
import sys
import plot3D as plot
import os
import numpy as np
import math
import sys


def transform(mesh, phi, offset):
	result = []
	pos = 0
	phi = phi/180*3.14159
	for point in mesh :
		result.append([])
		result[pos].append(offset[0] + point[0] * math.cos(phi) - point[1] * math.sin(phi))
		result[pos].append(offset[1] + point[0] * math.sin(phi) + point[1] * math.cos(phi))
		pos+=1

	return result;

ordner = ""

if sys.version_info[0] < 3:
	ordner = "./"+raw_input("Ordner Name: ")+"/"
else:
	ordner = "./"+input("Ordner Name: ")+"/"

if not os.path.exists(ordner):
    os.makedirs(ordner)

versuch = ""

if sys.version_info[0] < 3:
	versuch = raw_input("round: ")
else:
	versuch = input("round: ")

csv = open(ordner+"round"+versuch+".csv").read()

x = np.array([])
y = np.array([])

Ox = 0.0
Oy = 0.0
Ophi = 0.0

first = True
for lines in csv.split('\n'):

    elem = lines.split(';')
    if len(elem) == 3:
        Ox = float(elem[0])
        Oy = float(elem[1])
        Ophi = float(elem[2])
    elif len(elem) == 2:
        x = np.append(x, float(elem[0]))
        y = np.append(y, float(elem[1]))


plot.x = x
plot.y = y

plot.Ox = Ox
plot.Oy = Oy
plot.Ophi = Ophi

mesh = []

mesh.append([0.09, 0.305])
mesh.append([-0.09, 0.305])
mesh.append([-0.09, -0.305])
mesh.append([0.09, -0.305])

mesh = transform(mesh, Ophi, (Ox, Oy))
Mx = []
My = []

for point in mesh:
	Mx.append(point[0])
	My.append(point[1])

Mx.append(mesh[0][0])
My.append(mesh[0][1])

plot.setMesh(Mx, My)
plot.plot2d()
