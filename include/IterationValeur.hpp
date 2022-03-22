#ifndef ITERATIONVALEUR_HPP
#define ITERATIONVALEUR_HPP

#include <iostream>
#include <cmath>

#include "MDP_parfait.hpp"
#include "progdym.hpp"


template<class S, class A> class IterationValeur : public ProgDym<S,A>{
    protected:
        int M_imax;
        double M_eps;

        double max_a(int i_s, double V_k);
    public:
        //constructeurs
        IterationValeur() : ProgDym<S,A>(){
            M_imax=0;
            M_eps=0.;
        }
        IterationValeur(const IterationValeur &v) : ProgDym<S,A>(v){
            M_imax=v.get_imax();
            M_eps=v.get_eps();
        }
        IterationValeur(MDP_parfait<S,A>* m) : ProgDym<S,A>(m){
            M_imax=0;
            M_eps=0.;
        }

        //accesseurs
        int get_imax() const {return M_imax;}
        double get_eps() const {return M_eps;}

        void resolution();
};


template<class S, class A> double IterationValeur<S,A>::max_a(int i_s, double V_k){
    double V_k_p=0.;
    double somme_sp=0.;
    for(int i_a=0; i_a<this->M_modele->get_nb_actions(); i_a++){
        for(int i_sp=0; i_sp<this->M_modele->get_nb_etats(); i_sp++){
            somme_sp+=(this->M_modele->get_R(i_s,i_a,i_sp)+this->M_gamma*V_k)*this->M_modele->get_P(i_s,i_a,i_sp);
        }  
        if(somme_sp>=V_k_p){
            V_k_p=somme_sp;
        }
        somme_sp=0.;
    }
    return V_k_p;
}

template<class S, class A> void IterationValeur<S,A>::resolution(){
    double* V_temp=new double[this->M_modele->get_nb_etats()];
    double* V_k=new double[this->M_modele->get_nb_etats()];
    double* V_k_p=new double[this->M_modele->get_nb_etats()];
    int k=0;
    for(int i_s=0; i_s<this->M_modele->get_nb_etats(); i_s++){
        V_temp[i_s]=0.;
        V_k[i_s]=0.;
        V_k_p[i_s]=max_a(i_s,V_k[i_s]);
    }
    while(sqrt(pow(V_k_p[0]-V_k[0],2)+pow(V_k_p[1]-V_k[1],2))>M_eps or k<M_imax){
        for(int i_s=0; i_s<this->M_modele->get_nb_etats(); i_s++){
            V_temp[i_s]=V_k[i_s];
            V_k[i_s]=V_k_p[i_s];
            V_k_p[i_s]=max_a(i_s,V_temp[i_s]);
        }
        k++;
    }
    for(int i_s=0; i_s<this->M_modele->get_nb_etats(); i_s++){
        this->M_V[i_s]=V_k_p[i_s];
    }
}

#endif