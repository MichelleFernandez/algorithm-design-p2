// Module for calculate max benefit path
#include <stdio.h>
#include <stdlib.h>
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

vector<Edge> maxBenefitPath(Graph graph, int deposit) {
  vector<Edge> max_benefit_path;

  int b;

  // if deposit is not in T set
  if (graph.t_list[deposit].empty()) {
    cout << '1';
  }

  b = deposit;

  // while T set not empty
  // while(!graph.t_list.empty()) {
    
    // if ( !graph.t_edges[b].empty() ) {
    //   edge = obtainEdge();
    //   graph.t_edges[b].erase(edge);
    //   path.push_back(edge);
    //   b = edge->n2;

    // } else {
  //     vector<Edge> max_benefit_paths;
  //     vector<Edge> uv_max_benefit_path;

  //     for (int i = 1 ; i < graph.vertex+1 ; ++i) {
  //       if ( !graph.t_edges[i].empty() ) {
  //         uv_max_benefit_path = graph.maxBenefit(i,b);
  //         max_benefit_paths.push_back(uv_max_benefit_path);
  //       }
  //     }

  //     path = obtainPath(max_benefit_paths);
  //     max_benefit_path.push_back(path);
  //     graph.t_edges.erase()
    // }
  // }

  // if ( !(path.end()->n2 == deposit) ) {

  // }

  return max_benefit_path;
}


Edge obtenerLado(std::vector<std::vector<Edge> > T, int b){
  
  std::vector<Edge> prob;
  for(std::vector<Edge>::iterator e = T[b].begin(); e != T[b].end(); ++e){
    if(e-> n1 == b){
      for(int i = 0; i < e->getBenefit(); i++){
        prob.push_back(*e);
      }
    }
  }
  std::random_shuffle (prob.begin(), prob.end());
  int index = (rand() % (int)(prob.size() -1));

  return prob[index];
}

std::vector<Edge> obtenerCamino(std::vector<std::vector<Edge> > ccm){

  std::vector<std::vector<Edge> > prob;
  for(auto c = ccm.begin(); c != ccm.end(); ++c){
    for(auto e = c->begin(); e != c->end(); e++){
      for(int i = 0; i < e->getBenefit(); i++){
        prob.push_back(*c);
      }
    }
  }
  std::random_shuffle (prob.begin(), prob.end());
  int index = (rand() % (int)(prob.size() -1));

  return prob[index];
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

  // build the graph with the instance data
  graph = buildGraph(filename);

  graph->printGraph();

  // Prueba de camino de maximo beneficio desde A hasta B (dijkstra)
  vector<int> path = graph->maxBenefit(5,1);

  for (auto m = path.begin() ; m != path.end() ; ++m) {
    cout << '-' << '>' << *m << ' ';
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