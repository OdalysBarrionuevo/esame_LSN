#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "random.h"
#include <cmath>
#include <vector>

using namespace std;

double error(vector <double>, vector <double>, int); //calcolo errore
double prob(double, double, double); //probabilità data dal modulo quadro della funzione d'onda
double H_energy(double, double, double); //calcolo dell'energia
vector <vector <double>> Metropolis (Random&, ofstream&, double, double, double, double, double, int, int); //implementazione dell'algoritmo di metropolis che restituisce un vettore di medie istantanee e vettore delle medie al quadrato
