#include <iostream>
#include <cstdlib>

#include "MDP_parfait.hpp"
#include "Qlearning.hpp"
#include "utils.hpp"

int recompense_totale_q_learning(MDP<int,int>* m, Q_learning<int,int>* q){
    int s_t=m->get_reset(2,m->get_tab_etats());
    int s_t_p=0; //état suivant
    int a_t=0;
    double r_t=0.; //récompense
    double r_tot=0.;
    int t=0;
    while(t<100){
        a_t=q->applique_politique_opt(s_t);
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