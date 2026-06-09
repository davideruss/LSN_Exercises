#include <iostream>
#include <fstream> 
#include <cmath> 
#include <armadillo>

#include "random.h"
#include "population.h"
#include "chromosome.h"

using namespace arma;
using namespace std;

struct posizioni{
    
    double x,y;

};

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

int main(int argc, char** argv){
    
    if(argc!= 2){
        cout << "usage: " << argv[0]  << " <number of epochs>\n" ;
        return -1;
    }
    //Generatore di semi casuali
    Random rnd;
    int seed[4];
    int p1, p2;
    ifstream Primes("Primes");
    if (Primes.is_open()){
    Primes >> p1 >> p2 ;
    } else cerr << "PROBLEM: Unable to open Primes" << endl;
    Primes.close();

    ifstream input("seed.in");
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
    
    int dim= 34; //numero città
    int nist= 100; //numero di individui nella popolazione
    int nepo= atoi(argv[1]); //numero di epoche 

    //importo i punti da un file e calcolo le distanze che vengono inserite in una matrice in modo da calcolarle una sola volta
    ifstream inputfile1("INPUT/circle.dat");
    //ifstream inputfile2("INPUT/square.dat");

    mat dist = distances(inputfile1, dim);

    // cout << dist(0,0) << " " << dist(1,2) << " " << dist(2,1) << endl;
    
    Population pop = Population(dim, nist, rnd, dist);

    pop.initialize();

    for(int i=0; i<nepo; i++){
        pop.move();
        
        //pop.check();
        pop.print_l_ave();
    }
    cout << pop.get_l_ave() << "\n";
    cout << pop.get_best().get_cost() << "\n";
    pop.print_best();
    
    return 0;
}