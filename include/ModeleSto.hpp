#ifndef MODELESTO_HPP
#define MODELESTO_HPP

#include <iostream>
#include <cmath>
#include <random>
#include <cstdlib>

#include "MDP.hpp"
#include "progdym.hpp"
#include "utils.hpp"

using namespace std;

template<class S, class A> class ModelSto{
    protected:
        double M_gamma;
        double M_eps;
        double M_d;
        double** M_Pi;
        int* M_mu_opt;
        MDP<S,A>* M_modele;
        double** M_Q;

        //renvoi l'index de l'etat
        int index_etat(S s);
        //renvoi l'index de l'action
        int index_action(A a);
    public:
        //constructeurs
        ModelSto();
        ModelSto(const ModelSto &m);
        ModelSto(MDP<S,A>* m);

        //destructeur
        ~ModelSto();

        //accesseurs
        double get_gamma() const {return M_gamma;}
        double get_eps() const {return M_eps;}
        double get_d() const {return M_d;}
        double** get_Pi() const {return M_Pi;}
        double get_Pi(int i_s, int i_a){return M_Pi[i_s][i_a];}
        int* get_mu_opt() const {return M_mu_opt;}
        int get_mu_opt(int i) const {return M_mu_opt[i];}
        MDP<S,A>* get_modele() const {return M_modele;}
        double** get_Q() const {return M_Q;}
        double get_Q(int i_s, int i_a){return M_Q[i_s][i_a];}

        //mutateurs
        void set_gamma(double g){M_gamma=g;}
        void set_eps(double eps){M_eps=eps;}
        void set_d(double d){M_d=d;}

        A applique_politique_opt(S s);
        A applique_politique_alea(S s);

        void calcul_politique_opt();
        void calcul_politique_alea();

        virtual void calcul_episodes(S s)=0;
        void apprentissage(int nb_episodes);
};


template<class S, class A> int ModelSto<S,A>::index_etat(S s){ //on amdet que toutes les valeurs sont différentes
    for(int i=0; i<M_modele->get_nb_etats(); i++){
        if(abs(M_modele->get_tab_etats()[i]-s)<pow(10,-4)){ //car s peut être double
            return i;
        }
    }
    return -1;
}

template<class S, class A>int ModelSto<S,A>::index_action(A a){
    for(int i=0; i<M_modele->get_nb_actions(); i++){
        if(abs(M_modele->get_tab_actions()[i]-a)<pow(10,-4)){ //car a peut être double
            return i;
        }
    }
    return -1;
}

template<class S, class A> ModelSto<S,A>::ModelSto(){
    M_gamma=0.;
    M_eps=0.;
    M_d=0.;
    M_Pi=nullptr;
    M_mu_opt=nullptr;
    M_modele=nullptr;
    M_Q=nullptr;
}

template<class S, class A> ModelSto<S,A>::ModelSto(const ModelSto &m){
    M_gamma=m.get_gamma();
    M_eps=m.get_eps();
    M_d=m.get_d();
    M_Pi=new double*[m.get_modele()->get_nb_etats()];
    for(int i = 0; i < m.get_modele()->get_nb_etats(); i++){
        M_Pi[i]=new double[m.get_modele()->get_nb_actions()];
    }
    M_mu_opt=new int[m.get_modele()->get_nb_etats()];
    M_modele=m.get_modele();
    M_Q=new double*[m.get_modele()->get_nb_etats()];
    for(int i = 0; i < m.get_modele()->get_nb_etats(); i++){
        M_Q[i]=new double[m.get_modele()->get_nb_actions()];
    }
    for(int i_s = 0; i_s < m.get_modele()->get_nb_etats(); i_s++){
        for(int i_a = 0; i_a < m.get_modele()->get_nb_actions(); i_a++){
            M_Q[i_s][i_a]=0.;
        }
    }
}

template<class S, class A> ModelSto<S,A>::ModelSto(MDP<S,A>* m){
    M_gamma=0.;
    M_eps=0.;
    M_d=0.;
    M_Pi=new double*[m->get_nb_etats()];
    for(int i = 0; i < m->get_nb_etats(); i++){
        M_Pi[i]=new double[m->get_nb_actions()];
    }
    M_mu_opt=new int[m->get_nb_etats()];
    for(int i_s = 0; i_s < m->get_nb_etats(); i_s++){
        M_mu_opt[i_s]=0;
    }
    M_modele=m;
    M_Q=new double*[m->get_nb_etats()];
    for(int i = 0; i < m->get_nb_etats(); i++){
        M_Q[i]=new double[m->get_nb_actions()];
    }
    for(int i_s = 0; i_s < m->get_nb_etats(); i_s++){
        for(int i_a = 0; i_a < m->get_nb_actions(); i_a++){
            M_Q[i_s][i_a]=0.;
        }
    }
}

