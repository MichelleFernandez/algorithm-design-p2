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

pair<Edge,int> obtenerLado(vector<vector<Edge> > T, int b){
  
  int pos = 0;
  int max_pos = 0;
  Edge aux = T[b][0];
  int max = aux.getBenefit();
  vector<Edge>::iterator iter = T[b].begin();
  for(vector<Edge>::iterator e = T[b].begin(); e != T[b].end(); ++e){
    if(e-> getBenefit() > max){
      max = e->getBenefit();
      aux = *e;
      iter = e;
      max_pos = pos;
    }
    pos +=1;
  }

  return std::make_pair(aux, max_pos);
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
  for(int i=0; i < path.size() ; ++i){
    for(int j=0; j < cicle.size() ; ++j){
      if(!(path[i] == cicle[j])){
        cicle.push_back(path[i]);
      }
    }
  }
  return cicle;
}

std::vector<Edge> unirCaminoAlCiclo2(std::vector<Edge> cicle, std::vector<Edge> path){

  // std::vector<int>::reverse_iterator rit = path.rbegin();
  // for (; rit!= myvector.rend(); ++rit)
  //   *rit = ++i;

  for (auto m =path.rbegin() ; m != path.rend() ; ++m) {
    Edge edge(m->n2, m->n1, m->cost, m->benef);
    cicle.push_back(edge);  
  }

  return cicle;
}

vector<Edge> maxBenefitPath(Graph graph, int deposit) {
  vector<Edge> max_benefit_path;

  int b;

  b = deposit;

  // falta if deposit not in t

  if (graph.t_list[b].empty()) {
    pair<Edge,int> edge1 = obtenerLado(graph.a_list, b);
    max_benefit_path.push_back(edge1.first);
    b = edge1.first.n2;
  }

  int i = 0;
  // while T set not empty
  while(!graph.tEmpty()) {
    if ( !graph.t_list[b].empty() ) {
      pair<Edge,int> edge = obtenerLado(graph.t_list,b);
      graph.eraseEdge(edge.first);
      if (edge.first.getBenefit() > 0){
        max_benefit_path.push_back(edge.first);
        graph.crossEdge(edge.first);
        b = edge.first.n2;
      }
    } 
    else {
      vector<vector<Edge> > ccm;
      for(int i = 1; i < graph.t_list.size(); ++i){
        if (!graph.t_list[i].empty()) {
          std::vector<Edge> cmib = graph.maxBenefit(i, b);          
          ccm.push_back(cmib);
        }
      }

      std::pair<vector<Edge>,bool> c_mib = obtenerCamino(ccm);
      graph.eraseEdges(c_mib.first);

      max_benefit_path = unirCaminoAlCiclo2(max_benefit_path, c_mib.first);
      graph.crossEdges(c_mib.first);
      b = c_mib.first[0].n1;
    }
  }


  if(!(b == deposit)){
    std::vector<Edge> cmid = graph.maxBenefit(deposit,b);
    max_benefit_path = unirCaminoAlCiclo2(max_benefit_path, cmid);
  }

  return max_benefit_path;
}


// int main(int argc, char **argv) {

//   Graph *graph;       // instance graph structure
//   string filename;    // input file name 

//   int vo, voHeur;     // optimum values
//   int sdHeur;         // standard deviation against given optimum value

//   int deposit;

//   filename = argv[1];
//   deposit = atoi(argv[2]);

//   // build the graph with the instance data
//   graph = buildGraph(filename);

//   vector<Edge> path = maxBenefitPath(*graph, deposit);

//   // for(auto c = path.begin(); c != path.end(); ++c){
//   //   cout << c->n1;
//   //   cout << c->n2;
//   //   cout << '\n';
//   // }

//   return 0;
// }