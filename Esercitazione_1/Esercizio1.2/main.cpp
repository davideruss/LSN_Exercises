#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include "random.h"

using namespace std;

void histo0(ofstream &, Random &, int N, int L); //funzione che fa l'istogramma di una distribuzione uniforme

void histo1(ofstream &, Random &, int N, int L, double Gamma =1, double mu=0); //funzione che fa l'istogramma di una distribuzione di Cauchy

void histo2(ofstream &, Random &, int N, int L, double lambda=1); // funzione che fa l'istogramma di una distribuzione esponenziale


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
    

    int N=10000; //numero di valori nell'istogramma
    int L_values[] = {1, 2, 10, 100}; //numero di valori tra cui mediare per ogni valore dell'istogramma

//------------------- Cauchy-Lorentz distribution -----------------------

    ofstream outputfile;

    for (int L : L_values) {
        string filename = "histo_Lorentz_" + to_string(L) + ".txt";
        
        outputfile.open(filename);

        if (outputfile.is_open()) {
            histo1(outputfile, rnd, N, L); 
            outputfile.close();
        }
    }

// -------------------- Exponential ---------------------------------------

    for (int L : L_values) {
        string filename = "histo_exp_" + to_string(L) + ".txt";
        
        outputfile.open(filename);

        if (outputfile.is_open()) {
            histo2(outputfile, rnd, N, L); 
            outputfile.close();
        }
    }

// -------------------- Uniforme ---------------------------------------

    for (int L : L_values) {
        string filename = "histo_Uniform_" + to_string(L) + ".txt";
        
        outputfile.open(filename);

        if (outputfile.is_open()) {
            histo0(outputfile, rnd, N, L); 
            outputfile.close();
        }
    }

    return 0;

}

void histo0(ofstream &outputfile, Random & rnd, int N, int L){

    for(int i=0; i<N; i++){
        double sum = 0. ;

        for( int j=0; j<L; j++ ){
            sum+= rnd.Rannyu(); 
        }

        outputfile <<  sum/static_cast<double>(L) << endl;

    }

}

void histo1(ofstream &outputfile, Random & rnd, int N, int L, double Gamma , double mu){

    for(int i=0; i<N; i++){
        double sum = 0. ;

        for( int j=0; j<L; j++ ){
            sum+= rnd.Cauchy_Lorentz(Gamma, mu); 
        }

        outputfile <<  sum/static_cast<double>(L) << endl;

    }

}

void histo2(ofstream &outputfile, Random & rnd, int N, int L, double lambda){

    for(int i=0; i<N; i++){
        double sum = 0. ;

        for( int j=0; j<L; j++ ){
            sum+= rnd.exponential(lambda); 
        }

        outputfile <<  sum/static_cast<double>(L) << endl;

    }

}



