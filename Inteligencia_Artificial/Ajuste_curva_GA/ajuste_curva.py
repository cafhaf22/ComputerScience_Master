from scipy.fftpack import shift
import numpy as np
import matplotlib.pyplot as plt
#Coeficientes reales de la curva
A = 8
B = 25
C = 4
D = 45
E = 10
F = 17
G = 35
fig, ax = plt.subplots()

def draw_target_curve():
    x = np.arange(0,100,0.1)
    y = list()
    ax.grid(True)

    for i in range(1000):
        temp = A * (B * np.sin(x[i] / C) + D*np.cos(x[i] / E)) + F*x[i] - G
        y.append(temp)

    plt.xlim(0,100)
    ax.plot(x,y,'r')


if __name__ == '__main__' :
    
    #draw_target_curve()
    #plt.show()

    #Testing bit trim
    a = 150 # 1001 0110
    b = 75 # 0100 1011
    print("valor inicial de a:", a )
    print("valor inicial de b:", b)
    
    cut_pos = 4 # la cuenta va del msb -> lsb
    shifts_pos = 8 - cut_pos


    mask = 0xFF >> cut_pos
    print("mascara:", bin(mask))
    a_low = a & mask
    print("parte que se extrajo de a:",bin(a_low))
    b_low = b & mask
    print("parte que se extrajo de b:",bin(b_low))
    #intercambio
    new_a = a >> shifts_pos
    new_a = (new_a << shifts_pos) | b_low
    print("nueva 'a' despues de los shifts y la or con b_low: ", new_a)
    new_b = b >> shifts_pos
    new_b = (new_b << shifts_pos) | a_low
    print("nueva 'b' despues de los shifts y la or con a_low: ", new_b)
