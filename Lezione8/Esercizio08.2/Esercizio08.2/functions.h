#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "random.h"
#include <cmath>
#include <vector>

using namespace std;

double error(double,double, int);//calcolo dell'errore
double prob(double, double, double);//probabilità data dalla funzione d'onda al quadrato
double H_energy(double, double, double);//calcolo energia
vector <double> Metropolis (Random&, double, double, double, double, double, int, int);//implementazione del calcolo dell'energia e dell'errore ad un passo successivo. sfrutta metropolis
