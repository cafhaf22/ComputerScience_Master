from matplotlib import tri
import numpy as np
import matplotlib.pyplot as plt


def calculate_general_bell_mf(x, a, b, c):
    bell = list()
    for i in range(len(x)):
        bell_i = 1 / ( 1 + (abs((x[i] - c) / a) ** (2*b)))
        bell.append(bell_i)
    return bell

def calculate_gaussian_mf(x, c, sigma):
    gaus = list()
    gaus = np.exp((- 1/2 * (((x - c) / sigma) ** 2)))
    return gaus

def calculate_triangle_mf(x,a,b,c):
    triangle = list()
    for i in range(len(x)):
        triangle_i = max(min((x[i] - a) / (b - a), (c - x[i]) / (c - b)), 0)
        triangle.append(triangle_i)
    return triangle

def calculate_trapezoid_mf(x,a,b,c,d):
    trapezoid = list()
    for i in range(len(x)):
        trapezoid_i = max(min((x[i] - a) / (b - a), 1, (d - x[i]) / (d -c)), 0)
        trapezoid.append(trapezoid_i)
    return trapezoid



fig, ax = plt.subplots()
#formato de la figura
plt.ylim(0, 1.1)
plt.xlim(0, 90) # va a cambiar dependiendo de lo que grafique
plt.grid(True)
plt.ylabel("Membership Grades")
plt.xlabel("Ages")
ax.text(5, 1.02, "Young", fontsize=15)
ax.text(35, 1.02, "Middle age", fontsize=15)
ax.text(75, 1.02, "Old", fontsize=15)
#calculando funciones de pertenencia (MF's)
x = np.arange(0, 91, 1)
#middle age
y = calculate_general_bell_mf(x, 15, 4, 45)
ax.plot(x,y)
ax.plot([45,30,60],[1,0.5,0.5], '.')
#young
y = calculate_general_bell_mf(x, 25, 6, 5)
ax.plot(x,y)
#old
y = calculate_general_bell_mf(x, 25, 6, 85)
ax.plot(x,y)
#g = calculate_gaussian_mf(x, 50, 20)
#ax.plot(x,g, 'r--')
#t = calculate_triangle_mf(x, 20, 60, 80)
#ax.plot(x,t, 'g-')
#f = calculate_trapezoid_mf(x, 10, 20, 60, 95)
#ax.plot(x,f, '--')
plt.show()