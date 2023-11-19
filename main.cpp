#include <iostream>
#include <vector>
#include <set>
#include <sstream>
#include <limits>
#include <set>
#include <stdexcept>
#include "cmlmatrix.h"
const int INF = std::numeric_limits<int>::max();

typedef cml::matrix<int> i32Mat;
typedef std::vector<int> i32Vec;
typedef std::set<int>    i32Set;

struct Path {
    i32Vec path;
    int weigth = 0;
};

Path ClosestNeigbour(i32Mat& graph, i32Set nodes, int start) {
    Path p;
    nodes.erase(start);
    p.path.push_back(start); 
    int currNode = start;
    while(nodes.size() > 0) {
        int bestNode = -1;
        int bestWeight = INT32_MAX;

        for(auto node : nodes) {
            int weight = graph[currNode][node];
            if(weight < bestWeight) {
                bestNode = node;
                bestWeight = weight;
            }
        }

        p.path.push_back(bestNode);
        nodes.erase(bestNode);
        p.weigth += bestWeight;
        currNode = bestNode;

    }
    if(graph[currNode][start] <= 0) {
        throw std::runtime_error("No hamiltonian path");
    }
    p.weigth += graph[currNode][start];
    p.path.push_back(start);
    
    return p;

}

void TSP(cml::matrix<int>& graph) {
    int numNodes = graph.Rows();
    std::vector<Path> paths;
    paths.reserve(numNodes);
    i32Set nodes;

    int bestPathIndex = -1;
    int bestWeight = INT32_MAX;

    for(int i = 0; i < numNodes; i++) {
        nodes.emplace(i);
    }

    for(int i = 0; i < numNodes; i++) {
        paths.emplace_back(ClosestNeigbour(graph, nodes, i));
        if(paths[i].weigth < bestWeight) {
            bestWeight = paths[i].weigth;
            bestPathIndex = i;
        }
    }

    if(bestPathIndex > -1) {
        std::cout << "El recorrido:" << std::endl;

        auto bestPath = paths[bestPathIndex];
        for(int i = 0; i < bestPath.path.size(); i++) {
            std::cout << bestPath.path[i] + 1;
            if(i + 1 < bestPath.path.size()) {
                std::cout << " -> ";
            }
        }

        std::cout << std::endl;
        std::cout << "El costo: " << bestPath.weigth << std::endl;
    } else {
        std::cout << "Error: no hay un mejor camino" << std::endl;
    }

}

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
    std::cout << "Punto 02" << std::endl;
    TSP(colonies);

    // 3.- 



    return 0;
}