#include <iostream>
#include <cmath>
#include <random>
#include <cstdlib>

#include "MDP_parfait.hpp"

using namespace std;

void init_etats(MDP<int,int> &m){
    m.set_tab_etats(0,0);
    m.set_tab_etats(1,1);
}

void init_actions(MDP<int,int> &m){
    m.set_tab_actions(0,0);
    m.set_tab_actions(1,1);
    m.set_tab_actions(2,2);
}

void init_P(MDP_parfait<int,int> &m){
    for(int i=0; i<m.get_nb_etats(); i++){
        for(int j=0; j<m.get_nb_actions(); j++){
            for(int k=0; k<m.get_nb_etats(); k++){
                m.set_P(i,j,k,0.);
            }
        }
    }
    m.set_P(0,0,0,0.1);
    m.set_P(0,0,1,0.9);
    m.set_P(1,0,0,0.9);
    m.set_P(1,0,1,0.1);
    m.set_P(1,1,1,1.0);
    m.set_P(0,1,0,1.0);
    m.set_P(0,2,1,1.0);
}

void init_R(MDP_parfait<int,int> &m){
    for(int i=0; i<m.get_nb_etats(); i++){
        for(int j=0; j<m.get_nb_actions(); j++){
            for(int k=0; k<m.get_nb_etats(); k++){
                m.set_R(i,j,k,0);
            }
        }
    }
    m.set_R(0,0,0,2.0);
    m.set_R(0,0,1,-3.0);
    m.set_R(1,0,0,2.0);
    m.set_R(1,0,1,2.0);
    m.set_R(1,1,1,1.0);
    m.set_R(0,1,0,1.0);
    m.set_R(0,2,1,0.);
}

int robot_reset(int nb_etats,int* tab_etats){
    return tab_etats[rand()%(nb_etats)];
}

int robot_possible_action(int s,int*& tab_A){
    if(s==0){
        tab_A=new int[3]; tab_A[0]=0; tab_A[1]=1; tab_A[2]=2;
        return 3;
    }
    else if(s==1){
        tab_A=new int[2]; tab_A[0]=0; tab_A[1]=1;
        return 2; //car si haut, le robot ne peut pas recharger
    }
    else{
        tab_A=nullptr;
        return -1;
    }
}

void robot_environnement(int s,int a,double& r,int& s_p){
    if (s >= 2 or a >= 3 or s < 0 or a < 0) { 
        std::cout << "s : " << s << ", a : " << a << std::endl;
    }
    MDP_parfait<int,int> temp(2,3);
    init_P(temp); init_R(temp);
    double alpha=temp.get_P(s,a,0);
    random_device rd;
    default_random_engine re(rd());
    uniform_real_distribution<double> distrib(0,1.0);
    double nb_alea=distrib(re);

    if(nb_alea<alpha){
        s_p=0;
        r=temp.get_R(s,a,s_p);
    }
    else{
        s_p=1;
        r=temp.get_R(s,a,s_p);
    }
}

bool robot_is_terminal(int s,int n){
    return (n>30);
}