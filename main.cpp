#include <iostream>
#include <vector>
#include <sstream>
#include <limits>
#include <set>
#include "cmlmatrix.h"
const int INF = std::numeric_limits<int>::max();

void stringToPair(std::string& str, std::pair<int, int>& pr) {
    str = str.substr(1, str.size() - 2);
    std::stringstream ss(str);
    std::string token;

    std::getline(ss, token, ',');
    pr.first = std::stoi(token);
    std::getline(ss, token, ',');
    pr.second = std::stoi(token);
}

void dijkstra(cml::matrix<int>& matrix, int originNode, int numNode){
    std::vector<int> distance(numNode, INF);
    distance[originNode] = 0;
    std::set<std::pair<int, int>> priorityQueue;
    priorityQueue.insert({0,originNode});

    while (!priorityQueue.empty()){
        int actualNode = priorityQueue.begin()->second;
        priorityQueue.erase(priorityQueue.begin());
        for (int neighbourNode = 0; neighbourNode < numNode; ++neighbourNode){
            if(matrix[actualNode][neighbourNode] != INF){
                int newDistance = distance[actualNode] + matrix[actualNode][neighbourNode];
                if (newDistance < distance[neighbourNode]){
                    priorityQueue.erase({distance[neighbourNode],neighbourNode});
                    distance[neighbourNode] = newDistance;
                    priorityQueue.insert({distance[neighbourNode], neighbourNode});
                }
            }
        }
    }
    for(int i = 0; i < numNode; ++i){
        if (i == originNode) continue;
        std::cout<<"Km de colonia "<< originNode + 1 << " a colonia " << i + 1 << ": " << distance[i]<<"\n";
    }
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
    std::cout<<"Punto 01"<<std::endl;
    for (int i = 0; i < numNodes; ++i) {
        dijkstra(colonies,i,numNodes);
        std::cout<<"\n";
    }

    // Borren esto, ocupen la mtríz de adyacencia "colonies[i][j]" y así 

    // 2.- Ruta más corta posible para visitar cada colonia una sola vez

    // 3.- 



    return 0;
}