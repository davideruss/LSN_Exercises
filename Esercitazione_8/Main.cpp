#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include "random.h"
#include "funzioni.h"

using namespace std;

int main(){

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
    
    variabili var= {
        .N=100,
        .M=100,
        .delta_x=2.,
        .delta_sigma= 0.22,
        .delta_mu=0.25,
        .x= 1.,
        .sigma = 1.,
        .mu= 1.,
        .acceptance = 0,
        .acc_mu = 0,
        .acc_sigma = 0,
        .rnd= rnd,
        .H=0,
        .sigma_H=0,
        .T = 2.0,
    };

    ofstream outputfile;
    outputfile.open("OUTPUT/Dati.dat");

    int N=50;
    int M=500;

    for(int i=0; i<N; i++){

        var.acceptance = 0;
        var.acc_mu = 0;
        var.acc_sigma = 0;
        
        for(int j=0; j<M; j++){
            metro_mu(var);
            metro_sigma(var);
            
        }

        // double tentativi_mu_sigma = static_cast<double>(M); 
        
        // double tentativi_x = static_cast<double>(M * 2 * var.N * var.M);

        // cout << "Accettanza = " << double(var.acceptance / tentativi_x) << "\n";
    
        // cout<< "Accettanza = " <<double(var.acc_mu / tentativi_mu_sigma) << "\n" ;
        
        // cout << "Accettanza = " << double(var.acc_sigma / tentativi_mu_sigma) << "\n" ;

        var.T*=0.9;

        Print(var, outputfile);
    }
    outputfile.close();

    var.M=1000;

    Calcolo_H_e_Psi(var, "OUTPUT/H.dat", "OUTPUT/posizioni.dat");

    Calcolo_psi_analitica(var, "OUTPUT/psi.dat");

    return 0;

}


