#include <iostream>
#include <array>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include "random.h"
#include "Funzioni.h"

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
    
    ofstream outputfile;

    int N= 100; //numero di blocchi
    int M= 100; //numero di blocchi
    int n_passi = 100; //numero di passi del RW
    
    outputfile.open("RW_discreto.txt");
    analisi(n_passi, N, M, outputfile, rnd, 0);
    outputfile.close();

    outputfile.open("RW_continuo.txt");
    analisi(n_passi, N, M, outputfile, rnd, 1);
    outputfile.close();
    
    return 0;
}


