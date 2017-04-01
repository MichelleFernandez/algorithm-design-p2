// Module for calculate max benefit path
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <limits.h>
#include <algorithm>    // std::random_shuffle
#include <vector>
#include <cstdlib>
#include "graph.cpp"

using namespace std;

#define Ma 1200000000
#define Mi -1200000000

// return the node who has the minimun distance
int minDistance(int dist[], bool sptSet[], Graph g){
  // Initialize min value
  int min = INT_MIN, min_index;
  
  // find the node with min distance
  for (int v = 0; v < g.vertex; v++){
    if (sptSet[v] == false && dist[v] >= min){
      min = dist[v], min_index = v;
    }
  }
  return min_index;
}

// return the node who has the minimun distance
int minDistance2(int dist[], bool sptSet[],Graph g){
  // Initialize min value
  int min = INT_MIN, min_index;

  // find the node with minimun distance
  for (int v = 0; v < g.vertex; v++){
    if (sptSet[v] == false && dist[v] >= min){
      min = dist[v], min_index = v;
    }
  }
  
   return min_index;
}

// print all the edges of a vector
void printEdges(vector<Edge> edges){
  for(int i =0; i<edges.size(); i++){
    cout << endl << edges[i].n1 << "-" << edges[i].n2 << endl;
    cout << "c: " << edges[i].cost << " b: " << edges[i].benef << endl;
  }
}

// get the edges that belong to the shortest path
vector<Edge> filterEdges(vector<Edge> old, Graph g){

  // says if the vertex already was on the new solution
  bool ver[g.vertex+1];
  // initialize
  for (int i = 0; i < g.vertex; i++){
    ver[i] = false;
  }
  
  vector<Edge> news; // new path

  // find the shortest path on the solution
  while(old.size() !=0){
    // get the last element on the path
    Edge e = old.back();

    // if not in the new solution add it
    if(!ver[e.n2]){
      news.push_back(e);
      ver[e.n2] = true;
    }
    // delete the old path
    old.pop_back();
  }
  
  return news;
}
  
// print the distance between the deposit and the rest nodes
void printSolution(int dist[], vector<Edge> edge, Graph g) {
   printf("Vertex   Distance from Source\n");
   for (int i = 0; i < g.vertex; i++)
      printf("%d \t\t %d\n", i, dist[i]);

    printEdges(filterEdges(edge, g));
}

int getDistance(int dist[], int node){
  return dist[node];
} 


// return the farthest vertex
int minVertex(int dist[], int v){
  for (int i=v; i > 0; i--){
    if(dist[i] < Ma && dist[i] > Mi){
      return i;
    }
  }
}
// standard deviation calculation
// given the optimum value for an instance
int dHeur(int vo, int voHeur) {
    int sd = 100 * ((vo - voHeur) / vo);
    return sd;
}

std::pair<Edge,std::vector<Edge>::iterator> obtenerLado(std::vector<std::vector<Edge> > T, int b){
  
  Edge aux = T[b][0];
  int max = aux.getBenefit();
  std::vector<Edge>::iterator iter = T[b].begin();
  for(std::vector<Edge>::iterator e = T[b].begin(); e != T[b].end(); ++e){
    if(e-> getBenefit() > max){
      max = e->getBenefit();
      aux = *e;
      iter = e;
    }
  }

  return std::make_pair(aux, iter);
}

int get_c_benef(std::vector<Edge> v){
  int sum = 0;
  for(std::vector<Edge>::iterator it = v.begin(); it != v.end(); ++it){
    sum = sum + it->getBenefit();
  }
  return sum;
}

std::pair<std::vector<Edge>, bool> obtenerCamino(std::vector<std::vector<Edge> > ccm){

  std::vector<Edge> aux = ccm[0];
  int max = get_c_benef(aux);
  bool neg_cost = max <= 0;

  for(auto c = ccm.begin(); c != ccm.end(); ++c){
    if(get_c_benef(*c) > max){
      max = get_c_benef(*c);
      neg_cost = max <= 0;
      aux = *c;
    }
  }

  return std::make_pair(aux, neg_cost);
}

