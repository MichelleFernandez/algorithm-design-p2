// Module for class definition
#include <iostream>
#include <string.h>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <vector>
  
using namespace std;


// Edge class
class Edge {

  public:
  
  int n1;       // first node
  int n2;       // second node 
  int cost;     // cost of crossing 
  int benef;    // benefit of crossing 
  bool crossed; // says if tt was crossed


  Edge(int n1, int n2, int c, int b, bool cross = false) {
    this->n1 = n1;
    this->n2 = n2;
    this->cost = c;
    this->benef = b;
    this->crossed = cross;
  }

  // defintion of comparator operator
  bool operator==(const Edge &e1) const {
    return n1 == e1.n1 && n2 == e1.n2 &&
      cost == e1.cost && benef == e1.benef
      && crossed == e1.crossed;
  }

  // return the benefit of an edge depending on
  // if the edge was already crossed
  int getBenefit() {
    if (!crossed) {
      return benef-cost;
    }
    return -cost;
  }

  void setCrossed() {
    crossed = true;
  }

  int get_cost(){
    return cost;
  }

  int get_benefit(){
    return benef;
  }
};

// Graph class
class Graph {

  public:

  int vertex;           // num of vertices
  int edges;            // num of edges 

  vector<vector<Edge> > a_list;  // lists of edges
  vector<vector<Edge> > t_list;  // list of R U Q edges


  Graph(int vertex, int edges, vector<vector<Edge> > &eds, vector<vector<Edge> > &t_eds) { 
    this->vertex = vertex; 
    this->edges = edges;
    this->a_list = eds;
    this->t_list = t_eds;
  }

  bool eraseEdge(Edge edge) {
    auto m = this->t_list[edge.n1].begin();

    while (m->n2 != edge.n2) {
      cout << m->n2;
      ++m;
    }

    if (m->n2 == edge.n2) {
      t_list[edge.n1].erase(m);
    } else {
      return false;
    }

    m = this->t_list[edge.n2].begin();

    while (m->n2 != edge.n1) {
      ++m;
    }

    if (m->n2 == edge.n1) {
      t_list[edge.n2].erase(m);
      return true;
    } else {
      return false;
    }    
  }

  bool tEmpty() {
    if (t_list.empty()) return true;

    for (auto n=this->t_list.begin(); n != this->t_list.end() ; ++n) {
      if (!n->empty()) return false;
    }

    return true;
  }


  /*
     maxBenefit
     dijkstra maximum benefit path from source to destination vertices
  */
  vector<Edge> maxBenefit(int src, int dst) {
  // int maxBenefit(int src, int dst) {  
    vector<Edge> path;    
    int vertices_num = this->vertex;

    int benefit[vertices_num +1];
    bool visited[vertices_num +1];

    int u, v;
    Edge next_vertex(0,0,0,0);
    int uv_benefit;
    int max_benefit = -120000;
    int visited_num = 0;

    // initialize distances with the minimum int value
    // and visited with false
    for (int i = 0 ; i <= vertices_num ; ++i) {
      benefit[i] = -120000;
      visited[i] = false;
    }

    // distance of source is always zero
    benefit[src] = 0;

    // set source vertex as starting point
    u = src;
    //path.push_back(u);

    while (u != dst && visited_num <= vertices_num) {
      // mark next vertex as visited
      visited[u] = true;
      visited_num += 1;

      // for every edge leaving from u, analyze current benefit of v against 
      // new benefit possible benefit given by the u-v path
      for (auto e = this->a_list[u].begin() ; e != this->a_list[u].end() ; ++e) {
        v = e->n2;
        uv_benefit = benefit[u] +e->getBenefit();

        // if the vertex doesn't have its maximum benefit already calculated (visited)
        // and the newly calculated benefit is bigger than its current, 
        // set it as the new current maximum benefit of v 
        if ( !visited[v] && benefit[v] < uv_benefit ) {
          benefit[v] = uv_benefit;

          // find the v vertex with maximum benefit of leaving from u.
          if (uv_benefit > max_benefit) {
            max_benefit = uv_benefit;
            next_vertex = *e;
          }
        }
      }

      // add  to maximum benefit path
      path.push_back(next_vertex);

      // set values for next iteration
      u = next_vertex.n2;
      max_benefit = -120000;
    }

    // return benefit[dst];
    return path;
  }

  // return the benefit of an edge composse for the nodes
  // u and v
  // int getTotalBenefit(int u, int v){
  //   for(int i =0; i < a_list.size(); i++){
  //     if(a_list[i].n1 == u && a_list[i].n2 == v){
  //       return a_list[i].getBenefit();
  //     }
  //   }
  // }

