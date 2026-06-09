#include <iostream>
#include <fstream> 
#include <cmath> 
#include <armadillo>

#include "random.h"
#include "population.h"
#include "chromosome.h"

#include "mpi.h"

using namespace arma;
using namespace std;

struct posizioni{
    
    double x,y;

};

//Struct per i risultati, il problema è che volevo vedere il valore minimo e il rank del core che lo conteneva.
struct res{
    double cost;
    int rank;
};

//Crea la matrice con tutte le distanze tra le città in modo che non debbano essere calcolate ogni volta
mat distances(ifstream & inputfile, const int& rows){
    if (!inputfile.is_open()){ 
        throw "errore nel file delle posizioni";
    }
        vector<posizioni> pos(rows);

        for(int i=0; i<rows; i++){

            inputfile>> pos[i].x;
            inputfile>> pos[i].y;

        }

    mat dist(rows, rows, fill::zeros);
        for(int i=0; i<rows-1; i++){
            for(int j=i+1; j<rows; j++){

                dist(i,j)= sqrt(pow(pos[j].x-pos[i].x,2) + pow(pos[j].y- pos[i].y, 2));
                dist(j,i) = dist (i,j);
            }
        
        }

    return dist;
}

int pbc(int size, int num){
    if(num>= size){
        return num- size;
    } else if(num<0){
        return size+ num;
    }else {
        return num;
    }
}

int main(int argc, char** argv){
    
    MPI_Init(&argc, &argv);
    
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    //Generatore di semi casuali
    Random rnd;
    int seed[4];
    int p1, p2;
    ifstream Primes("../INPUT/Primes");
    if (Primes.is_open()){
        // Fai saltare a ogni rank un numero di righe proporzionale al proprio ID
        for(int i = 0; i < rank; i++){
            Primes >> p1 >> p2; // Scarta le righe lette dai rank precedenti
        }
        Primes >> p1 >> p2;     // Legge i semi unici per questo rank
    } else {
        cerr << "PROBLEM: Unable to open Primes" << endl;
    }

    ifstream input("../INPUT/seed.in");
    string property;
    if (input.is_open()){
        while ( !input.eof() ){
            input >> property;
            if( property == "RANDOMSEED" ){
                input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
                rnd.SetRandom(seed,p1,p2);
            } 
        }
        input.close();
    } else cerr << "PROBLEM: Unable to open seed.in" << endl;
    
    ifstream info("../INPUT/info.dat");
    
    int dim; //numero città
    int nist; //numero di individui nella popolazione
    int nepo; //numero di epoche 
    int nswap; //dopo quante epoche si scambiano individui tra isole
    
    info >> dim;
    info >> nist;
    info >>nepo;
    info >> nswap;

    info.close();
    
    //importo i punti da un file e calcolo le distanze che vengono inserite in una matrice in modo da calcolarle una sola volta
    ifstream inputfile1("../INPUT/cap_prov_ita.dat");
    mat dist = distances(inputfile1, dim);
    inputfile1.close();
    
    Population pop = Population(dim, nist, rnd, dist, rank, size);
    pop.initialize(); 

    int nmig= nepo/nswap; //numero di migrazioni
    
    for(int j =0; j<nmig; j++){
        for(int i=0; i<nswap; i++){
            pop.move();
            
            // pop.check();
            // pop.print_l_ave();

        }
        
        ivec best_conf = pop.get_best().getconf();  //il vettore che verrà passato agli altri core

        ivec recv_conf(dim - 1); //il vettore che verrà ricevuto
        
        //------------------------------------------------------ Nel metodo 1 -----------------------------------------------------
        
        // imat all_bests(dim - 1, size);

        // MPI_Allgather(best_conf.memptr(), dim - 1, MPI_LONG_LONG, all_bests.memptr(), dim - 1, MPI_LONG_LONG, MPI_COMM_WORLD);
        
        // int rnd_island ;
        // do{
        //     rnd_island = floor(rnd.Rannyu(0,size));
        // } while(rnd_island == rank);
        
        // ivec a = all_bests.col(rnd_island);
        // pop.migrant(a);
        
        
        //---------------------------------------------------------------------------------------------------------------------
        
        // ------------------------------------------------------ Nel metododo 2 ---------------------------------------------------
        
        int dest = pbc(size, rank-1);   // isola successiva
        int source = pbc(size, rank+1); // isola precedente

        //i core con rank pari ricevono e poi inviano, quelli con core dispari al contrario in modo da evitare dead lock

        if (rank %2==0) {
            if (rank!= size -1) {
                MPI_Recv(recv_conf.memptr(), dim-1, MPI_LONG_LONG, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                pop.migrant(recv_conf);
            }
            if (rank!= 0) {
                MPI_Send(best_conf.memptr(), dim-1, MPI_LONG_LONG, dest, 0, MPI_COMM_WORLD);
            }
        } else {
            
            MPI_Send(best_conf.memptr(), dim-1, MPI_LONG_LONG, dest, 0, MPI_COMM_WORLD);
            
            if (rank !=size- 1) {
                MPI_Recv(recv_conf.memptr(), dim -1, MPI_LONG_LONG, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                pop.migrant(recv_conf);
            }
        }
// ------------------------------------------------------------------------------------------------------------------------
        
        if(rank==0){ 
            cout << "migrazione numero " << j << "\n";
            cout << "valore migliore nel rank 0: " <<pop.get_best().get_cost() <<"\n";
        }

    }   

    res local_res, global_res;
    local_res.cost = pop.get_best().get_cost();
    local_res.rank = rank;

    MPI_Allreduce(&local_res, &global_res, 1, MPI_DOUBLE_INT,MPI_MINLOC, MPI_COMM_WORLD);

    if(rank ==global_res.rank){
        pop.print_best();
        cout << "Migliore costo: "<< pop.get_best().get_cost() << "\n";
        cout << "Nel rank: "<< rank << "\n";
    }

    MPI_Finalize();
    return 0;
}