//destructeur
template<class S, class A> ModelSto<S,A>::~ModelSto(){
    if(M_Pi!= nullptr){
        for(int i=0; i<M_modele->get_nb_etats(); i++){
            delete [] M_Pi[i];
        }
        delete [] M_Pi;
    } 
    if(M_mu_opt!= nullptr){delete [] M_mu_opt;} 
    if(M_Q!= nullptr){
        for(int i=0; i<M_modele->get_nb_etats(); i++){
            delete [] M_Q[i];
        }
        delete [] M_Q;
    }
}

template<class S, class A> A ModelSto<S,A>::applique_politique_opt(S s){
    int i_s=index_etat(s);
    return M_modele->get_tab_actions()[M_mu_opt[i_s]];
}

template<class S, class A> A ModelSto<S,A>::applique_politique_alea(S s){
    int i_s=index_etat(s);
    A* tab_actions_possibles;
    int n_a=M_modele->get_possible_action(s, tab_actions_possibles);
    int* tab_index_actions_possibles=new int[n_a];
    for(int i=0; i<n_a; i++){
        tab_index_actions_possibles[i]=index_action(tab_actions_possibles[i]);
    }
    double* Fdr=new double[n_a+1];
    Fdr[0]=0.;
    for(int i_a=0; i_a<n_a; i_a++){
        Fdr[i_a+1]=Fdr[i_a]+M_Pi[i_s][tab_index_actions_possibles[i_a]];
    }
    random_device rd;
    default_random_engine re(rd());
    uniform_real_distribution<double> distrib(0,1.0);
    double p=distrib(re);
    for(int i_a=0; i_a<n_a; i_a++){
        if(Fdr[i_a]<=p and Fdr[i_a+1]>p){
            return M_modele->get_tab_actions()[tab_index_actions_possibles[i_a]];
        }
    }
    return -1;
}

template<class S, class A> void ModelSto<S,A>::calcul_politique_opt(){
    int max_i_a;
    double max_a;
    A* tab_actions_possibles;
    int n_a;
    int i_a;
    for(int i_s=0; i_s<M_modele->get_nb_etats(); i_s++){
        n_a=M_modele->get_possible_action(M_modele->get_tab_etats()[i_s], tab_actions_possibles);
        max_i_a= 0;
        max_a=0.;
        for(int i=0; i<n_a; i++){ 
            i_a=index_action(tab_actions_possibles[i]);
            if(M_Q[i_s][i_a]>=max_a){
                max_a=M_Q[i_s][i_a];
                max_i_a=i_a;
            }
        }
        M_mu_opt[i_s]=max_i_a;
    }
}

template<class S, class A> void ModelSto<S,A>::calcul_politique_alea(){
    int i_a_opt;
    A* tab_actions_possibles;
    int n_a;
    for(int i_s=0; i_s<M_modele->get_nb_etats(); i_s++){ 
        i_a_opt=index_action(applique_politique_opt(M_modele->get_tab_etats()[i_s]));
        n_a=M_modele->get_possible_action(M_modele->get_tab_etats()[i_s], tab_actions_possibles);
        for(int i_a=0; i_a<M_modele->get_nb_actions(); i_a++){  
            if(is_in<A>(tab_actions_possibles,n_a,M_modele->get_tab_actions()[i_a])){
                if(i_a==i_a_opt){
                    M_Pi[i_s][i_a_opt]=1.-M_eps+M_eps/n_a;
                } else {
                    M_Pi[i_s][i_a]=M_eps/n_a;
                }
            }
            else{
                M_Pi[i_s][i_a]=0.;
            }
        }
    }
}

template<class S, class A> void ModelSto<S,A>::apprentissage(int nb_episodes){
    for(int i=0; i<nb_episodes; i++){
        S act=M_modele->get_reset(M_modele->get_nb_etats(),M_modele->get_tab_etats());
        calcul_episodes(act);
    }
}

#endif