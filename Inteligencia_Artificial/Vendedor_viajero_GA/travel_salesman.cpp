#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <stdlib.h> 
#include <utility>
#include <unordered_map>
#include <cmath>
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

#define N 100
#define M 20

int RandomNumGenerator(int r,int n = 0, int offset = 0);


int RandomNumGenerator(int r, int n, int offset) {
    unsigned seed = std::chrono::system_clock::now()
                        .time_since_epoch()
                        .count();
    //Pasando la seed al generador de numeros aleatorios
    srand(seed + n);
    return (rand() % r + offset);
}


class Population {
    public:
        Population(int n, int m) : population (n, std::vector<int>(m)), chrom_(n) {
            GenerateRandomPopulation();
        }
        void DisplayMatrix();
        std::vector<int>& GetRouteAt(int at);
        int GetNumOfChrom();
        void UpdatePopulation(const std::vector<std::vector<int>>& new_generation);
    private:
        void GenerateRandomPopulation();
        bool IsRepeated(std::vector<int>& vec, const int num);

        int chrom_;
        std::vector<std::vector<int>> population;
};

std::vector<int>& Population::GetRouteAt(int at) {
    return population[at];
}

void Population::UpdatePopulation(const std::vector<std::vector<int>>& new_generation) {
    population.clear();
    //std::cout << "Ppulation dimensiones, filas: " << GetNumOfChrom() << " columnas: " << population[0].size() << std::endl;
    population = new_generation;
}

