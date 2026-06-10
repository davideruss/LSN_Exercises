#include <iostream>
#include <cmath>
#include <vector>
#include <numeric>
#include "population.h"
#include "chromosome.h"
#include "random.h"

using namespace std;
using namespace arma;

Population :: Population(int dim, int ist, const Random &var_rand,const mat &distances ){
    
    rnd = var_rand;
    ndim = dim;
    nist = ist;
    
    pop.reserve(nist);
    pop_new.reserve(nist);
    
    for(int i=0; i<nist; i++){
        pop.push_back(Chromosome(ndim, distances));
        pop_new.push_back(Chromosome(pop[i]));
    }

    sort(pop.begin(), pop.end(), [](const Chromosome& a,const Chromosome& b){return a.get_cost() < b.get_cost();});
    
    
    best= pop[0];
    this->initialize();

    l_average();
}

void Population :: initialize(){
    ifstream input("INPUT/properties.dat");
    if(!input.is_open()){
        throw "il file properties.dat non si apre\n";
    }
    string property;
    while(!input.eof()){
        input >>property;
        if(property=="PROB_CROSSOVER"){
            input >> prob_crossover;
        } else if( property =="PROB_MUTATION1"){
            input >> prob_mutation1;
        } else if( property =="PROB_MUTATION2"){
            input >> prob_mutation2;
        } else if( property =="PROB_MUTATION3"){
            input >> prob_mutation3;
        } else if( property =="PROB_MUTATION4"){
            input >> prob_mutation4;
        } else if(property == "VAL_SELECTION"){
            input >> val_selection;
        } else{
            cout << "mi sa che c'è un errore nel file properties\n";
        }
    }
    
}


void Population :: sort_new(){
    sort(pop_new.begin(), pop_new.end(), [](const Chromosome& a,const Chromosome& b){return a.get_cost() < b.get_cost();});

}

void Population ::crossover(int a, int b){
    int sez = int(rnd.Rannyu()*(ndim -2))+1;
    ivec conf_1(ndim-1);
    ivec conf_2(ndim-1);
    for(int i=0;i< sez; i++ ){
        conf_1[i] = pop_new[a].getconf()(i);
        conf_2[i] = pop_new[b].getconf()(i);
    }
    int j = sez;
    bool check1 = false;
    bool check2 = false;

    for(int i=0;i<ndim-1; i++){
        for(int t=0; t<sez; t++){
            if(conf_1[t] == pop_new[b].getconf()(i)){
                check1= true; //se il controllo è true vuol dire che quella città è già presente
                break;
            }
        }

        if(check1==false){
            conf_1[j]= pop_new[b].getconf()(i);
            j++;
        } else {
            check1=false;
        }
        
        if(j==ndim-1){
            break;//se ho riempito conf_1 esco dal ciclo
        }
    }
    
    j=sez; //ripristino la variabile al valore originale

    for(int i=0;i<ndim-1; i++){
        for(int t=0; t<sez; t++){
            if(conf_2[t] == pop_new[a].getconf()(i)){
                check2= true; //se il controllo è true vuol dire che quella città è già presente
                break;
            }
        }

        if(check2==false){
            conf_2[j]= pop_new[a].getconf()(i);
            j++;
        }else{
            check2= false;
        }

        if(j==ndim-1) break;
    }  

    pop_new[a].changeconf(conf_1);
    pop_new[b].changeconf(conf_2);
    
}

int Population :: selection(){

    return floor(nist*pow(rnd.Rannyu(), val_selection));
}

void Population :: mutation1(){
    //scambio due elementi
    int a= floor(rnd.Rannyu()*(ndim-1));
    int b= floor(rnd.Rannyu()*(ndim-1));
    
    if(a!=b){
        int c= int(rnd.Rannyu()*nist);

        pop_new[c].swap(a, b);
        pop_new[c].cost();
    }

}

void Population :: mutation2(){
    int mid = (ndim+1)/2;
    int a = floor(rnd.Rannyu()*(nist));
    int start= floor(rnd.Rannyu()*mid);
    int end = mid + floor(rnd.Rannyu()*(mid-2));

    if(start==end){
        throw "errore nella mutazione 2\n";
    }
    while(start< end){
        pop_new[a].swap(start, end);
        start++;
        end--;
    }

    pop_new[a].cost(); // calcolo il nuovo costo post mutazione
}

void Population :: mutation3(){
    int c = rnd.Rannyu(0, nist);
    int n = rnd.Rannyu(0, ndim-1);

    for(int i =0; i<ndim-1; i++){
        pop_new[c].swap(i, (i+n)%(ndim-1));
    }
    pop_new[c].cost();
    
}

void Population :: mutation4(){
    int c = rnd.Rannyu(0, nist);
    int sez = (ndim-1)/2;
    int a = rnd.Rannyu(0, sez);

    for(int i=0; i<a; i++){
        pop_new[c].swap(i, sez+i);
    }
    pop_new[c].cost();
}


void Population :: new_pop(){

    for(int i=0; i<nist; i++){

        pop_new[i] = Chromosome(pop[this->selection()]);
    }
}

void Population::l_average(){

    double sum = std::accumulate(pop.begin(), pop.end(), 0.0, [](double tot, Chromosome &a){return tot+ a.get_cost();});
    l_ave = sum/pop.size();
}

void Population:: move(){

    //creo la nuova popolazione
    new_pop();
    //faccio il crossover con probabilità prob_crossover
    for(int i=0;i<nist-1 ; i+=2){

        if(rnd.Rannyu()<prob_crossover) crossover(i, i+1);
    }

    //faccio mutazioni con probabilità prob_mutation
    for(int i =0; i<10; i++){
        if(rnd.Rannyu()< prob_mutation1) mutation1();

        if(rnd.Rannyu()< prob_mutation2) mutation2();

        if(rnd.Rannyu()< prob_mutation3) mutation3();

        if(rnd.Rannyu()< prob_mutation4) mutation4();
    }

    //ordino la popolazione nuova
    sort_new();

    //dopo aver finito le operazioni sposto la popolazione nuova al posto di quella vecchia
    pop.swap(pop_new);

    //calcolo la media della funzione costo
    l_average();

    //se otteniamo un individuo migliore rispetto a tutti gli altri trovati lo salvo
    if(pop[0].get_cost()< best.get_cost()) best=pop[0];

}

void Population ::check(){
    bool controllo=false;
    int val_aspettato= ndim*(ndim-1)/2;
    int tot = accu(pop[floor(rnd.Rannyu()*(ndim-1))].getconf());

    if(tot!=val_aspettato){
        controllo = true;
    } 

    cout << "ma il crossover fa casino? "<<  controllo << endl;
}

void Population :: print_best(){
    ofstream outputfile("OUTPUT/best.dat");
    if(!outputfile.is_open()){
        throw "il file best.dat non si apre\n";
    }

    outputfile << 0 << "\n";
    for(int i=0; i<ndim-1; i++){
        outputfile << best.getconf()(i) << "\n";
    }
    outputfile << 0 << "\n";
}

void Population :: print_l_ave(){
    ofstream outputfile;
    outputfile.open("OUTPUT/l_ave.dat", ios::app);

    if(!outputfile.is_open()){
        throw "il file l_ave.dat non si apre\n";
    }

    outputfile << l_ave << "\n";
}

