#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <vector>
#include <unordered_set>
#include "max_benefit.cpp"

std::vector<vector<Edge> > solParcial;
std::vector<vector<Edge> > mejorSol;
int beneficioDisponible;
std::unordered_set<int> solP = {1};
// donde guardo los nodos que pertenecen a la sol?

std::vector<Edge> obtener_lista_de_sucesores(int v){
	std::vector<Edge> L;
	for(std::vector<Edge>::iterator e = solParcial[v].begin(); e != solParcial[v].end(); ++e){
		L.push_back(*e);
		Edge e1(e -> n1, e-> n2, e -> get_cost(), 0);
		L.push_back(e1);
	}

	return L;
}
int beneficio(std::vector<vector<Edge> > g){
	int v = 0;
	int b = 0;
	while(!g[v].empty() and v < g.size()){
		Edge e = g[v][0];
		g[v].erase(g[v].begin());
		b = b + (e.get_benefit() - e.get_cost());
		v = e.n1;
	}
	return b;
}

bool ciclo_negativo(Edge e, std::vector<vector<Edge> > solParcial){
	// verificar si e.n2 ya está en la solucion parcial
	// en caso true, se ve que el ciclo no sea negativo
	if(true){
		if(true){
			return true;
		}
	}
	return false;
}

bool esta_lado_en_sol_parcial(Edge e, std::vector<vector<Edge> > solParcial){

	int count = 0;
	for (std::vector<Edge>::iterator ed = solParcial[e.n1].begin(); ed != solParcial[e.n1].end(); ++ed){
		if(e.n1 == ed -> n1){
			count++;
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

bool repite_ciclo(std::vector<Edge> L, Edge e, std::vector<vector<Edge> > solParcial){
	
	return false;
}

bool cumple_acotamiento(Edge e, std::vector<vector<Edge> > solParcial){
	int beneficioE = e.get_benefit() - e.get_cost();
	int beneficioSolParcial = beneficio(solParcial) + beneficioE;
	int maxBeneficio = beneficioDisponible - max(0, beneficioE) + beneficioSolParcial;
	if(maxBeneficio <= beneficio(mejorSol)){
		return false;
	}
	else{
		return true;
	}
}

int vertice_mas_externo(std::vector<vector<Edge> > v);



void agregar_lado(Edge e, std::vector<vector<Edge> > solParcial){
	solParcial[e.n1].push_back(e);
}

Edge eliminar_ultimo_lado(std::vector<vector<Edge> > solParcial){

	std::vector<vector<Edge> > aux = solParcial;
	int v = 0;
	int be;
	int ce;
	Edge e = aux[0][0];
	while (!aux[v].empty() && v < aux.size()) {
        e = aux[v][0]; // First element
        aux[v].erase(aux[v].begin());
        be = e.get_benefit();
        ce = e.get_cost();
        //vToDelete = vOut;
        v = e.n1;
    }
    Edge e1(e.n1, e.n2, ce, be);
    solParcial[v].erase(solParcial[v].end());

    return e1;
}

void busqueda_en_profundidad(int v){
	// vemos si se encuentra una mejor solución factible
	if (v == 1){ //1 es el depósito
		if(beneficio(solParcial) > beneficio(mejorSol)){
			std::copy(solParcial.begin(), solParcial.end(), mejorSol.begin());
		}
	} 

	std::vector<Edge> L = obtener_lista_de_sucesores(v); //lista de aristas
	int ce;
	int be;
	for(int i =0; i < L.size(); i++){
		Edge e = L[i];
		if(!ciclo_negativo(e, solParcial) &&
			!esta_lado_en_sol_parcial(e, solParcial) &&
			!repite_ciclo(L, e, solParcial) &&
			cumple_acotamiento(e, solParcial)){
				agregar_lado(e, solParcial);
				ce = e.get_cost();
				be = e.get_benefit();
				beneficioDisponible = beneficioDisponible - max(0, be-ce);
				busqueda_en_profundidad(e.n1);
		}
	}

	Edge e = eliminar_ultimo_lado(solParcial);
	ce = e.get_cost();
	be = e.get_benefit();
	beneficioDisponible = beneficioDisponible + max(0, be-ce);
}