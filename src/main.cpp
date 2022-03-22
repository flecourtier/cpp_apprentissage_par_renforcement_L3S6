#include <iostream>
#include <cstdlib>

#include "MDP_parfait.hpp"
#include "IterationValeur.hpp"
#include "Qlearning.hpp"
#include "utils.hpp"

#include "MDP_parfait_exemple.cpp"
#include "test_Iteration_valeur.cpp"
#include "test_Q_learning.cpp"

int main() {
    //PREMIERE PARTIE :

    MDP_parfait<int, int> mp(2,3);
    mp.set_reset(robot_reset);
    mp.set_possible_action(robot_possible_action);
    mp.set_environnement(robot_environnement);
    mp.set_is_terminal(robot_is_terminal);
    init_etats(mp);
    init_actions(mp);
    init_P(mp);
    init_R(mp);

    MDP<int, int> m(2,3);
    m.set_reset(robot_reset);
    m.set_possible_action(robot_possible_action);
    m.set_environnement(robot_environnement);
    m.set_is_terminal(robot_is_terminal);
    init_etats(m);
    init_actions(m);

    //DEUXIEME PARTIE :

    IterationValeur<int,int> v(&mp);
    v.set_gamma(0.95);
    v.resolution();
    v.calcul_politique();

    //avec politique
    int recompense_tot=recompense_totale_avec_politique(&mp,&v);

    //sans politique (moyenne sur 10000 essais)
    srand((int)time(0));
    double moyenne=0.;
    for(int i=0; i<10000; i++){
        moyenne+=recompense_totale_sans_politique(&mp);
    }
    moyenne=moyenne/10000.;

    std::cout << "Récompense avec politique : " << recompense_tot << std::endl;
    std::cout << "Moyenne des récompense sans politique : " << moyenne << std::endl;

    //TROISIEME PARTIE :

    /*
    Nous avons choisi ici d'effectuer comme pour la partie 2 une moyenne des récompenses
    sur 500 essais mais également déterminer quel(s) mu_opt apparaissent le plus. Pour 
    cela nous avons décidé de récupérer le nombre de fois où chaque mu_opt possible
    apparait sur ces 500 essais et d'en faire un pourcentage. A noter qu'on constate
    bien que les mu_opt avec la valeur 2 en deuxième position ont tous un pourcentage
    nul, ce qui est correct car il n'est pas possible de recharger dans le cas où la
    batterie est haute.
    Le dossier résultat ne fait pas à proprement parlé parti du travail demandé mais
    nous allons tout de même vous expliquer un peu ce que l'on a fait. Pour déterminer 
    les politiques les plus fréquentes, nous avons rediriger la sortie du terminal 
    vers un fichier "sortie" afin de récupérer au travers d'un script python 
    "resultat.py" les pourcentages des mu_opt obtenus. 
    Nous avons ensuite généré un graphique à barres dans "graphique_resultat.py"
    où l'on peut voir que la meilleure action pour un niveau de batterie bas est 
    de recharger et que pour un niveau haut, c'est soit d'aller chercher soit 
    d'attendre (ce qui correspond au mu_opt [2,0] et [2,1]) ce qui parrait tout à fait
    cohérent. 
    De plus, on pourrait se demander maintenant ce que notre robot ferait si
    on lui ajoute un état supplémentaire correspondant par exemple à un niveau de
    batterie intermédiaire. Chaque action aurait alors la possibilité d'être la
    meilleure pour un état.
    */

    double moyenne_rec=0.;
    int* total_mu_opt=new int[9];
    int a0,a1;
    for(int i=0; i<9; i++){
        total_mu_opt[i]=0;
    }
    for(int i=0; i<500; i++){
        Q_learning<int,int> q(&m);
        q.set_gamma(0.95);
        q.set_alpha(0.5);
        q.set_eps(1.0);
        q.set_d(0.98);
        q.calcul_politique_opt();
        q.calcul_politique_alea();
        q.apprentissage(2);
        moyenne_rec+=recompense_totale_q_learning(&m,&q);
        
        a0 = q.get_mu_opt(0);
        a1 = q.get_mu_opt(1);
        total_mu_opt[a0 * m.get_nb_actions() + a1 ] += 1;
    }
    moyenne_rec=moyenne_rec/500.;

    double* pourcentage_mu_opt=new double[9];
    for(int i=0; i<9; i++){
        pourcentage_mu_opt[i]=(total_mu_opt[i]/500.)*100.;
    }

    std::cout << "Pourcentage des mu_opt : " << std::endl;
    for(int i_a0=0; i_a0<m.get_nb_actions(); i_a0++){
        for(int i_a1=0; i_a1<m.get_nb_actions(); i_a1++){
            std::cout << "[" << i_a0 << "," << i_a1<< "] : " << pourcentage_mu_opt[i_a0 * m.get_nb_actions() + i_a1] << " %" << std::endl;
        }
    }

    std::cout << "Moyenne des récompenses par apprentissage : " << moyenne_rec << std::endl;

    return 0;
}