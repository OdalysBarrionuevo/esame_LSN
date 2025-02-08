#ifndef __individuo__
#define __individuo__

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "random.h"
#include <cmath>
#include <vector>
#include "cities.h"

using namespace std;

class Individuo {
    
// Funzione che calcola la distanza tra due citta
double dist(const vector<double>& v1, const vector<double>& v2) {
    double dx = v2[0] - v1[0];
    double dy = v2[1] - v1[1];
    return sqrt(dx*dx + dy*dy);
}

private:
    int size_individuo; // Numero delle città
    vector<int> individuo; //Elenco di indici che rappresenta l'ordine delle città da percorrere (con la prima città fissata)
    void Fill(Random& rnd); //Genero gli indici casualmente per ogni individuo


public:
    Individuo (int size, Random& rnd); //costruttore
    void Check(); //Controllo che gli individuo rispettino le B.C.
    vector <vector<double>> GetCities (Cities& C); //Estraggo le coordinate di un individuo
    void Swap(Random& rnd); //scambio di due valori casuali dell'individuo (escludendo la prima città)
    double CostFunction(Cities& C); //valore della distanza totale dell'individuo (L1)
    int GetSize(){return size_individuo;}; //Numero città
    void PrintIndividuo(); //stampa dei valori dell'individuo
    void SetValue(int index, int value){individuo[index] = value;}; //Impongo che un certo indice di individuo contenga un certo valore
    int GetValue(int index){return individuo[index];}; //Estraggo il valore della posizione index di un individuo
    //Mutazioni
    void Mutation1(Random& rnd);
    void Mutation2(Random& rnd);
    void Mutation3(Random& rnd);
    void Mutation4(Random& rnd);
};

#endif // __individuo__
