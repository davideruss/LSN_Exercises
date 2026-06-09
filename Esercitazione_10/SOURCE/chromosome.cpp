#include <iostream>
#include <cmath>
#include <vector>
#include "chromosome.h"

using namespace std;
using namespace arma;

Chromosome :: Chromosome(int dim,const mat & distances){
    
    //rnd = var_rand;
    ndim = dim; //numero di elementi dell'individuo

    dist = distances; //matrice delle distanze 

    ivec a(ndim-1); //questo è il vettore di armadillo che contiene i vari indici che corrispondono alle città
    //il vettore deve includere i numeri fino a (dim-1) escludendo "0" che sarà la prima città (e anche l'ultima)

    for(int i=1; i<ndim; i ++){
        a(i-1)=i;
    }
    
    chr= shuffle(a); 

    cost();
    
    return;
}


Chromosome:: Chromosome(const Chromosome &a){
    
    ndim = a.ndim;
    chr = a.chr;
    dist = a.dist;
    l_cost = a.l_cost;

}

void Chromosome::changeconf(const ivec& chr_changed){

    chr = chr_changed;
    this->cost();
}

void Chromosome :: cost(){
    l_cost = dist(0,chr(0)) + dist(0, chr(ndim-2));
    for(int i=0; i<(ndim-2); i++){
        l_cost+= dist(chr(i), chr(i+1));

    }
    return;
}

double Chromosome :: get_cost() const{
    return l_cost;
}

void Chromosome :: swap(int a, int b){
    
    std::swap(chr(a), chr(b));
    //nota: una volta usata questa funzione
    //bisogna ricordarsi che dopo le varie operazioni
    //bisogna chiamare la funzione cost()!!
}



