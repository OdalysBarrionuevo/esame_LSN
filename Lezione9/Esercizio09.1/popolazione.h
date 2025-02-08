#ifndef __popolazione__
#define __popolazione__

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "random.h"
#include <cmath>
#include <vector>
#include "individuo.h"
#include "cities.h"

using namespace std;

class Popolazione {
    
// Funzione che verifica se il termine 'element' sia presente nell'individuo I
bool in(const int element, Individuo I) {
    for(unsigned int i=0; i < I.GetSize(); i++) if(element == I.GetValue(i)) return true;
    return false;
};

private:
    int size_popolazione; //dimensione popolazione
    vector <Individuo> popolazione; //popolazione di individui
    void InitializePopolazione(Random& rnd); //usato dal costruttore per generare gli individui

public:
    Popolazione(int size, Random& rnd); //costruttore
    void SwapAll(Popolazione& P, Random& rnd); //Metodo per ottenere individui diversi tra di loro
    void Sort(Cities& C); //Metto in ordine gli individui di una popolazione in base alla cost function
    Individuo& GetIndividuo(int index); //Estraggo l'individuo index dalla popolazione
    int GetSize(){return size_popolazione;}; //Dimensione popolazione
    vector <Individuo> Crossover(int index1, int index2, Random& rnd); // Metodo di riproduzione della popolazione: restituisce ogni volta un vettore con due componenti di tipo Individuo
    void SetIndividuo(Individuo I){popolazione.push_back(I);}; //Aggiungo un individuo alla popolazione
    void Clear(){ popolazione.clear();}; //Svuoto il vettore popolazione
    
    


};



#endif //__popolazione__
