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

using namespace std;

class Cities {

private:
    
    int size_cities; //Numero città
    vector<vector<double>> cities; //coordinate x e y delle 34 città
    void Generate (Random& rnd); //Metodo che genera le città
    
public:
    
    Cities(int size, Random& rnd); //costruttore
    vector <double> GetCity(int index); //Coordinate della città index
    int GetSize() {return size_cities;}; //Numero città
    void SetCities(vector <vector<double>> a); //Setto le città generate in cities
    void Print(string& filename); //Stampo le città generate
    
    
    
};




#endif // __cities__
