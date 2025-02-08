#include "cities.h"
#include "individuo.h"
#include "popolazione.h"

using namespace std;

int main(int argc, char *argv[]) {

    Random rnd;
    int seed[4];
    int p1, p2;
    ifstream Primes("Primes");
    if (Primes.is_open()) {
        Primes >> p1 >> p2;
      } else
        cerr << "PROBLEM: Unable to open Primes" << endl;
      Primes.close();

      ifstream input("seed.in");
      string property;
      if (input.is_open()) {
        while (!input.eof()) {
              input >> property;
              if (property == "RANDOMSEED") {
            input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
            rnd.SetRandom(seed, p1, p2);
              }
        }
        input.close();
      } else
        cerr << "PROBLEM: Unable to open seed.in" << endl;
    
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Implemento l'algoritmo genetico per risolvere il travelling salesman problem sulla circonferenza/quadrato

    Cities C(34, rnd); //Genero 34 città
    string filename = "Città_circonferenza.txt"; //Nome file
    C.Print(filename); //Metodo per stampare le citta generate su file
    
    Popolazione P(1000, rnd); //Creo un oggetto di tipo Popolazione
    
    int ind1, ind2, mut; //Indici dei figli e scelta casuale del tipo di mutazione
    double Min, Mean, prob; //Minimo, media e probabilità di crossover
    Individuo child1(34, rnd), child2(34, rnd); //Coppia di figli (oggetti di tipo individuo)
    Popolazione newPopolazione(1000, rnd); //Vettore in cui salvo i nuovi individui di volta in volta
    ofstream min, mean; //File per salvare il minimo e la media
    min.open("min_cost_circonferenza.txt");
    mean.open("mean_cost_circonferenza.txt");
    
    // Implemento un ciclo di 500 iterazioni dell'algoritmo genetico
    for(int i = 0; i < 500; i++){
        
        P.Sort(C);//Metto in ordine gli individui in base alla loro CostFunction
        
        Min = (P.GetIndividuo(P.GetSize()-1)).CostFunction(C); // Minimo della popolazione corrente
        
        // Salvo la cost function minima
        min << i+1 << "\t" << Min << endl;
        
        //Media fatta sulla metà migliore della popolazione (per ogni generazione)
        Mean = 0.0;
        for(int j = P.GetSize()/2; j < P.GetSize(); j++){
            Mean += ((P.GetIndividuo(j)).CostFunction(C))/(double)(P.GetSize()/2.);
        }
        mean << i+1 << "\t" << Mean << endl;
        
        
        // Implemento un ciclo in modo tale da creare una nuova popolazione di 1000 individui
        for (int j = 0; j < P.GetSize()/2; j++){
            // Scelgo ind1, ind2 in modo tale che è molto probabile che si scelgano gli individui migliori (con cost function bassa) e mi assicuro che siano diversi
            do{
                ind1 = int(pow(rnd.Rannyu(), 0.3) * P.GetSize()); //CONTROLLA
                ind2 = int(pow(rnd.Rannyu(), 0.3) * P.GetSize());
            }while(ind1 == ind2);
            
            prob = rnd.Rannyu(); //Probabilità che uso per crossover e mutazioni
            mut = (int)(rnd.Rannyu(1, 5)); //Scelta casuale del tipo di mutazione
            
            // Eseguo un crossover fra i due individui con indici ind1, ind2 con probabilità 0.9
            if(prob < 0.9){
                vector <Individuo> children = P.Crossover(ind1, ind2, rnd);
                child1 = children[0];
                child2 = children[1];
            }else{
                child1 = P.GetIndividuo(ind1);
                child2 = P.GetIndividuo(ind2);
            }
            
            // Eseguo una mutazione sugli individui ind1 e ind2 con probabilità 0.1
            if(prob < 0.1 and mut == 1){
                child1.Mutation1(rnd);
                child2.Mutation1(rnd);
            }
            
            if(prob < 0.1 and mut == 2){
                child1.Mutation2(rnd);
                child2.Mutation2(rnd);
            }
            if(prob < 0.1 and mut == 3){
                child1.Mutation3(rnd);
                child2.Mutation3(rnd);
            }
            if(prob < 0.1 and mut == 4){
                child1.Mutation4(rnd);
                child2.Mutation4(rnd);
            }
            
            // Controllo la validità dei nuovi individui
            child1.Check();
            child2.Check();
            
            //Aggiungo i due nuovi individui alla nuova popolazione
            newPopolazione.SetIndividuo(child1);
            newPopolazione.SetIndividuo(child2);
            
        }
        //Se il minimo della nuova popolazione è più piccolo della vecchia allora sostituico completamente la vecchia popolazione con la nuova, altrimenti la sostituzione la faccio ugualmente ma salvando il minimo della vecchia popolazione
        if((newPopolazione.GetIndividuo(P.GetSize()-1)).CostFunction(C) <= Min){
            P = newPopolazione;
            newPopolazione.Clear();
        }else{
            Individuo TrueMin = P.GetIndividuo(P.GetSize()-1);
            P = newPopolazione;
            P.GetIndividuo(P.GetSize()-1) = TrueMin;
            newPopolazione.Clear();
        }
    }
    min.close();
    mean.close();
    
    // Ordino ancora gli individui e scelgo l'ultimo individuo come soluzione del commesso viaggiatore e stampo su file
    P.Sort(C);
    int best_index = P.GetSize() - 1;
    Individuo best_individuo = P.GetIndividuo(best_index);
    vector <vector<double>> solution = best_individuo.GetCities(C);
    Cities newCities(34, rnd);
    newCities.SetCities(solution);
    string soluzione = "Soluzione_circonferenza.txt";
    newCities.Print(soluzione);
    
    rnd.SaveSeed();
    return 0;
}