std::vector<Edge> unirCaminoAlCiclo(std::vector<Edge> cicle, std::vector<Edge> path){
  for(int i=0; i < path.size(); ++i){
    for(int j=0; j < cicle.size(); ++i){
      if(!(path[i] == cicle[j])){
        cicle.push_back(path[i]);
      }
    }
  }
  return cicle;
}


std::vector<std::vector<Edge> > eliminar_lados(std::vector<Edge> eds, std::vector<std::vector<Edge> >g){
  
  for (int i = 0; i < eds.size(); ++i){
    for(std::vector<Edge>::iterator it = g[eds[i].n1].begin(); it != g[eds[i].n1].end(); i++){
      if(*it == eds[i]){
        g[eds[i].n1].erase(it);
      }
    }
  }
  return g;
}

vector<Edge> maxBenefitPath(Graph graph, int deposit) {
  vector<Edge> max_benefit_path;

  int b;

  b = deposit;

  // while T set not empty
  while(!graph.t_list.empty()) {
    
    if ( !graph.t_list[b].empty() ) {
      std::pair<Edge,std::vector<Edge>::iterator> edge = obtenerLado(graph.t_list,b);
      graph.t_list[b].erase(edge.second);
      if(edge.first.getBenefit() > 0){
        max_benefit_path.push_back(edge.first); // VERIFICAR QUE SE AGREGA BIEN
        b = edge.first.n2;
      }
    } 
    else {
      vector<vector<Edge> > ccm;
      for(int v = 1; v < graph.t_list.size(); ++v){
        for(int i = 1; i < graph.t_list[v].size(); ++i){
          std::vector<Edge> cmib = graph.maxBenefit(b, graph.t_list[v][i].n2);
          ccm.push_back(cmib);
        }
      }

      std::pair<vector<Edge>,bool> c_mib = obtenerCamino(ccm);
      graph.t_list = eliminar_lados(c_mib.first, graph.t_list);

      if(!c_mib.second){
        max_benefit_path = unirCaminoAlCiclo(max_benefit_path, c_mib.first);
        b = c_mib.first.back().n2;

      }

    }
  }

  if(!max_benefit_path.back().n2 == deposit){
    std::vector<Edge> cmid = graph.maxBenefit(max_benefit_path.back().n2,deposit);
    max_benefit_path = unirCaminoAlCiclo(max_benefit_path, cmid);
  }

  return max_benefit_path;
}




int main(int argc, char **argv) {

  Graph *graph;       // instance graph structure
  string filename;    // input file name 

  int vo, voHeur;     // optimum values
  int sdHeur;         // standard deviation against given optimum value

  int deposit;

  filename = argv[1];
  // vo = atoi(argv[2]);
  deposit = atoi(argv[2]);

  int n_1 = atoi(argv[3]);

  int n_2 = atoi(argv[4]);

  // build the graph with the instance data
  graph = buildGraph(filename);

  graph->printGraph();

  // Prueba de camino de maximo beneficio desde A hasta B (dijkstra)
  vector<Edge> path = graph->maxBenefit(n_1,n_2);
  std::vector<Edge> v = maxBenefitPath(*graph,1);
  for (auto m = path.begin() ; m != path.end() ; ++m) {
    cout << '-' << '>' << m->n1 << '-' << m->n2 << ' ';
  }
  cout << '\n';

  // vector<Edge> max_benefit_path = maxBenefitPath(*graph, deposit);

  // // optimum value for the execution
  // voHeur = v+b;

  // // Standard deviation calculation
  // sdHeur = dHeur(vo, voHeur);

  // cout << voHeur << endl;
  // cout << sdHeur << endl;


  // ofstream file((filename+"-salida.txt").c_str());
  // file << voHeur << endl;
  // file.close();
  
  return 0;
}