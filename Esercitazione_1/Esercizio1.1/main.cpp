#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include "random.h"

using namespace std;

double error(double , double, int );

int main(){

    int M=10000; //numero di elementi
    int N=100; //numero di bin
    int L= M/N; //numero di elementi in ogni bin

//------------------- First part -----------------------

    //calcoliamo la media dei numeri casuali con la relativa incertezza

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

    
    ofstream outputfile("Average.txt");
    
    double ave=0.;
    double sum_prog=0;
    double sum_prog2=0;
    //Queste sono le tre variabili che mi serviranno per fare il data blocking
    
    for(int i=0; i<N; i++){
        
        for(int j=0; j<L; j++){
            ave+=rnd.Rannyu(); //calcolo la media su un blocco
        }
        ave/= static_cast<double>(L);
        
        //aggiorno le somme progressive
        sum_prog+=ave;
        sum_prog2+= pow(ave, 2);

        double average = sum_prog/(i+1);
        double squared_average = sum_prog2/(i+1);
        double err = error(average, squared_average, i);
        
        //stampo media ed errore sul file
        outputfile << average- 0.5 << "\t" << err << "\n";

        ave=0;
    }

    outputfile.close();

//------------------- Second part -----------------------

    //calcoliamo la deviazione standard dei numeri casuali con la relativa incertezza

    ofstream outputfile2("devstd.txt");

    double dev=0.;
    sum_prog=0;
    sum_prog2=0;

    
    for(int i=0; i<N; i++){
        
        for(int j=0; j<L; j++){
            dev+=pow((rnd.Rannyu()-0.5), 2);
        }
        dev/= static_cast<double>(L);
        
        //aggiorno le somme progressive
        sum_prog+=dev;
        sum_prog2+= pow(dev, 2);

        double average = sum_prog/(i+1);
        double squared_average = sum_prog2/(i+1);
        double err = error(average, squared_average, i);
        
        //stampo media ed errore sul file
        outputfile2 << average- 1./12. << "\t" << err << "\n";
        
        dev=0;
    }

    outputfile2.close();

//------------------- Third part -----------------------
    
    //calcoliamo la distribuzione del chi quadro

    ofstream outputfile3("chi^2.txt");
    
    N= 10000; //numero di throws per ogni calcolo del chi quadro
    M=100; //numero di intervalli in cui ho diviso [0,1]
    int n=10000; //numero di volte in cui calcoliamo il chi quadro
    int E = N/M; // valore aspettato

    //vector <double> random_numbers(n);
    vector<int> counts(M , 0);

    for(int j=0; j<n; j++){
        fill(counts.begin(), counts.end(), 0);
        
        for(int i =0; i<N; i++){

            counts[floor(rnd.Rannyu()*100.)]++;

        }

        double chi2=0;

        for(int i =0; i<M; i++){
            chi2+=pow(static_cast<double>(counts[i]-E), 2);
        }
        outputfile3 << chi2/static_cast<double>(E) << endl;
        
        
    }

    outputfile3.close();
    return 0;
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



