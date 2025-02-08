#ifndef __cities__
#define __cities__

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <stdlib.h> //exit
#include "random.h"
#include <cmath>
#include <vector>
#include <algorithm>
#include <mpi.h>

using namespace std;

class Cities {

private:
    
    int size_cities;//Numero città
    vector<vector<double>> cities;//coordinate x e y delle 110 città
    
public:
    
    Cities(){};
    Cities(int size, Random& rnd); //costruttore
    Cities(int size);//costruttore
    void Generate (Random& rnd); //Metodo che genera/legge le città
    vector <double> GetCity(int index);//Coordinate della città index
    vector <vector<double>> Get_all();//restituisce vettore di coordinate
    int GetSize() {return size_cities;};//Numero città
    void SetCities(vector <vector<double>> a);//Setto le città generate/lette in cities
    void Print(string& filename);//Stampo le città generate/lette
    
    
    
};




#endif // __cities__
