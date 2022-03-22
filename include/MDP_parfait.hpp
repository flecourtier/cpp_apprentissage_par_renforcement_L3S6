#ifndef MDP_PARFAIT_HPP
#define MDP_PARFAIT_HPP

#include <iostream>
#include <cmath>

#include "MDP.hpp"

template <class S, class A> class MDP_parfait : public MDP<S,A>{
    private:
        double** P;
        double** R;

        int index(int i_s, int i_a){return i_s+this->M_nb_etats*i_a;}
    public:
        //accesseurs
        double** get_P() const {return P;}
        double get_P(int i_s, int i_a, int i_s_plus){return P[index(i_s,i_a)][i_s_plus];}
        double** get_R() const {return R;}
        double get_R(int i_s, int i_a, int i_s_plus){return R[index(i_s,i_a)][i_s_plus];}
    
        //mutateurs
        void set_P(int i_s, int i_a, int i_s_plus,double p){
            P[index(i_s,i_a)][i_s_plus]=p;
        }
        void set_R(int i_s, int i_a, int i_s_plus,double r){
            R[index(i_s,i_a)][i_s_plus]=r;
        }

        //constructeurs
        MDP_parfait() : MDP<S,A>(){
            P=nullptr;
            R=nullptr;
        }
        MDP_parfait(int nb_etats, int nb_actions);
        MDP_parfait(const MDP_parfait<S,A> &m);

        //destructeur
        ~MDP_parfait(){
            if(P!= nullptr){delete [] P;} 
            if(R!= nullptr){delete [] R;}
            //appel automatique au destructeur de la classe mère (MDP)
        }
        
        MDP_parfait<S,A> & operator = (const MDP_parfait<S,A> &);
};


template <class S, class A> MDP_parfait<S,A>::MDP_parfait(int nb_etats, int nb_actions) : MDP<S,A>(nb_etats,nb_actions){
    P=new double*[nb_etats*nb_actions];
    for(int i = 0; i < nb_etats*nb_actions; i++){
        P[i]=new double[nb_etats];
    }
    R=new double*[nb_etats*nb_actions];
    for(int i = 0; i < nb_etats*nb_actions; i++){
        R[i]=new double[nb_etats];
    }
}

template <class S, class A> MDP_parfait<S,A>::MDP_parfait(const MDP_parfait<S,A> &m) : MDP<S,A>(m){
    if(m.P!=nullptr){
        P=new double*[m.get_nb_etats()*m.get_nb_actions()];
        for(int i = 0; i < m.get_nb_etats()*m.get_nb_actions(); i++){
            P[i]=new double[m.get_nb_etats()];
        }
        for(int i=0; i<m.get_nb_etats()*m.get_nb_actions(); i++){
            for(int j=0; j<m.get_nb_etats(); j++){
                P[i][j]=m.P[i][j];
            }
        }
    }
    if(m.R!=nullptr){
        R=new double*[m.get_nb_etats()*m.get_nb_actions()];
        for(int i = 0; i < m.get_nb_etats()*m.get_nb_actions(); i++){
            R[i]=new double[m.get_nb_etats()];
        }
        for(int i=0; i<m.get_nb_etats()*m.get_nb_actions(); i++){
            for(int j=0; j<m.get_nb_etats(); j++){
                R[i][j]=m.R[i][j];
            }
        }
    }
}


template<class S, class A> MDP_parfait<S,A> & MDP_parfait<S,A>::operator = (const MDP_parfait<S,A> & m){
    if (this != &m){
        set_nb_etats(m.get_nb_etats());
        set_nb_actions(m.get_nb_actions());
        if(this->M_tab_etats!= nullptr){delete [] this->M_tab_etats;}
        if(this->M_tab_actions!= nullptr){delete [] this->M_tab_actions;}
        if(m.get_tab_etats()!=nullptr){
            this->M_tab_etats=new S[this->M_nb_etats];
            for(int i=0; i<this->M_nb_etats; i++){
                this->M_tab_etats[i]=m[i];
            }
        }
        if(m.get_tab_actions()!=nullptr){
            this->M_tab_actions=new A[this->M_nb_actions];
            for(int i=0; i<this->M_nb_actions; i++){
                this->M_tab_actions[i]=m(i);
            }
        }
        this->reset=m.reset;
        this->possible_action=m.possible_action;
        this->environnement=m.environnement;
        this->is_terminal=m.is_terminal;
        if(P!= nullptr) delete [] P;
        if(m.P()!=nullptr){
            P=new double[m.get_nb_etats()*m.get_nb_actions()][m.get_nb_etats()];
            for(int i=0; i<m.get_nb_etats()*m.get_nb_actions(); i++){
                for(int j=0; j<m.get_nb_etats(); j++){
                    P[i][j]=m.P[i][j];
                }
            }
        }
        if(R!= nullptr) delete [] R;
        if(m.R!=nullptr){
            R=new double[m.get_nb_etats()*m.get_nb_actions()][m.get_nb_etats()];
            for(int i=0; i<m.get_nb_etats()*m.get_nb_actions(); i++){
                for(int j=0; j<m.get_nb_etats(); j++){
                    R[i][j]=m.R[i][j];
                }
            }
        }
    }
    return *this;
}

#endif