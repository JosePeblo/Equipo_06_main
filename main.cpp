#include <iostream>
#include <vector>
#include <sstream>
#include "cmlmatrix.h"

void stringToPair(std::string& str, std::pair<int, int>& pr) {
    str = str.substr(1, str.size() - 2);
    std::stringstream ss(str);
    std::string token;

    std::getline(ss, token, ',');
    pr.first = std::stoi(token);
    std::getline(ss, token, ',');
    pr.second = std::stoi(token);
}

int main(void) {
    int numNodes;
    std::cin >> numNodes;

    cml::matrix<int> colonies(numNodes, numNodes);
    for(int i = 0; i < numNodes; i++) {
        for(int j = 0; j < numNodes; j++) {
            std::cin >> colonies[i][j];
        }
    }

    cml::matrix<int> flux(numNodes, numNodes);
    for(int i = 0; i < numNodes; i++) {
        for(int j = 0; j < numNodes; j++) {
            std::cin >> flux[i][j];
        }
    }

    std::vector<std::pair<int, int>> centrals(numNodes);

    std::pair<int, int> home;

    for(int i = 0; i < numNodes; i++) {
        std::string str;
        std::cin >> str;
        stringToPair(str, centrals[i]);

    }

    std::string str;
    std::cin >> str;
    stringToPair(str, home);

    // Aquí empezamos 

    // 1.- Forma óptima de cablear la fibra óptica conenctando colonias
    // para que se pueda compartir la infotmación cualesquiera dos colonias

    // Borren esto, ocupen la mtríz de adyacencia "colonies[i][j]" y así 

    // 2.- Ruta más corta posible para visitar cada colonia una sola vez

    // 3.- 



    return 0;
}