#include "popolazione.h"
#include <vector>

using namespace std;

Popolazione::Popolazione(int size, Random& rnd) {
    
    size_popolazione = size;
    popolazione.reserve(size);
    InitializePopolazione(rnd);
};

void Popolazione::InitializePopolazione(Random& rnd) { //hai modificato il ciclo for

    for (int i = 0; i < size_popolazione; ++i) {
        Individuo I(110, rnd);
        popolazione.push_back(I);
    }
};

void Popolazione::SwapAll(Popolazione& P, Random& rnd){
    for (int i = 0; i < size_popolazione; i++) {
        P.GetIndividuo(i).Swap(rnd); //Swap metodo di individuo (P.GetIndividuo(i) è un individuo)
    }
};

void Popolazione::Sort(Cities& C){
    for (int i = 0; i < size_popolazione - 1; i++) {
        
        for (int j = 0; j < size_popolazione - i - 1; j++) {
            
            if (popolazione[j].CostFunction(C) < popolazione[j+1].CostFunction(C)) {
                // Scambio i valori se l'elemento corrente è maggiore del successivo
                Individuo temp = popolazione[j];
                popolazione[j] = popolazione[j + 1];
                popolazione[j + 1] = temp;
            }
        }
    }
};

Individuo& Popolazione::GetIndividuo(int index){
    if (index >= 0 and index < size_popolazione) {
        return popolazione[index];
    } else {
        cerr << "Indice non valido." << endl;
        exit(1);
    }
};

vector <Individuo> Popolazione::Crossover(int index1, int index2, Random& rnd){
    vector <Individuo> cross;
    // Cromosomi genitori
    Individuo parent1 = popolazione[index1];
    Individuo parent2 = popolazione[index2];
    
    // Genero un punto di crossover casuale
    int crossoverPoint = int(rnd.Rannyu() * (parent1.GetSize() - 1)) + 1;
    
    // Definisco i figli
    Individuo child1(110, rnd);
    Individuo child2(110, rnd);
    
    // Riempio i figli con i primi crossoverPoint valori dei genitori
    for (int i = 0; i < crossoverPoint; i++) {
        child1.SetValue(i, parent1.GetValue(i));
        child2.SetValue(i, parent2.GetValue(i));
    }
    for (int i = crossoverPoint; i < child1.GetSize(); i++) {
        child1.SetValue(i, -1);
        child2.SetValue(i, -1);
    }
    
    // Riempio i figli con i valori mancanti presi in ordine dai genitori
    // Va bene usare anche parent2.GetSize()
    int i = 1;
    int j = crossoverPoint;
    int k = crossoverPoint;
    //cout << "crossover = " << crossoverPoint << endl;
    //CROSSOVER POINT OK
    do{
        if(in(parent2.GetValue(i), child1) == false){
            child1.SetValue(j, parent2.GetValue(i));
            j++;
        }
        if(in(parent1.GetValue(i), child2) == false){
            child2.SetValue(k, parent1.GetValue(i));
            k++;
        }
        i++;
    }while(j < parent1.GetSize() or k < parent1.GetSize());
    //cout << "J = " << j << " K = " << k << " I = " << i << endl;
    //INDICI J, K, I COERENTI
    
    cross.push_back(child1);
    cross.push_back(child2);
    return cross;
};

void Popolazione::Set_Individuo(std::vector<int> vec, int index){
    popolazione[index].SetVector(vec);
};

/*void Popolazione::NewPopulation(Popolazione newPopolazione) {
    
    if (newChromosomes.size() == numChromosomes and statament == true) chromosomes = newChromosomes;
    else cerr << "Old population and new population sizes don't match!" << endl;
}*/