  // verify if an edge (u,v) exist on the graph
  // bool edgeExist(int u, int v){
  //   for (int i=0; i< a_list.size(); i++){
  //     if(a_list[i].n1 == u && a_list[i].n2 == v){
  //       return true;
  //     }
  //   }
  //   return false;
  // }

  // mark an edge as crossed
  bool crossEdge(Edge edge) {
    auto m = this->t_list[edge.n1].begin();

    while (m->n2 != edge.n2) {
      cout << m->n2;
      ++m;
    }

    if (m->n2 == edge.n2) {
      m->setCrossed();
    } else {
      return false;
    }

    m = this->t_list[edge.n2].begin();

    while (m->n2 != edge.n1) {
      ++m;
    }

    if (m->n2 == edge.n1) {
      m->setCrossed();
      return true;
    } else {
      return false;
    }  
  }

  void crossEdges(vector<Edge> edges) {
    for (auto m = edges.begin() ; m != edges.end() ; ++m) {
      m->setCrossed();
    }
  }

  // return the edge composse by the nodes u,v
  // Edge getEdge(int u, int v){
  //   for (int i=0; i< a_list.size(); i++){
  //     if(a_list[i].n1 == u && a_list[i].n2 == v){
  //       return a_list[i];
  //     }
  //   }
  // }


  /*
     printGraph
     dijkstra maximum benefit path from source to destination vertices
  */
  void printGraph() {
    // required edges (T set)
    cout << 'T' << ':' << '\n';
    int i = 0;
    for (auto n=this->t_list.begin(); n != this->t_list.end() ; ++n) {
      cout << '[' << i << ']' << '\n';
      for (auto m = n->begin() ; m != n->end() ; ++m) {
        cout << m->n1 << ' ';
        cout << m->n2 << ' ';
        cout << m->cost << ' ';
        cout << m->benef << ' ';
        cout << '\n';
      }
      ++i;
    }

    cout << '\n';

    // all edges
    i = 0;
    for (auto n=this->a_list.begin(); n != this->a_list.end() ; ++n) {
      cout << '[' << i << ']' << '\n';
      for (auto m = n->begin() ; m != n->end() ; ++m) {
        cout << m->n1 << ' ';
        cout << m->n2 << ' ';
        cout << m->cost << ' ';
        cout << m->benef << ' ';
        cout << m->crossed << ' ';        
        cout << '\n';
      }
      ++i;
    }
  }
};


/*
   buildGraph
   reads an input graph instance and builds the corresponding graph
*/
Graph * buildGraph(string filename) {

  // Auxiliar variables to ease file reading
  stringstream sstream;
  string line; 
  string tmp;

  int vertex, edges, n_edges;
  int n1, n2;
  int cost, benef;
  char c;

  vector<vector<Edge> > t_eds;  // R U Q edges vector

  ifstream file(filename.c_str());  // file stream

  if ( file.is_open() ) {

    getline(file, line);
    
    sstream.str(line);
    sstream >> tmp >> tmp >> tmp >> c >> vertex;
    
    cout << vertex << "\n";
    
    vector<vector<Edge> > eds(vertex+1);  // edges vector

    getline(file, line);

    sstream.clear();  
    sstream.str(line);
    sstream >> tmp >> tmp >> tmp >> tmp >> edges;

    for ( int i = 0 ; i < edges ; ++i ) {
      getline(file, line);

      sstream.clear();
      sstream.str(line);

      sstream >> n1;
      sstream >> n2;
      sstream >> cost;
      sstream >> benef;

      Edge edge1(n1, n2, cost, benef);
      Edge edge2(n2, n1, cost, benef);

      // add edges to corresponding edges list for both vertices
      eds[n1].push_back(edge1);
      eds[n2].push_back(edge2);
    }

    // T = R U Q edges set 
    t_eds = eds;

    getline(file, line);
    sstream.clear();  
    sstream.str(line);
    sstream >> tmp >> tmp >> tmp >> tmp >> tmp >> n_edges;

    edges += n_edges;

    for ( int i = 0 ; i < n_edges ; ++i ) {
      getline(file, line);

      sstream.clear();
      sstream.str(line);

      sstream >> n1;
      sstream >> n2;
      sstream >> cost;
      sstream >> benef;

      Edge edge1(n1, n2, cost, benef);
      Edge edge2(n2,n1, cost, benef);
      eds[n1].push_back(edge1);
      eds[n2].push_back(edge2);
    }

    static Graph graph(vertex, edges*2,eds, t_eds);

    return &graph;

    }  else cout << "Unable to open file"; 

    return NULL;
}