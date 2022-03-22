#ifndef MDP_HPP
#define MDP_HPP

#include <iostream>
#include <cmath>

template <class S, class A> class MDP{
    protected:
        int M_nb_etats;
        int M_nb_actions;
        S *M_tab_etats;
        A *M_tab_actions;
        S (*reset)(int,S*);
        int (*possible_action)(S,A*&);
        void (*environnement)(S,A,double&,S&);
        bool (*is_terminal)(S,int);
    public:
        //accesseurs
        int get_nb_etats() const{return M_nb_etats;}
        int get_nb_actions() const{return M_nb_actions;}
        S *get_tab_etats() const {return M_tab_etats;}
        A *get_tab_actions() const {return M_tab_actions;}
        S get_reset(int n,S* s) const {return reset(n,s);}
        int get_possible_action(S s,A*& a) const {return possible_action(s,a);}
        void get_environnement(S s,A a,double& r,S& s_p) const {return environnement(s,a,r,s_p);}
        bool get_is_terminal(S s,int n) const {return is_terminal(s,n);}

        //mutateurs
        void set_nb_etats(int nb_etats){M_nb_etats=nb_etats;}
        void set_nb_actions(int nb_actions){M_nb_actions=nb_actions;}
        void set_tab_etats(int i,S e) {M_tab_etats[i]=e;}
        void set_tab_actions(int i, A a) {M_tab_actions[i]=a;}
        void set_reset(S (*r)(int,S*)) {reset=r;}
        void set_possible_action(int (*p)(S,A*&)) {possible_action=p;}
        void set_environnement(void (*env)(S,A,double&,S&)) {environnement=env;}
        void set_is_terminal(bool (*i)(S,int)) {is_terminal=i;}

        //constructeurs
        MDP();
        MDP(int nb_etats, int nb_actions);
        MDP(const MDP &m);

        //destructeur
        ~MDP(){
            if(M_tab_etats!= nullptr){delete [] M_tab_etats;} 
            if(M_tab_actions!= nullptr){delete [] M_tab_actions;}
        }

        //surcharges
        MDP<S,A> & operator = (const MDP<S,A> &);
        S operator [] (int);
        A operator () (int);
};


template<class S,class A>  MDP<S,A>::MDP(){
    M_nb_etats=0;
    M_nb_actions=0;
    M_tab_etats=nullptr;
    M_tab_actions=nullptr;
    reset=nullptr;
    possible_action=nullptr;
    environnement=nullptr;
    is_terminal=nullptr; 
}

template<class S,class A>  MDP<S,A>::MDP(int nb_etats, int nb_actions){
    M_nb_etats=nb_etats;
    M_nb_actions=nb_actions;
    M_tab_etats=new S[nb_etats];
    M_tab_actions=new A[nb_actions];
    reset=nullptr;
    possible_action=nullptr;
    environnement=nullptr;
    is_terminal=nullptr; 
}

template<class S,class A> MDP<S,A>::MDP(const MDP &m){
    set_nb_etats(m.get_nb_etats());
    set_nb_actions(m.get_nb_actions());
    if(m.get_tab_etats()!=nullptr){
        M_tab_etats=new S[M_nb_etats];
        for(int i=0; i<M_nb_etats; i++){
            M_tab_etats[i]=m.get_tab_etats()[i]; //m[i]
        }
    }
    if(m.get_tab_actions()!=nullptr){
        M_tab_actions=new A[M_nb_actions];
        for(int i=0; i<M_nb_actions; i++){
            M_tab_actions[i]=m.get_tab_actions()[i]; //m(i)
        }
    }
    reset=m.reset;
    possible_action=m.possible_action;
    environnement=m.environnement;
    is_terminal=m.is_terminal;
}

//surcharges
template<class S, class A> MDP<S,A> & MDP<S,A>::operator = (const MDP<S,A> & m){
    if (this != &m){
        set_nb_etats(m.get_nb_etats());
        set_nb_actions(m.get_nb_actions());
        if(M_tab_etats!= nullptr){delete [] M_tab_etats;}
        if(M_tab_actions!= nullptr){delete [] M_tab_actions;}
        if(m.get_tab_etats()!=nullptr){
            M_tab_etats=new S[M_nb_etats];
            for(int i=0; i<M_nb_etats; i++){
                M_tab_etats[i]=m[i];
            }
        }
        if(m.get_tab_actions()!=nullptr){
            M_tab_actions=new A[M_nb_actions];
            for(int i=0; i<M_nb_actions; i++){
                M_tab_actions[i]=m(i);
            }
        }
        reset=m.reset;
        possible_action=m.possible_action;
        environnement=m.environnement;
        is_terminal=m.is_terminal;
    }
    return *this;
}

//sucharge de [] pour les états
template<class S, class A> S MDP<S,A>::operator [](int i){
  return M_tab_etats[i];
}

//sucharge de () pour les actions
template<class S, class A> A MDP<S,A>::operator ()(int i){
  return M_tab_actions[i];
}

#endif