void Population::DisplayMatrix() {
    for (int i = 0; i < population.size() ; i++) {
        for (int j = 0; j < population[i].size() ; j++) {
            std::cout << population[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

bool Population::IsRepeated(std::vector<int>& vec, const int num) {
    if(std::find(vec.begin(), vec.end(), num) != vec.end()){
        // el numero esta repetido
        return true;
    }
    // no es un numero repetido
    return false;
}

int Population::GetNumOfChrom() {
    return chrom_;
}

void Population::GenerateRandomPopulation() {
    for (int i = 0; i < population.size() ; i++) {
        for (int j = 0; j < population[i].size() ; j++) {
            int randNum = RandomNumGenerator(20,0,1);
            while(IsRepeated(population[i], randNum)) {
                randNum = RandomNumGenerator(20,0,1);
            }
            population[i][j] = randNum;
        }
    }
} 

class Cities : public Population {
    public:
        Cities (int n, int m) : Population(n,m), num_cities(m), route_distance(n,0.0){
            CreateCitiesMap();
            CalcRouteDistance();
        }
        void DisplayCitiesMap();
        void DisplayRoutesDistance();
        void CalcRouteDistance();
        float GetDistanceAt(int at);
        int GetNumCities();
        void UpdateRouteDistance();
        std::unordered_map<int, std::pair<int,int>>& GetLocations();
    private:
        void CreateCitiesMap();
        std::unordered_map<int, std::pair<int,int>> city_location;
        std::vector<float> route_distance;
        int num_cities;
};

std::unordered_map<int, std::pair<int,int>>& Cities::GetLocations() {
    return city_location;
}

void Cities::UpdateRouteDistance() {
    //std::cout << "Tamaño del vector de rutas: " << route_distance.size() << std::endl;
    route_distance.clear();
    route_distance.resize(GetNumOfChrom(),0.0);
    //Calculando la distancia de las rutas de una nueva generacion
    CalcRouteDistance();

}

float Cities::GetDistanceAt(int at) {
    return route_distance[at];
}

int Cities::GetNumCities() {
    return num_cities;
}

void Cities::DisplayRoutesDistance() {
    for (int i = 0; i < route_distance.size() ; i++) {
        std::cout << "Route " << i+1 << ": " << route_distance[i] << std::endl;
    }
}

void Cities::DisplayCitiesMap() {
    for(int i = 1; i <= num_cities; i++) {
        std::cout << "city " << i << ": (" <<  city_location[i].first << ", " << city_location[i].second << ")" << std::endl;
    }
}

void Cities::CreateCitiesMap() {
    int x, y;
    for(int i = 1; i <= num_cities; i++) {
        x = RandomNumGenerator(20,i,1);
        y = RandomNumGenerator(20,x,1);
        city_location[i] = std::make_pair(x,y); 
    } 
}

void Cities::CalcRouteDistance() {
    for(int i = 0; i < route_distance.size(); i++) {
        const std::vector<int>& route = GetRouteAt(i);
        for (int j = 1; j <= route.size() ; j++) {
            float xDist = std::abs(city_location[route[j-1]].first - city_location[route[j]].first);
            float yDist = std::abs(city_location[route[j-1]].second - city_location[route[j]].first);
            route_distance[i] += std::sqrt(std::pow(xDist,2.0) + std::pow(yDist,2.0));
        }
    }
}


class Tournament {
    public:
        Tournament(Cities& cities) : cities_(cities){}
        void DisplayConstestantsInfo();
        void PlayTournament(int times_per_generation, int generations);

        //Funciones para graficar
        void PlotCitiesCoordenates();
    private:
        int SelectFittestContestant();
        int SelectFittestOfGeneration();
        std::vector<int> PickContenders();
        std::vector<int> ApplyMutations(std::vector<int> father);
        void InvertMutation(std::vector<int>& father);
        void SwapMutation(std::vector<int>& father);
        void TournamentAlgorithm(int times);
        bool IsRepeatedContestant(std::vector<int>& vec, int num_check);
        bool ShortestRoute(int c1, int c2);
        Cities cities_;
};

void Tournament::DisplayConstestantsInfo() {
    //std::cout << "--------------------------------- Population (Routes) Map ---------------------------------" << std::endl;
    //cities_.DisplayMatrix();
    //std::cout << "--------------------------------- Cities Coordenates ---------------------------------" << std::endl;
    //cities_.DisplayCitiesMap();
    //std::cout << "--------------------------------- Routes Max Distance ---------------------------------" << std::endl;
    //cities_.DisplayRoutesDistance();
}

std::vector<int> Tournament::PickContenders() {
    //Los contendientes se escogeran en grupos de 5
    std::vector<int> contestant_index(5,-1);
    contestant_index[0] = RandomNumGenerator(100);
    //Generamos un numero aleatorio entre el 0 y 99 para escoger a los cromosomas que van a participar en el torneo
    //No se puede repetir el mismo index para la misma ronda de participantes
    for(int i = 1; i < contestant_index.size() ; i++) {
        int rand_num = RandomNumGenerator(100);
        while(IsRepeatedContestant(contestant_index, rand_num)) {
            rand_num = RandomNumGenerator(100);
        }
        contestant_index[i] = rand_num;
    }
    //std::cout << "Contendientes: " << std::endl;
    //for(auto c : contestant_index) {
    //    std::cout << "distancia de la ruta " << c + 1 << ": " << cities_.GetDistanceAt(c) << " "; 
    //}
    //std::cout << std::endl;
    return contestant_index;
}

bool Tournament::IsRepeatedContestant(std::vector<int>& vec,int num_check) {
    if(std::find(vec.begin(), vec.end(), num_check) != vec.end()) {
        return true;
    }
    return false;

}

int Tournament::SelectFittestContestant() {
    std::vector<int> contestants = PickContenders();

    int l = 0, r = contestants.size()-1;
    int winner;
    while(l < r) {
        if(ShortestRoute(contestants[l], contestants[r])) {
            winner = contestants[r];
            l++;

        }
        else {
            winner = contestants[l];
            r--;
        }
    }
    //std::cout << "Ruta ganadora: " << winner << ", distancia: " << cities_.GetDistanceAt(winner) << std::endl;
    return winner;
}

int Tournament::SelectFittestOfGeneration() {
    int fittest_of_generation;
    int l = 0, r = 99;
    while(l < r) {
        if(ShortestRoute(l, r)) {
            fittest_of_generation = r;
            l++;

        }
        else {
            fittest_of_generation = l;
            r--;
        }
    }
    return fittest_of_generation;
}

bool Tournament::ShortestRoute(int c1, int c2) {
    return (cities_.GetDistanceAt(c1) > cities_.GetDistanceAt(c2));
}


void Tournament::PlotCitiesCoordenates() {
    std::unordered_map<int, std::pair<int,int>> coordenates = cities_.GetLocations();
    std::vector<int> x, y;

    plt::figure_size(1200, 780);
    for(int i = 0; i < 20; i++) {
        x.push_back(coordenates[i].first); 
        y.push_back(coordenates[i].second);
    }
    plt::plot(x,y);
    plt::xlim(0, 1000*1000);
    plt::title("Mejor Ruta por generación");

    plt::show();
}


void Tournament::PlayTournament(int times_per_generation, int generations) {
    //Por generacion se jugaran 100 torneos con el 5% de la poblacion (es decir el 5% de 100)
    int generation_fittest;
    for(int i = 0; i < generations ; i++){
        std::cout << "Generation " << i + 1 << ":" << std::endl;
        TournamentAlgorithm(times_per_generation);
        generation_fittest = SelectFittestOfGeneration();
        std::cout << "\t\tMejor individuo de la generacion " << generation_fittest + 1 << ": " << cities_.GetDistanceAt(generation_fittest) <<"\n\t\tRuta: ";
        for(auto c: cities_.GetRouteAt(generation_fittest)) {
            std::cout << c << " "; 
        }
        std::cout << std::endl;
    }
    cities_.DisplayRoutesDistance();
    PlotCitiesCoordenates();
    //cities_.DisplayMatrix();
}

void Tournament::TournamentAlgorithm(int times) {
    std::vector<int> fathers(times);
    std::vector<std::vector<int>> new_generation(cities_.GetNumOfChrom(), std::vector<int>(cities_.GetNumCities()));
    //Seleccionando a los padres
    for (int i = 0; i < times ; i++){
        std::cout << "\tPlaying tournament " << i + 1 << "...";
        fathers[i] = SelectFittestContestant();
        std::cout << std::endl;
    }
    //applicando las mutaciones a cada cromosoma
    for (int i = 0; i < fathers.size() ; i++) {
        new_generation[i] = ApplyMutations(cities_.GetRouteAt(fathers[i]));
    }
    //una vez que se realizaron las mutaciones a cada padre es necesario actualizar la poblacion
    cities_.UpdatePopulation(new_generation);
    //Una vez actualizada la poblacion con la nueva generacion, se calculan las nuevas rutas
    cities_.UpdateRouteDistance();

}

std::vector<int> Tournament::ApplyMutations(std::vector<int> father) {
    //std::cout << "Individuo antes de la mutacion" << std::endl;
    //for(auto c: father) {
    //    std::cout << c << " ";
    //}
    //std::cout << std::endl;
    //Seleccionar la mutacion que vamos a aplicar
    if(RandomNumGenerator(2)) {
        //swap
        SwapMutation(father);
    }
    else {
        //invert string
        InvertMutation(father);
    }
    //std::cout << "Individuo despues de la mutacion" << std::endl;
    //for(auto c: father) {
    //    std::cout << c << " ";
    //}
    //std::cout << std::endl;
    return father;

}

void Tournament::InvertMutation(std::vector<int>& father) {
    int initial_pos = RandomNumGenerator(18, 0,1); //El segmento no puede ser 0
    int final_pos; // La posicion final es mayor que la posicion inicial y menor que 19 (el ultimo elemento del array)
    //si posicion inicial es 18 entonces la posicion final es 0
    if(initial_pos == 18) {
        final_pos = 0;
    }
    else 
    {
        final_pos = RandomNumGenerator(18 - initial_pos, 0,1);
    }
    //std::cout << "Posicion inicial: "<<initial_pos << " Posicion final " << final_pos << std::endl;
    std::vector<int> inversion_range(father.begin() + initial_pos, father.end() - final_pos);
    
    //for(auto c: inversion_range) {
    //    std::cout << c << " ";
    //}
    //std::cout << std::endl;
    int i = inversion_range.size() - 1;
    while(i >= 0) {
        father[initial_pos] = inversion_range[i];
        --i;
        ++initial_pos;
    }
}

void Tournament::SwapMutation(std::vector<int>& father) {
    int segment_size = RandomNumGenerator(10, 0, 1); //El segmento no puede ser 0 ni el 10
    int first_pos, second_pos; 
    //Si el segmento es 10 entonces la primera poscion iniciaal debe ser 0
    //Si el segmento es 10 entonces la segunda poscion iniciaal debe ser 10
    if(segment_size == 10) {
        segment_size--;
        first_pos = 0;
        second_pos = 10;
    } 
    else {
        first_pos = RandomNumGenerator(10 - segment_size);
        second_pos = RandomNumGenerator(10 - segment_size, first_pos) +10;
    }
    //std::cout << segment_size << " " << first_pos << " " << second_pos << std::endl;
    std::vector<int> first_range(father.begin() + first_pos, father.begin() + first_pos + segment_size);
    std::vector<int> second_range(father.begin() + second_pos, father.begin() + second_pos + segment_size);
    int i, j = 0;
    while(i < first_range.size() && j < second_range.size()) {
            father[first_pos] = second_range[j];
            father[second_pos] = first_range[i];
            j++;
            i++;
            first_pos++;
            second_pos++;
        }
    
    
}


int main() {
    //No funciona con N!=100 porque la funcion PickContenders esta hardcodeada a valores de 1000
    Cities cities (N,M);
    Tournament tournament(cities);
    //tournament.DisplayConstestantsInfo();
    tournament.PlayTournament(100,100);
    //una vez que el torneo termino es tiempo de 
    tournament.PlotCitiesCoordenates();
    plt::show();
    return 0;
}