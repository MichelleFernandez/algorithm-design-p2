#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <vector>
#include <unordered_set>
#include <ctime>
#include "max_benefit.cpp"

time_t t_start;
time_t t_end;
double dif;
std::vector<Edge> solParcial;
std::vector<Edge> mejorSol; // se obtiene de max_benefit
int beneficioDisponible;
std::unordered_set<int> solP = {1};

bool compare_edges(Edge e1, Edge e2){
	return(e1.getBenefit() > e2.getBenefit());
}

//ya
std::vector<Edge> obtener_lista_de_sucesores(int v, std::vector<vector<Edge> > g){
	std::vector<Edge> L;
	for(std::vector<Edge>::iterator e = g[v].begin(); e != g[v].end(); ++e){
		L.push_back(*e);
		Edge e1(e -> n1, e-> n2, e -> get_cost(), 0);
		L.push_back(e1);
	}
	std::sort (L.begin(), L.end(), compare_edges);
	return L;
}

// ya
int beneficio(std::vector<Edge> g){
	int v = 0;
	int b = 0;
	while(!g.empty() and v < g.size()){
		//Edge e = g[v];
		//g.erase(g.begin());
		b += (g[v].getBenefit());
		v += 1;
	}
	return b;
}

//ya
bool ciclo_negativo(Edge e, std::vector<Edge> solParcial){
	
	std::vector<Edge> aux;
	std::copy(solParcial.begin(), solParcial.end(), std::back_inserter(aux));
	std::unordered_set<int>::iterator it = solP.find(e.n2);
	if(it == solP.end()){
		return false; // no es ciclo
	}
	else{
		aux.push_back(e);
		if(beneficio(aux) < 0){
			return true;
		}
		else{
			return false;
		}
	}
}

//ya
bool esta_lado_en_sol_parcial(Edge e, std::vector<Edge> solParcial){

	int count = 0;
	for(int i = 0; i < solParcial.size(); ++i){
		if(e == solParcial[i]){
			count += 1;
		}
	}
	if (count == 0){
		return false;
	} else if (count == 1){
		if(e.get_benefit() == 0){
			return false;
		}
		else{
			return true;
		}
	}
	else{
		return true;
	}
}

// ya
Edge find_edge(int n1, std::vector<Edge> solParcial){
	for (int i = 0; i < solParcial.size(); ++i){
		if(solParcial[i].n1 == n1){
			return solParcial[i];
		}
	}
}

void cleanPath(std::vector<Edge> &path){
	for(std::vector<Edge>::iterator i = path.begin(); i !=path.end(); ++i){
		for(std::vector<Edge>::iterator i2 = path.begin(); i2 !=path.end(); ++i2){
			if(i->n1 == i2->n2 && i->n2 == i2->n1){
				cout << i->n1 << "-"<< i->n2 << " " << i2->n1 << "-" << i2->n2 << "\n";
				//i2->crossed = true;
			}	
		}
	}
}

//ya
bool repite_ciclo(Edge e, std::vector<Edge> solParcial){

	std::unordered_set<int>::iterator it = solP.find(e.n2);
	if(it == solP.end()){
		return false; // no es ciclo
	}
	else{
		if(solParcial.back().n1 == e.n2){
			return true;
		}
	}
	return false;
}

//ya
bool cumple_acotamiento(Edge e, std::vector<Edge> &solParcial){
	int beneficioE = e.get_benefit() - e.get_cost();
	int beneficioSolParcial = beneficio(solParcial) + beneficioE;
	int maxBeneficio = beneficioDisponible - max(0, beneficioE) + beneficioSolParcial;
	if(maxBeneficio < beneficio(mejorSol)-1){
		return false;
	}
	else{
		return true;
	}
}

//ya
void agregar_lado(Edge e, std::vector<Edge> &solParcial){
	solParcial.push_back(e);
	solP.insert(e.n2);
}

//ya
Edge eliminar_ultimo_lado(std::vector<Edge> &solParcial){

    Edge last = solParcial.back();
    Edge e(last.n1, last.n2, last.cost, last.benef, last.crossed);
    solParcial.erase(solParcial.end());

    return e;
}

void cambiar_cross(Edge e, std::vector<std::vector<Edge> > &g){
	for(std::vector<Edge>::iterator it = g[e.n2].begin(); it != g[e.n2].end(); ++it){
		if(e.n1 == it->n2){
			it->crossed = true;
		}
	}
}

std::vector<Edge> busqueda_en_profundidad(int v, std::vector<vector<Edge> > g){
	// vemos si se encuentra una mejor solución factible
	if (v == 1){ //1 es el depósito
		if(beneficio(solParcial) > beneficio(mejorSol)){
			std::copy(solParcial.begin(), solParcial.end(), mejorSol.begin());
		}
	} 

	std::vector<Edge> L = obtener_lista_de_sucesores(v,g); //lista de aristas
	int ce;
	int be;
	for(int i =0; i < L.size(); i++){
		// copio L[i] en e
		Edge e(L[i].n1, L[i].n2, L[i].cost, L[i].benef, L[i].crossed);
		if(!ciclo_negativo(e, solParcial) &&
			!esta_lado_en_sol_parcial(e, solParcial) &&
			!repite_ciclo(e, solParcial) &&
			cumple_acotamiento(e, solParcial)){
				agregar_lado(e,solParcial);
				cambiar_cross(e,g);
				ce = e.get_cost();
				be = e.get_benefit();
				beneficioDisponible = beneficioDisponible - max(0, be-ce);
				t_end = time(NULL);
				dif= difftime(t_end,t_start);
				if(dif> 108000){ // 1/2 hora 
					cout << "-T-\n";
					exit(0);
				}
				mejorSol = busqueda_en_profundidad(e.n2, g);
		}
	}

	Edge e_c = eliminar_ultimo_lado(solParcial);
	ce = e_c.get_cost();
	be = e_c.get_benefit();
	beneficioDisponible = beneficioDisponible + max(0, be-ce);
	return mejorSol;
}

int main(int argc, char **argv){
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

  	//graph->printGraph();

  	mejorSol = maxBenefitPath(*graph, deposit);

  	//cleanPath(mejorSol);

  	for(std::vector<Edge>::iterator i = mejorSol.begin(); i != mejorSol.end(); ++i){
  		cout << "Edge: " << i->n1 << "-" << i->n2 << " cross: " << i->crossed << "\n";
  	}

  	//beneficioDisponible = beneficio(mejorSol);
  	//busqueda_en_profundidad(deposit, graph->t_list);
  	
  	Edge d(1,1,0,0, true);
  	solParcial.push_back(d);
  	t_start = time(NULL);
  	beneficioDisponible = beneficio(mejorSol);
  	cout << beneficioDisponible << "\n";
  	mejorSol = busqueda_en_profundidad(1, graph->a_list);
  	int vh = beneficio(mejorSol);
  	cout << filename << " " << vh << " " << dif << "\n";
  	return 0;

}