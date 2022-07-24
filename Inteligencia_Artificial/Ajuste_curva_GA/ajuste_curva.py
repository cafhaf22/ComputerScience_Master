from cProfile import label
from pickletools import uint2
from scipy.fftpack import shift
import numpy as np
import matplotlib.pyplot as plt
import time
#Coeficientes reales de la curva
A = 8
B = 25
C = 4
D = 45
E = 10
F = 17
G = 35

population_size = 200
cont_percent = 0.03
mutation_percent = 0.05
mutation = True
tournaments_per_generation = int(population_size/2)
num_generations = 100


fig, (ax1,ax2) = plt.subplots(1,2, figsize= (12, 8))

def calc_coeficients(cromosoma, weight_numerator = 255):
    denom = [A,B,C,D,E,F,G]
    for i in range(len(cromosoma)):
        weight = np.ceil(weight_numerator /denom[i])
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

def target_curve():
    x = np.arange(0,100,0.1)
    y = list()
    for i in range(len(x)):
        temp = A * (B * np.sin(x[i] / C) + D*np.cos(x[i] / E)) + F*x[i] - G
        y.append(temp)
    return y

def func_aptitude(cromosoma):
    x = np.arange(0,100,0.1)
    real_y = calc_curve(x,A,B,C,D,E,F,G)
    new_y = calc_curve(x, cromosoma[0], cromosoma[1], cromosoma[2], cromosoma[3], cromosoma[4], cromosoma[5], cromosoma[6])
    new_y = np.array(new_y)
    ap_func = np.abs(real_y - new_y) #error puntual
    sum_err = np.sum(ap_func)
    return sum_err

def select_contenders():
    num_contenders = int(np.ceil(population_size * cont_percent))
    contenders = np.random.randint(0,population_size, num_contenders)
    return contenders

def select_individual_with_lowest_err(contenders):
    individuals_picked_err = []
    for contender in contenders:
        individuals_picked_err.append(ap_func[contender])
    lowest_err = np.min(individuals_picked_err)
    round_winner = np.argmin(individuals_picked_err)
    #print("Individuos escogidos:")
    #print(contenders)
    #print("errores de cada individuo")
    #print(individuals_picked_err)
    #print("Ganador de la ronda es: ", contenders[round_winner], " con un error de ", lowest_err)
    return contenders[round_winner], lowest_err

def apply_reproduction_operator(winner_father, winner_mother):
    mask = 0xFF
    #print("Cromosoma padre: ", population[winner_father])
    #print("Cromosoma madre: ", population[winner_mother])
    #seleccionando el byte en el que se realizara la operacion
    select_byte = np.random.randint(0,7) # un numero entre el 0 y el 6
    select_bit = np.random.randint(0, 9) #un numero entre el 0 y el 8 (si se selecciona el 0 entonces solo se invierten los numeros completos) 
    #print("Posicion del Gen seleccionado:", select_byte)
    #print("Posicion del Bit de corte:", select_bit)
    wf = population[winner_father][select_byte]
    wm = population[winner_mother][select_byte]
    #print("Gen de corte padre:", wf)
    #print("Gen de corte madre: ", wm)
    
    shift_pos = 8 - select_bit
    mask = mask >> select_bit
    #print("mascara:", bin(mask))
    f_low = wf & mask
    m_low = wm & mask
    #print("parte que se extrajo del padre:",bin(f_low))
    #print("parte que se extrajo de la madre:",bin(m_low))
    #intercambio
    new_f = wf >> shift_pos
    new_f = (new_f << shift_pos) | m_low
    new_m = wm >> shift_pos
    new_m = (new_m << shift_pos) | f_low
    #print("nuevo 'padre' despues de los shifts y la or con m_low: ", new_f) #podria ser 0
    #print("nueva 'madre' despues de los shifts y la or con f_low: ", new_m) #podria ser 0
    if new_m == 0:
        new_m = wm
    if new_f == 0:
        new_f = wf

    #crear nuevos cromosomas hijos
    #intercambiando los nuevos valores y los byte restantes
    crom_father_son =  population[winner_father].copy()
    crom_mother_son =  population[winner_mother].copy()
    crom_father_son[select_byte] = new_f
    crom_mother_son[select_byte] = new_m
    #intercambiando los bytes restantes
    select_byte += 1
    temp_f_son = crom_father_son.copy()
    while(select_byte < len(crom_father_son)):
        crom_father_son[select_byte] = crom_mother_son[select_byte]
        crom_mother_son[select_byte] = temp_f_son[select_byte]
        select_byte += 1
        
    
    #print("hijo del padre: ")
    #print(crom_father_son)

    #print("Hijo de la madre: ")
    #print(crom_mother_son)
    return crom_father_son, crom_mother_son

