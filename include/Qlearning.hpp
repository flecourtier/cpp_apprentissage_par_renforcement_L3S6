#ifndef QLEARNING_HPP
#define QLEARNING_HPP

#include <iostream>
#include <cmath>

#include "MDP.hpp"
#include "ModeleSto.hpp"

template<class S, class A> class Q_learning : public ModelSto<S,A> {
    protected:
        double M_alpha;
    public:
        //constructeurs
        Q_learning() : ModelSto<S,A>() {
            M_alpha=0.;
        }

        Q_learning(const Q_learning &q) : ModelSto<S,A>(q) {
            M_alpha=q.get_alpha();
        }

        Q_learning(MDP<S,A>* m) : ModelSto<S,A>(m) {
            M_alpha=0.;
        }

        //destructeur
        ~Q_learning(){};

        //accesseur
        double get_alpha() const {return M_alpha;}

        //mutateur
        void set_alpha(double a){M_alpha=a;}

        virtual void calcul_episodes(S s){
            bool terminal=false;
            int t=0;
            S s_t=s;
            A a_t;
            S s_t_p;
            double r_t;
            A a_opt;
            while(terminal==false){
                if(this->M_modele->get_is_terminal(s_t,t)==true){
                    terminal=true;
                }
                a_t=this->applique_politique_alea(s_t);
                this->M_modele->get_environnement(s_t,a_t,r_t,s_t_p);
                a_opt=this->applique_politique_opt(s_t_p);
                if(this->M_modele->get_is_terminal(s_t,t)==true){
                    this->M_Q[this->index_etat(s_t)][this->index_action(a_t)]+=M_alpha*r_t;
                }
                else{
                    this->M_Q[this->index_etat(s_t)][this->index_action(a_t)]+=M_alpha*(r_t+this->M_gamma*this->M_Q[this->index_etat(s_t_p)][this->index_action(a_opt)]);
                }
                s_t=s_t_p;
                this->M_eps=this->M_d*this->M_eps;
                t++;
                this->calcul_politique_alea();
                this->calcul_politique_opt();
            }
        }
};

#endif