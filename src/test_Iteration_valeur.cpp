#include <iostream>
#include <cstdlib>

#include "MDP_parfait.hpp"
#include "IterationValeur.hpp"
#include "utils.hpp"

int recompense_totale_avec_politique(MDP_parfait<int,int>* m, IterationValeur<int,int>* v){
    int s_t=m->get_reset(2,m->get_tab_etats());
    int s_t_p=0; //état suivant
    int a_t=0;
    double r_t=0.; //récompense
    double r_tot=0.;
    int t=0;
    while(t<100){
        a_t=v->applique_politique(s_t);
        m->get_environnement(s_t,a_t,r_t,s_t_p);
        r_tot+=r_t;
        s_t=s_t_p;
        if(m->get_is_terminal(s_t,t)){
            break;
        }
        t++;
    }
    return r_tot;
}

int recompense_totale_sans_politique(MDP_parfait<int,int>* m){
    int s_t=m->get_reset(2,m->get_tab_etats());
    int s_t_p=0; //état suivant
    int a_t=0;
    int* tab_act_possible;
    int nb_act_possible;
    double r_t=0.; //récompense
    double r_tot=0.;
    int t=0;
    while(t<100){
        nb_act_possible=m->get_possible_action(s_t,tab_act_possible);
        a_t=tab_act_possible[rand()%(nb_act_possible)];
        m->get_environnement(s_t,a_t,r_t,s_t_p);
        r_tot+=r_t;
        s_t=s_t_p;
        if(m->get_is_terminal(s_t,t)){
            break;
        }
        t++;
    }
    return r_tot;
}