def apply_mutation():
    num_crom_mutation = int(np.ceil(population_size * mutation_percent))
    mutation_individuals = np.random.choice( population_size, num_crom_mutation)
    #print("Individuos seleccionados para mutacion:", mutation_individuals)
    select_byte = np.random.randint(0, 7) 
    select_bit = np.random.randint(1, 9) #no puede ser 0 y no puede ser mayor de 8
    mask = 1 << (select_bit - 1)
    for i in mutation_individuals:
        #print("individuo ",i,"= ",population[i])
        #print("gen seleccionado: ", population[i][select_byte])
        if((population[i][select_byte]  ^ mask) > 0):
            population[i][select_byte] = population[i][select_byte]  ^ mask
        #print("Depues de la mutacion")
        #print("individuo ",i,"= ",population[i])
        #print("gen seleccionado: ", population[i][select_byte])

def apply_elitism():
    pass

def get_fittest_of_generation(ap_func):
    err_per_gen = ap_func.copy()
    ind_with_lowest_err_of_gen = np.argmin(err_per_gen)
    #print("Individuo con el menor erro de la generacion", ind_with_lowest_err_of_gen)
    #print("Error", err_per_gen[ind_with_lowest_err_of_gen])
    #print("Parametros curva", ind_with_lowest_err_of_gen, ":", population[ind_with_lowest_err_of_gen])
    return ind_with_lowest_err_of_gen

if __name__ == '__main__' :
    ax1.grid(True)
    x = np.arange(0,100,0.1)
    y = target_curve()
    target, = ax1.plot(x,y,'r', label= "Target curve" )
    approx, = ax1.plot(0,0,'b--', label = "AG approx")
    ax1.set_title("Target curve vs GA approximation")
    ax1.legend()
    ax2.set_title("Fitness function per generation")
    #Algoritmo de ajuste de curva
    #obtener la poblacion inicial
    population = list(gen_population())
    #calculando los nuevos coeficientes
    for i in range(len(population)): #calculando los coeficientes por cada cromosoma
        #hicimos el preprocesado hasta aqui
        calc_coeficients(population[i]) #population actualiza los cromosomas divididos por el peso
    
    #calcular la funcion de aptitud para cada caso
    ap_func = list()
    for i in range(len(population)):
        ap_func.append(func_aptitude(population[i]))
    
    #torneo padres y madres
    #primero vamos a hacer el torneo una vez
    sons_population = list()
    lowest_err_per_tournament = list()
    for i in range(num_generations):
        #print("----------------------------------------- Generacion: ", i+1, "-----------------------------------------")

        #calc_coeficients(population[i]) #population actualiza los cromosomas divididos por el peso
        for j in range(tournaments_per_generation):
            #print("-----------------------------------------Torneo ", j + 1,"-----------------------------------------")
            cont_torn_fathers = select_contenders()
            cont_torn_mothers = select_contenders()
            winner_father, winner_err_father = select_individual_with_lowest_err(cont_torn_fathers)
            winner_mother, winner_err_mother = select_individual_with_lowest_err(cont_torn_mothers)
            #guardando el menor error de cada torneo
            lowest_err_per_tournament.append(winner_err_father)
            lowest_err_per_tournament.append(winner_err_mother)
            #aplicando la  al ganador
            son_father, son_mother = apply_reproduction_operator(winner_father, winner_mother)
            sons_population.append(son_father)
            sons_population.append(son_mother)
        
        #De la generacion selecciono al mejor individuo
        best_ind_of_gen = get_fittest_of_generation(ap_func)
        #print("Mejor Individuo de la generacion:",best_ind_of_gen ,": ",population[best_ind_of_gen], "Error: ",ap_func[best_ind_of_gen])
        #graficamos la mejor curva de la generacion
        best_ind_params = population[best_ind_of_gen].copy()
        best_curve_gen = calc_curve(x, best_ind_params[0], best_ind_params[1], best_ind_params[2], best_ind_params[3], best_ind_params[4], best_ind_params[5], best_ind_params[6])
        
        approx.set_xdata(x)
        approx.set_ydata(best_curve_gen)
        fig.canvas.draw()
        fig.canvas.flush_events()

        ax2.plot(i+1, ap_func[best_ind_of_gen], '-ro')
        ax1.set_xlabel("Coeficients: " + str(population[best_ind_of_gen]))
        ax2.set_xlabel("Error: " + str(ap_func[best_ind_of_gen]))
        
        plt.pause(0.000001)


        #FALTA EL CALCULO DELA FUNCION DE APTITUD DE CADA GENERACION Y LA GENERACION DE NUEVOS COEFICIENTES POR GENERACION
        #Pasando la generacion de hijos a la matriz de poblacion
        population.clear()
        population = sons_population.copy()
        sons_population.clear()

        if(mutation):
        #Elitismo se encarga de asegurar que ciertos individuos vivan
        #Tiene la ventaja de hacer el algoritmo genertico mas rapido pero puiede provocar que se atoren
            #apply_elitism()
        #Mutacion NOT a nivel bit despues de obtener los hijos
        #solo se hara con un cierto porcentaje de la poblacion (entre 3% y el 5%)
        #esto se hace para tener mayor diversidad genetica
            apply_mutation()

        #calculando de nuevo la funcion de aptitud
        ap_func.clear()
        for k in range(len(population)):
            ap_func.append(func_aptitude(population[k]))