#include <iostream>
#include <vector>
#include <set>
#include <sstream>
#include <limits>
#include <set>
#include <stdexcept>
#include <queue>
#include "cmlmatrix.h"
const int INF = std::numeric_limits<int>::max();

typedef cml::matrix<int> i32Mat;
typedef std::vector<int> i32Vec;
typedef std::set<int>    i32Set;
typedef std::pair<int,int> i32Pair;

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

void TSP(i32Mat& graph) {
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

void EuclideanDistance(const std::vector<i32Pair>& centrals, const i32Pair& newCentral) {
    int centralIndex = -1;
    float shortestDistance = MAXFLOAT;
    for(int i = 0; i < centrals.size(); i++) {
        int x = newCentral.first - centrals[i].first;
        int y = newCentral.second - centrals[i].second;
        float dist = sqrt((float)(x*x + y*y));
        if(dist <  shortestDistance) {
            shortestDistance = dist;
            centralIndex = i;
        }
    }
    std::cout << "La central más cercana a [" << newCentral.first << ", " << newCentral.second 
              << "] es [" << centrals[centralIndex].first << ", " << centrals[centralIndex].second 
              << "] con una distancia de " << shortestDistance << "." << std::endl;
}

bool bfs(i32Mat rGraph, int s, int t, int parent[]) {
    // Crear un arregli para saber que nodos se han visitado y se inician como no visitados
    bool visited[rGraph.Rows()];
    memset(visited, 0, sizeof(visited));

    std::queue<int> q;
    q.push(s);
    visited[s] = true;
    parent[s] = -1;

    // Para usar el algoritmo de FordFulkerson usaremos BFS
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v = 0; v < rGraph.Rows(); v++) {
            if (visited[v] == false && rGraph[u][v] > 0) {
                if (v == t) {
                    parent[v] = u;
                    return true;
                }
                q.push(v);
                parent[v] = u;
                visited[v] = true;
            }
        }
    }
    return false;
}

// Retorna el flujo máximo de s a t en el grafo dado
// Algoritmo de FordFulkerson
int fordFulkerson(i32Mat graph, int s, int t) {
    int u, v;

    // Crear una copia del gráfico residual
    i32Mat rGraph(graph.Rows(), graph.Cols()); // Gráfico residual donde rGraph[i][j] indica
                      // la capacidad residual de la arista de i a j (si existe
                      // una arista)
    for (u = 0; u < graph.Rows(); u++)
        for (v = 0; v < graph.Rows(); v++)
             rGraph[u][v] = graph[u][v];


    int parent[graph.Rows()];  // Este array es llenado por BFS y se utiliza para almacenar el camino
    int max_flow = 0;  // Flujo máximo empezando en 0
    //  Aumentar el flujo mientras haya camino desde la fuente hasta el sumidero
    while (bfs(rGraph, s, t, parent)) {
        int path_flow = INT_MAX;
        for (v = t; v != s; v = parent[v]) {
            u = parent[v];
            path_flow = std::min(path_flow, rGraph[u][v]);
        }

        // Actualizar las capacidades residuales de las aristas y las aristas invertidas
        for (v = t; v != s; v = parent[v]) {
            u = parent[v];
            rGraph[u][v] -= path_flow;
            rGraph[v][u] += path_flow;
        }
        // Agregar el flujo del camino al flujo total
        max_flow += path_flow;
    }
    // Regresa el flujo máximo 
    return max_flow;
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

    std::pair<int, int> newCentral;

    for(int i = 0; i < numNodes; i++) {
        std::string str;
        std::cin >> str;
        stringToPair(str, centrals[i]);

    }

    std::string str;
    std::cin >> str;
    stringToPair(str, newCentral);

    // 1.- Forma óptima de cablear la fibra óptica conenctando colonias
    // para que se pueda compartir la infotmación cualesquiera dos colonias
    std::cout<<"Punto 01"<<std::endl;
    for (int i = 0; i < numNodes; ++i) {
        dijkstra(colonies,i,numNodes);
        std::cout<<"\n";
    }

    // 2.- Ruta más corta posible para visitar cada colonia una sola vez
    std::cout << "Punto 02" << std::endl;
    TSP(colonies);
    std::cout << std::endl;

    // 3.- Flujo máximo de información del nodo inicial al nodo final.
    std::cout << "Punto 03" << std::endl;
    int source = 0, sink = numNodes - 1;
    std::cout << "Flujo máximo: " << fordFulkerson(flux, source, sink) << std::endl;
    
    // 4.- Distancia más corta entre las centrales existentes y la nueva central
    EuclideanDistance(centrals, newCentral);

    return 0;
}