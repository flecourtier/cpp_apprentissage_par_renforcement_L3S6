#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <cmath>

// pour afficher un tableau simple
template<class T> void display(T tab[], int n){ 
    if(tab!=nullptr){
        std::cout << "[";
        for (int i=0; i<n-1; i++){
            std::cout << tab[i] << ",";
        }
        if(n!=0){
            std::cout << tab[n-1];
        }
        std::cout << "]" << std::endl;
    }
}

// pour afficher un tableau 2d
template<class T> void display_2d(T **tab, int nx, int ny){
	std::cout << "[";
	for (int i=0; i<nx-1; i++){
		std::cout << "[";
		for (int j=0; j<ny-1; j++){
			std::cout << tab[i][j] << ",";
		}
		if(ny!=0){
			std::cout << tab[i][ny-1];
		}
		std::cout << "]," << std::endl;
	}
	if(nx!=0){
		std::cout << "[";
		for (int j=0; j<ny-1; j++){
			std::cout << tab[nx-1][j] << ",";
		}
		if(ny!=0){
			std::cout << tab[nx-1][ny-1];
		}
		std::cout << "]";
	}
	std::cout << "]" << std::endl;
}

//pour vérifier si un élément appartient à un tableau
//dans notre cas si une action est dans le tableau des action possible
//( valable seulement pour les types numériques (int, double ...) )
template<class T> bool is_in(T* tab, int n, T elem){
	for(int i=0; i<n; i++){
		if(abs(tab[i]-elem)<pow(10,-4)){
			return true;
		}
	}
	return false;
}

#endif