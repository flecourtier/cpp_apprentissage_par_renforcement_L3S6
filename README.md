# cpp_apprentissage_par_renforcement_L3S6
Projet de C++ guidé en L3S6 : apprentissage par renforcement

Sujet proposé par Emmanuel Franck (voir "sujet_projet.pdf")

Il s'agit d'un environnement où un agent va effectuer une série d'action au cours du temps. L'agent à un temps t est dans un état <img src="https://render.githubusercontent.com/render/math?math=s_t\in S"> et peut effectuer une action <img src="https://render.githubusercontent.com/render/math?math=a_t\in A">. Cette action lui permet de se retrouver dans un nouvel état <img src="https://latex.codecogs.com/gif.latex?s_{t+1}"/> et il obtient une récompense <img src="https://latex.codecogs.com/gif.latex?r_{t+1}"/> qui va évaluer l'action et/ou le nouvel état. Ce nouvel état ainsi que la récompense est donnée par l'environnement. Le processus continu jusqu'à ce qu'un état terminal soit atteint. L'objectif est de trouver la meilleure série d'action effectuée afin de maximiser les récompenses.
