#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include "random.h"

using namespace std;

double error(double , double, int );

double calculate_pi(Random &); 

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
    
    int N=1000; //numero di lanci per ogni blocco
    int M=100; //numero di blocchi

    //imposto i parametri dell'esperimento
    double half_pi = calculate_pi(rnd)*0.5;
    double bar_length = 0.5 ;
    double line_distance = 1. ;
    
    //i parametri da estrarre
    double pos, ang;

    int num_intersection=0; //numero di volte in cui la barra interseca una linea

    //elementi per il datablocking
    double ave=0.;
    double sum_prog=0.;
    double sum_prog2=0.;

    ofstream outputfile;
    outputfile.open("Buffon.txt");
    
    for(int i =0; i<M; i++){
        
        //estraggo la posizione dell'estremo del bastone e l'angolo e poi mi calcolo la posizione del secondo estremo
        for(int j=0; j<N; j++){
            
            pos = rnd.Rannyu();
            ang = rnd.Rannyu(0, half_pi);

            if(pos + bar_length*cos(ang)> line_distance) num_intersection ++;
        }

        ave = 2*bar_length*static_cast<double>(N)/(static_cast<double>(num_intersection)*line_distance);

        sum_prog += ave;
        sum_prog2 += pow(ave, 2);
        double average = sum_prog/(i+1);
        double squared_average = sum_prog2/(i+1);
        double err = error(average, squared_average, i);
        outputfile<< average << " " << err <<endl;

        num_intersection =0;
    }
    outputfile.close();
    
}

double error(double av, double av2, int n){
    
    if(n>0){
        return sqrt((av2- av*av)/static_cast<double>(n));
    } else if(n==0){
        return 0;
    } else{
        cerr << "error in the error function"<< endl;
        return 0 ;
    }

}

double calculate_pi(Random & rnd){

    int N=100000;
    int sum=0;
    double s;

    for(int i=0; i<N; i++){

        s=pow(rnd.Rannyu(),2)+ pow(rnd.Rannyu(),2);
        if(s<1) sum++;
    }
    return static_cast<double>(sum)*4/static_cast<double>(N);
}

