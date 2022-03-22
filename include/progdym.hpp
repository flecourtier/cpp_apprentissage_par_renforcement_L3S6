#ifndef PROGDYM_HPP
#define PROGDYM_HPP

#include <iostream>
#include <cmath>

#include "MDP_parfait.hpp"

template<class S, class A> class ProgDym{
    protected:
        double M_gamma;
        int* M_mu;
        double* M_V;
        MDP_parfait<S,A>* M_modele;

        //renvoi l'index de l'etat
        int index_etat(S s);
    public:
        //constructeurs
        ProgDym();
        ProgDym(const ProgDym &p);
        ProgDym(MDP_parfait<S,A>* m);

        //destructeur
        ~ProgDym();

        //accesseurs
        double get_gamma() const {return M_gamma;}
        int* get_mu() const {return M_mu;}
        int get_mu(int i) const {return M_mu[i];}
        double* get_V() const {return M_V;}
        double get_V(int i) const {return M_V[i];}
        MDP_parfait<S,A>* get_modele() const {return M_modele;}

        //mutateur
        void set_gamma(double g){M_gamma=g;}

        A applique_politique(S s);
        void calcul_politique();

        virtual void resolution(){};
};


template<class S, class A> int ProgDym<S,A>::index_etat(S s){ //on admetra que toutes les valeurs sont différentes
    for(int i=0; i<M_modele->get_nb_etats(); i++){
        if(abs(M_modele->get_tab_etats()[i]-s)<pow(10,-4)){ //car s peut être double
            return i;
        }
    }
    return -1;
}

template<class S, class A> ProgDym<S,A>::ProgDym(){
    M_gamma=0.;
    M_mu=nullptr;
    M_V=nullptr;
    M_modele=nullptr;
}

template<class S, class A> ProgDym<S,A>::ProgDym(const ProgDym &p){
    M_gamma=p.get_gamma();
    int taille=p.get_modele()->get_nb_etats();
    if(p.get_mu()!=nullptr){
        M_mu=new int[taille];
        for(int i=0; i<taille; i++){
            M_mu[i]=p.get_mu(i);
        }
    }
    if(p.get_V()!=nullptr){
        M_V=new double[taille];
        for(int i=0; i<taille; i++){
            M_V[i]=p.get_V(i);
        }
    }
    M_modele=p.get_modele();
}

template<class S, class A> ProgDym<S,A>::ProgDym(MDP_parfait<S,A>* m){
    M_gamma=0.;
    int taille=m->get_nb_etats();
    M_mu=new int[taille];
    M_V=new double[taille];
    M_modele=m;
}

//destructeur
template<class S, class A> ProgDym<S,A>::~ProgDym(){
    if(M_mu!= nullptr){delete [] M_mu;} 
    if(M_V!= nullptr){delete [] M_V;}
}

template<class S, class A> A ProgDym<S,A>::applique_politique(S s){
    int i_s=index_etat(s);
    int i_a=M_mu[i_s];
    return M_modele->get_tab_actions()[i_a];
}

template<class S, class A> void ProgDym<S,A>::calcul_politique(){
    int* max_i_a=new int[M_modele->get_nb_etats()];
    double* max_a=new double[M_modele->get_nb_etats()];
    double* somme_sp=new double[M_modele->get_nb_etats()];
    for(int i_s=0; i_s<M_modele->get_nb_etats(); i_s++){
        max_i_a[i_s]=0;
        max_a[i_s]=0.;
        somme_sp[i_s]=0.;
    }
    for(int i_s=0; i_s<M_modele->get_nb_etats(); i_s++){
        for(int i_a=0; i_a<M_modele->get_nb_actions(); i_a++){
            for(int i_sp=0; i_sp<M_modele->get_nb_etats(); i_sp++){
                somme_sp[i_s]+=(M_modele->get_R(i_s,i_a,i_sp)+M_gamma*M_V[i_sp])*M_modele->get_P(i_s,i_a,i_sp);
            }  
            if(somme_sp[i_s]>=max_a[i_s]){
                max_a[i_s]=somme_sp[i_s];
                max_i_a[i_s]=i_a;
            }
            somme_sp[i_s]=0.;
        }
        M_mu[i_s]=M_modele->get_tab_actions()[max_i_a[i_s]];
    }
}

#endif