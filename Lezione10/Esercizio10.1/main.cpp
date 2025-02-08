#include "cities.h"
#include "individuo.h"
#include "popolazione.h"

using namespace std;

void ShuffleVector(vector<vector<int>>& vec, Random& rnd);

int main(int argc, char *argv[]) {
    
    //per usare la programmazione parallela
    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    Random rnd;//oggetto di tipo Random
    int seed[4];
    int p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16;
    ifstream Primes("Primes");
    if (Primes.is_open()) {
        Primes >> p1 >> p2 >> p3 >> p4 >> p5 >> p6 >> p7 >> p8 >> p9 >> p10 >> p11 >> p12 >> p13 >> p14 >> p15 >> p16;
      } else
        cerr << "PROBLEM: Unable to open Primes" << endl;
      Primes.close();

      ifstream input("seed.in");
      string property;
      if (input.is_open()) {
        while (!input.eof()) {
              input >> property;
              if (property == "RANDOMSEED") {//generatore di numeri casuali diverso per ogni nodo
                  input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
                  if(rank == 0) rnd.SetRandom(seed,p1 , p2);
                  if(rank == 1) rnd.SetRandom(seed,p3 , p4);
                  if(rank == 2) rnd.SetRandom(seed,p5 , p6);
                  if(rank == 3) rnd.SetRandom(seed,p7 , p8);
                  if(rank == 4) rnd.SetRandom(seed,p9 , p10);
                  if(rank == 5) rnd.SetRandom(seed,p11 , p12);
                  if(rank == 6) rnd.SetRandom(seed,p13 , p14);
                  if(rank == 7) rnd.SetRandom(seed,p15 , p16);
              }
        }
        input.close();
      } else
        cerr << "PROBLEM: Unable to open seed.in" << endl;
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Implemento l'algoritmo genetico per risolvere il travelling salesman problem sui capoluighi di provincia italiani
    Cities C(110);//costruttore di 110 citta
    int City_size = 110; //numero città
    if(rank == 0){//salvo le città solo nel rank 0
        C.Generate(rnd); //genero/leggo città
        string filename = "Città_Italia_MPI_NC.txt"; //Nome file città
        C.Print(filename); //Metodo per stampare le citta file
    }
    //appiattisco il vettore contenente le coordinate per poi condividerlo con gli altri nodi
    vector<double> cityCoordinates(City_size * 2);
    if(rank == 0) {
        for(int i = 0; i < City_size; i++) {
            cityCoordinates[2*i] = C.Get_all()[i][0];
            cityCoordinates[2*i+1] = C.Get_all()[i][1];
        }
    }
    //condivisione delle città con gli altri nodi in modo tale che possano lavorare sullo stesso problema
    MPI_Bcast(cityCoordinates.data(), City_size * 2, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    //il vettore delle città deve essere in formato matriciale
    vector <vector<double>> cities_copy(110, vector<double> (2));
    if(rank != 0 ) {
        for(int i = 0; i < City_size; i++) {
            cities_copy[i][0] = cityCoordinates[2*i];
            cities_copy[i][1] = cityCoordinates[2*i+1];
        }
        C.SetCities(cities_copy);
    }
    
    Popolazione P(1000, rnd); //Creo un oggetto di tipo Popolazione
    
    int ind1, ind2, mut; //indici che indicano due individui di popolazione e tipo di mutazione
    double prob; //probabilità di crossover e mutazioni
    double _Min, Mean; //minimo per ogni nodo e media
    double True_min;//il minimo tra tutti i nodi
    
    ofstream min, mean;//file di minimo e media
    Individuo child1(110, rnd), child2(110, rnd); //individui per la nuova popolazione
    Popolazione newPopolazione(1000, rnd); //Vettore in cui salvo i nuovi individui di volta in volta
    
    if (rank == 0){
        min.open("min_cost_Italia_MPI_NC.txt");
        mean.open("mean_cost_Italia_MPI_NC.txt");
    }
    
    // Implemento un ciclo di 800 iterazioni dell'algoritmo genetico
    for(int i = 0; i < 800; i++){
        P.Sort(C);// Metto in ordine gli individui in base alla loro CostFunction
        
        vector <double> Min(8, 0);//vettore di minimi tra tutti i nodi
        _Min = (P.GetIndividuo(P.GetSize()-1)).CostFunction(C); //valore minimo
        
        //condivisione del minimo di ogni nodo col nodo 0
        MPI_Gather(&_Min, 1, MPI_DOUBLE, Min.data(), 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        
        // Salvo la cost function minima e la media delle cost function della (i+1)-esima generazione
        if(rank == 0){
            auto true_min = min_element(Min.begin(), Min.end());
            min << i+1 << "\t" << *true_min << endl;
            True_min = *true_min;
            Mean = 0.0;
            
            for(int j = P.GetSize()/2; j < P.GetSize(); j++){
                Mean += ((P.GetIndividuo(j)).CostFunction(C))/(double)(P.GetSize()/2.);
            }
            mean << i+1 << "\t" << Mean << endl;
        }
        
        //condivido il minimo con tutti i nodi
        MPI_Bcast(&True_min, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        
        //ogni due generazioni faccio uno scambio casuale dei migliori 350 individui tra tutti i nodi
        int N_migr = 2;
        if(i % N_migr == 0){
            
            vector <vector<int>> last125 (350, vector<int> (110)); //vettore degli ultimi 350
            vector <vector<int>> gather125 (350*size, vector<int> (110)); //vettore che verrà usato per radunare i 350 migliori di tutti i nodi
            
            //riempio last125 con i migliori 350 individui
            for (int s = 0; s < 350; ++s) {
                last125[s] = P.GetIndividuo(P.GetSize() - s - 1).Get_listCities();
            }
        
            //appiattimento per la condivisione
            vector <int> last125_flat(350*110);
            for (int r = 0; r < 350; ++r) {
                for (int g = 0; g < 110; ++g) {
                    last125_flat[r * 110 + g] = last125[r][g];
                }
            }
            
            //condivido i migliori 350 individui di tutti i nodi col nodo 0
            vector <int> gather125_flat(350*size*110);
            MPI_Gather(last125_flat.data(), 350*110, MPI_INTEGER, gather125_flat.data(), 350*110, MPI_INTEGER, 0, MPI_COMM_WORLD);
            
            //il vettore degli individui deve essere in formato matriciale
            if(rank == 0){
                for (int c = 0; c < 350 * 8; ++c) {
                    for (int d = 0; d < 110; ++d) {
                        gather125[c][d] = gather125_flat[c * 110 + d];
                    }
                }
                ShuffleVector(gather125, rnd); //scambio casuale tra tutti i 350 migliori individui dei nodi
                
                //appiattimento per la condivisione
                for (int n = 0; n < 350*8; ++n) {
                    for (int m = 0; m < 110; ++m) {
                        gather125_flat[n * 110 + m] = gather125[n][m];
                    }
                }
            }
            
            //condivido il mescolamento dei migliori 350 individui di tutti i nodi
            MPI_Bcast(gather125_flat.data(), 350*size*110, MPI_INTEGER, 0, MPI_COMM_WORLD);
            
            //il vettore degli individui deve essere in formato matriciale
            for (int y = 0; y < 350 * 8; ++y) {
                for (int z = 0; z < 110; ++z) {
                    gather125[y][z] = gather125_flat[y * 110 + z];
                }
            }
            
            //ridistribuisco gli individui mescolati alla popolazione di ogni nodo
            for(int h = 0; h < 350; h++){
                P.Set_Individuo(gather125[350*rank+h], P.GetSize() - 350 + h);
            }
            P.Sort(C); //riordino la popolazione
        }
        
        // Implemento un ciclo in modo tale da creare una nuova popolazione di 1000 individui
        for (int j = 0; j < P.GetSize()/2; j++){
            // Scelgo ind1, ind2 in modo tale che è molto probabile che si scelgano gli individui migliori (con cost function bassa) e mi assicuro che siano diversi
            do{
                if (rank == 0){
                    ind1 = int(pow(rnd.Rannyu(), (0.05)) * P.GetSize());
                    ind2 = int(pow(rnd.Rannyu(), (0.05)) * P.GetSize());
                }
                if (rank == 1){
                    ind1 = int(pow(rnd.Rannyu(), (0.1)) * P.GetSize());
                    ind2 = int(pow(rnd.Rannyu(), (0.1)) * P.GetSize());
                }
                if (rank == 2){
                    ind1 = int(pow(rnd.Rannyu(), (0.15)) * P.GetSize());
                    ind2 = int(pow(rnd.Rannyu(), (0.15)) * P.GetSize());
                }
                if (rank == 3){
                    ind1 = int(pow(rnd.Rannyu(), (0.2)) * P.GetSize());
                    ind2 = int(pow(rnd.Rannyu(), (0.2)) * P.GetSize());
                }
                if (rank == 4){
                    ind1 = int(pow(rnd.Rannyu(), (0.25)) * P.GetSize());
                    ind2 = int(pow(rnd.Rannyu(), (0.25)) * P.GetSize());
                }
                if (rank == 5){
                    ind1 = int(pow(rnd.Rannyu(), (0.3)) * P.GetSize());
                    ind2 = int(pow(rnd.Rannyu(), (0.3)) * P.GetSize());
                }
                if (rank == 6){
                    ind1 = int(pow(rnd.Rannyu(), (0.35)) * P.GetSize());
                    ind2 = int(pow(rnd.Rannyu(), (0.35)) * P.GetSize());
                }
                if (rank == 7){
                    ind1 = int(pow(rnd.Rannyu(), (0.4)) * P.GetSize());
                    ind2 = int(pow(rnd.Rannyu(), (0.4)) * P.GetSize());
                }
            }while(ind1 == ind2);
            
            prob = rnd.Rannyu(); //Probabilità che uso per crossover e mutazioni
            mut = (int)(rnd.Rannyu(1, 5)); //Scelta casuale del tipo di mutazione
            
            // Eseguo un crossover fra i due individui con indici ind1, ind2 con probabilità 0.95
            if(prob < 0.95){
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
        //se il minimo della nuova popolazione è più grande della vecchia allora mi assicuro di conservare il più piccolo tra i due
        if((newPopolazione.GetIndividuo(P.GetSize()-1)).CostFunction(C) <= True_min){
            P = newPopolazione;
            newPopolazione.Clear();
        }else{
            Individuo TrueMin = P.GetIndividuo(P.GetSize()-1);
            P = newPopolazione;
            P.GetIndividuo(P.GetSize()-1) = TrueMin;
            newPopolazione.Clear();
        }
    }
    if(rank == 0){
        min.close();
        mean.close();
    }
    
    // Ordino ancora gli individui e scelgo l'ultimo individuo come soluzione del commesso viaggiatore e stampo su file
    if(rank == 0){
        P.Sort(C);
        int best_index = P.GetSize() - 1;
        Individuo best_individuo = P.GetIndividuo(best_index);
        vector <vector<double>> solution = best_individuo.GetCities(C);
        Cities newCities(110, rnd);
        newCities.SetCities(solution);
        string soluzione = "Soluzione_Italia_MPI_NC.txt";
        newCities.Print(soluzione);
        //stampo a video l'ordine degli indici delle città del miglior individuo trovato
        for (int f = 0; f < best_individuo.GetSize(); f++){
            cout << best_individuo.GetValue(f) << endl;
        }
    }
        
    rnd.SaveSeed();
    MPI_Finalize();
    return 0;
}

//scambio casuale degli elementi di un vettore
void ShuffleVector(std::vector<std::vector<int>>& vec, Random& rnd) {
    int n = vec.size();
    for (int i = n - 1; i > 0; --i) {
        int j = static_cast<int>(rnd.Rannyu(0, i + 1));
        std::swap(vec[i], vec[j]);
    }
};
