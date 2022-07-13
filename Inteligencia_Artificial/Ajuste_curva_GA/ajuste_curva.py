from pickletools import uint2
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

population_size = 100

fig, ax = plt.subplots()

def calc_coeficients(cromosoma, weight_numerator):
    for i in range(len(cromosoma)):
        weight = np.ceil(weight_numerator / cromosoma[i])
        cromosoma[i] = np.ceil(cromosoma[i] / weight)

def gen_population():
    #generamos numeros entre 1 - 255 en un array de 100 x 7
    return np.random.randint(1,256, size= (population_size, 7))


def calc_curve(x,a,b,c,d,e,f,g):
    y = list()
    for i in range(len(x)):
        temp = a * (b * np.sin(x[i] / c) + d*np.cos(x[i] / e)) + f*x[i] - g
        y.append(temp)
    return y

def draw_target_curve():
    x = np.arange(0,100,0.1)
    y = list()
    ax.grid(True)

    for i in range(len(x)):
        temp = A * (B * np.sin(x[i] / C) + D*np.cos(x[i] / E)) + F*x[i] - G
        y.append(temp)

    plt.xlim(0,100)
    ax.plot(x,y,'r')

def func_aptitude(cromosoma):
    x = np.arange(0,100,0.1)
    real_y = calc_curve(x,A,B,C,D,E,F,G)
    new_y = calc_curve(x, population[0,0], population[0,1], population[0,2], population[0,3], population[0,4], population[0,5], population[0,6])
    new_y = np.array(new_y)
    ap_func = np.abs(real_y - new_y) #error puntual
    sum_err = np.sum(ap_func)
    return sum_err


if __name__ == '__main__' :
    
    #draw_target_curve()
    x = np.arange(0,100,0.1)
    #Algoritmo de ajuste de curva
    weight_numerator = 255 #maximo valor que puede tomar un numero de 8bits
    #obtener la poblacion inicial
    population = gen_population()
    #calculando los nuevos coeficientes
    for i in range(len(population)): #calculando los coeficientes por cada cromosoma
        #hicimos el preprocesado hasta aqui
        calc_coeficients(population[i], weight_numerator) #population actualiza los cromosomas divididos por el peso
    
    #calcular la funcion de aptitud para cada caso
    ap_func = list()
    for i in range(len(population)):
        ap_func.append(func_aptitude(population[i]))
    print(ap_func[0])






    
    
    
    
    #Testing bit trim
    #a = 150 # 1001 0110
    #b = 75 # 0100 1011
    #print("valor inicial de a:", a )
    #print("valor inicial de b:", b)
    #
    #cut_pos = 4 # la cuenta va del msb -> lsb
    #shifts_pos = 8 - cut_pos
#
#
    #mask = 0xFF >> cut_pos
    #print("mascara:", bin(mask))
    #a_low = a & mask
    #print("parte que se extrajo de a:",bin(a_low))
    #b_low = b & mask
    #print("parte que se extrajo de b:",bin(b_low))
    ##intercambio
    #new_a = a >> shifts_pos
    #new_a = (new_a << shifts_pos) | b_low
    #print("nueva 'a' despues de los shifts y la or con b_low: ", new_a)
    #new_b = b >> shifts_pos
    #new_b = (new_b << shifts_pos) | a_low
    #print("nueva 'b' despues de los shifts y la or con a_low: ", new_b)
