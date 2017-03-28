#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <vector>
#include "max_benefit.cpp"

std::vector<vector<Edge>> solParcial;
std::vector<vector<Edge>> mejorSol;
int beneficioDisponible;

void obtener_lista_de_sucesores(int v);

bool ciclo_negativo(Edge e, std::vector<vector<Edge>> solParcial){
	// verificar si e.n2 ya está en la solucion parcial
	// en caso true, se ve que el ciclo no sea negativo
	if(true){
		if(true){
			return true;
		}
	}
	return false;
}

bool esta_lado_en_sol_parcial(Edge e, std::vector<vector<Edge>> solParcial);

bool repite_ciclo(std::vector<Edge> L, Edge e, std::vector<vector<Edge>> solParcial){
	// verificar si e.n2 ya está en la solucion parcial
	// forma u ciclo con e',en caso true, se ve que e' este en solParcial
	// y que ((be − ce) < (be' − ce')
	if(true){
		if(true){
			return true;
		}
	}
	return false;
}

bool cumple_acotamiento(Edge e, std::vector<vector<Edge>> solParcial);

int vertice_mas_externo(std::vector<vector<Edge>> v);

int beneficio(std::vector<vector<Edge>> g);

void agregar_lado(Edge e, std::vector<vector<Edge>> solParcial);

Edge eliminar_ultimo_lado(std::vector<vector<Edge>> solParcial);

void busqueda_en_profundidad():
	int v = vertice_mas_externo(solParcial);
	// vemos si se encuentra una mejor solución factible
	if (v == 1){ //1 es el depósito
		if(beneficio(solParcial) > beneficio(mejorSol)){
			std::copy(solParcial.begin(), solParcial.end(), mejorSol.begin());
		}
	} 

	std::vector<Edge> L = obtener_lista_de_sucesores(v); //lista de aristas
	for(int i =0; i < L.size(); i++){
		Edge e = L[i];
		if(!ciclo_negativo(e, solParcial) and 
			!esta_lado_en_sol_parcial(e, solParcial) and
			!repite_ciclo(L, e, solParcial) and
			cumple_acotamiento(e, solParcial)){
				agregar_lado(e, solParcial);
				ce = e.get_cost();
				be = e.get_benefit();
				beneficioDisponible = beneficioDisponible - max(0, be-ce);
				busqueda_en_profundidad();
		}
	}

	Edge e = eliminar_ultimo_lado(solParcial);
	ce = e.get_cost();
	be = e.get_benefit();
	beneficioDisponible = beneficioDisponible + max(0, be-ce);
