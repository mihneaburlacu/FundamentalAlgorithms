/*
Burlacu Mihnea-Sebastian
Grupa 9

Am implementat algoritmul DFS pentru un graf si sortarea topologica utilizand metoda de parcurgere
Complexitatea algoritmului DFS este O(|V| + |E|), unde V = nr. de noduri, iar E = nr. de muchii
Sortarea topologica functioneaza in functie de timpii de finalizare de la DFS
*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"

using namespace std;

#define MAX_SIZE 4500
#define STEP_SIZE 100
#define NR_TESTS 5

Profiler prof("DFS");

int op; //numarul de operatii pentru profiler

enum {
	ALB, GRI, NEGRU
};

typedef struct nod {
	int val; //cheie
	struct nod* next;
	struct nod* parent;
	struct nod** listaAdj;
	int d; //timpul initial de descoperire
	int f; //timpul final 
	int color; //culoare
	int size; //dimensiune
}NodeT;

typedef struct G {
	NodeT** listaAdiacenta; //lista pentru vecinii fiecarui nod
	int nrNoduri; //V
	int nrMuchii; //E
}Graf;

NodeT* createNode(int newVal) {
	NodeT* newNode = (NodeT*)malloc(sizeof(NodeT)); //alocam noul nod

	newNode->listaAdj = (NodeT**)calloc(50, sizeof(NodeT*)); //alocam lista de adiacenta nod

	newNode->val = newVal; //adaugam cheia
	newNode->parent = NULL; //initial nu are parinte
	newNode->size = 0;
	newNode->d = 0;
	newNode->f = 0;
	newNode->color = ALB; //nu e parcurs initial
	newNode->next = NULL;

	return newNode;
}

Graf* makeGraph(int nrNodes) {
	Graf* g = (Graf*)calloc(1, sizeof(Graf)); //alocam graful

	g->nrNoduri = nrNodes; //initializam nr. de noduri din graf

	g->listaAdiacenta = (NodeT**)calloc(nrNodes, sizeof(NodeT*)); //alocam lista de adiacenta

	for (int i = 0; i < nrNodes; i++) {
		g->listaAdiacenta[i] = createNode(i); //adaugam nodul
	}

	return g;
}

void afisareGraf(Graf* g) {
	int i;

	for (int i = 0; i < g->nrNoduri; i++) {
		NodeT* p = g->listaAdiacenta[i];

		cout << endl;
		cout << "Nodul " << i << endl;

		while (p) {
			cout << p->val << "->";
			p = p->next;
		}
		cout << endl;

	}
}

void addMuchie(Graf *graf, int a, int b) {
	/*
	NodeT* x = graph.listaAdiacenta[a];
	NodeT* y = createNode(b);

	if (x->next == NULL) { //daca nodul nu are vecini facem muchie cu y
		x->next = y;  
	}
	else { //daca are vecini
		while (x->next != NULL && x->val != b) { //cat timp mai avem vecini si sunt diferiti de valoarea b
			x = x->next; //trecem la urmatorul vecin
		}

		if (x->val != b) {
			x->next = y; //adaugam nodul 
		}
	}
	*/

	graf->listaAdiacenta[a]->listaAdj[graf->listaAdiacenta[a]->size++] = graf->listaAdiacenta[b];
	
}

//timpp pentru timpii finali si initiali la DFS

void dfs_visit_recursiv(Graf* graph, NodeT* node, int &timpp)
{
	timpp++;
	node->color = GRI; //il marcam gri
	node->d = timpp; //updatam timpul nodului de descoperire
	op = op + 2;

	//cout << "timpul initial de descoperire: " << node->d << endl;

	NodeT* secondNode = graph->listaAdiacenta[node->val]->next; //trecem la vecini
	//cout << secondNode->val << endl;

	for (int i = 0; i < node->size; i++) {
		op++;
		if (node->listaAdj[i]->color == ALB) {
			op++;
			node->listaAdj[i]->parent = node;
			//cout << node->parent->val << " - " << node->val << " " << endl;
			dfs_visit_recursiv(graph, node->listaAdj[i], timpp);
		}
	}

	op = op + 2;
	timpp++;
	node->color = NEGRU; //marcam ca finalizat
	node->f = timpp; //timpul final

	cout << node->val << ": " << node->d << " (d), " << node->f << " (f) " << endl;
	
}

void dfs_visit_recursiv_perf(Graf* graph, NodeT* node, int& timpp)
{
	timpp++;
	node->color = GRI; //il marcam gri
	node->d = timpp; //updatam timpul nodului de descoperire
	op = op + 2;

	//cout << "timpul initial de descoperire: " << node->d << endl;

	NodeT* secondNode = graph->listaAdiacenta[node->val]->next; //trecem la vecini
	//cout << secondNode->val << endl;

	for (int i = 0; i < node->size; i++) {
		op++;
		if (node->listaAdj[i]->color == ALB) {
			op++;
			node->listaAdj[i]->parent = node;
			//cout << node->parent->val << " - " << node->val << " " << endl;
			dfs_visit_recursiv(graph, node->listaAdj[i], timpp);
		}
	}

	op = op + 2;
	timpp++;
	node->color = NEGRU; //marcam ca finalizat
	node->f = timpp; //timpul final

	//cout << node->val << ": " << node->d << " (d), " << node->f << " (f) " << endl;

}

void dfs_algoritm(Graf* graph) {
	NodeT* node = NULL;

	int timp = 0;

	for (int i = 0; i < graph->nrNoduri; i++) {
		node = graph->listaAdiacenta[i];
		op++;

		if (node->color == ALB) { //daca nu e parcurs
			//cout << "nodul " << node->val << endl;
			dfs_visit_recursiv(graph, node, timp);

		}
	}
}

void dfs_algoritm_perf(Graf* graph) {
	NodeT* node = NULL;

	int timp = 0;

	for (int i = 0; i < graph->nrNoduri; i++) {
		node = graph->listaAdiacenta[i];
		op++;

		if (node->color == ALB) { //daca nu e parcurs
			//cout << "nodul " << node->val << endl;
			dfs_visit_recursiv_perf(graph, node, timp);

		}
	}
}

void topologicalSort(Graf* graf) {
	dfs_algoritm(graf); //apelam dfs

	for (int i = 0; i < graf->nrNoduri - 1; i++) {
		for (int j = i + 1; j < graf->nrNoduri; j++) { //mergem la urmatoarele noduri
			if (graf->listaAdiacenta[i]->f > graf->listaAdiacenta[j]->f) { //facem interschimbare
				NodeT* node = graf->listaAdiacenta[i];
				graf->listaAdiacenta[i] = graf->listaAdiacenta[j];
				graf->listaAdiacenta[j] = node;
			}
		}
	}

	cout << "Sortare topologica: " << endl;
	for (int t = 0; t < graf->nrNoduri; t++) {
		cout << graf->listaAdiacenta[t]->val << "  " << graf->listaAdiacenta[t]->d << " (d)  " << graf->listaAdiacenta[t]->f << " (f) " << endl;
		//cout << graf->listaAdiacenta[t] << endl;
	}
}

void testareAlgoritmi() {
	Graf* graf = makeGraph(6); //cream graful cu 6 noduri

	//cream muchii
	addMuchie(graf, 0, 1);
	addMuchie(graf, 0, 3);
	addMuchie(graf, 1, 4);
	addMuchie(graf, 2, 4);
	addMuchie(graf, 2, 5);
	addMuchie(graf, 3, 4);
	addMuchie(graf, 5, 1);
	addMuchie(graf, 5, 3);

	cout << "dfs: " << endl;
	dfs_algoritm(graf);

	cout << endl;
	cout << "liste de adiacenta: " << endl;
	for (int i = 0; i < graf->nrNoduri; i++) {
		cout << graf->listaAdiacenta[i]->val << "->";

		//afisam vecinii de muchii
		for (int t = 0; t < graf->listaAdiacenta[i]->size; t++) {
			cout << graf->listaAdiacenta[i]->listaAdj[t]->val << "->";
		}

		cout << endl;
	}
	cout << endl;
	//cout << endl;

	topologicalSort(graf);
	cout << endl;
}

void perf() {
	Graf* graf = (Graf*)malloc(sizeof(Graf));

	graf->nrNoduri = 100;

	graf->listaAdiacenta = (NodeT**)malloc(graf->nrNoduri * sizeof(NodeT*));

	for (int i = 1000; i <= MAX_SIZE; i = i + STEP_SIZE) {

		Operation opTotal = prof.createOperation("DFS Op Muchii", i);

		op = 0;

		for (int j = 0; j < graf->nrNoduri; j++) {
			graf->listaAdiacenta[j] = createNode(j);
		}

		for (int v = 0; v < i; v++) {
			int a = rand() % 100; //primul nod random
			int b = rand() % 100; //al doilea nod

			if (a != b) {
				addMuchie(graf, a, b); //cream muchie intre ele
			}
		}

		int timp = 0;

		dfs_algoritm_perf(graf);

		opTotal.count(op);

		for (int m = 0; m < graf->nrNoduri; m++) {
			free(graf->listaAdiacenta[m]); //eliberam memoria
		}
	}

	for (int i = STEP_SIZE; i <= 200; i = i + 10) {
		graf->nrNoduri = i;

		graf->listaAdiacenta = (NodeT**)malloc(i * sizeof(NodeT*));

		Operation opTotal1 = prof.createOperation("DFS Op Varfuri", i);

		op = 0;

		for (int j = 0; j < graf->nrNoduri; j++) {
			graf->listaAdiacenta[j] = createNode(j);
		}

		for (int l = 0; l < MAX_SIZE; l++) {
			int a = rand() % i;
			int b = rand() % i;

			if (a != b) {
				addMuchie(graf, a, b); //cream muchia intre cele 2 noduri generate
			}
		}

		int timp = 0;

		dfs_algoritm_perf(graf);

		opTotal1.count(op);

		for (int m = 0; m < graf->nrNoduri; m++) {
			free(graf->listaAdiacenta[m]); //eliberam memoria
		}
	}

	prof.showReport();
}

int main()
{
	testareAlgoritmi();

	//perf();

	/*
	Graf* graf = makeGraph(6); //cream graful cu 6 noduri

	//cream muchii
	addMuchie(graf, 0, 1);
	addMuchie(graf, 0, 3);
	addMuchie(graf, 1, 4);
	addMuchie(graf, 2, 4);
	addMuchie(graf, 2, 5);
	addMuchie(graf, 3, 4);
	addMuchie(graf, 5, 1);
	addMuchie(graf, 5, 3);

	dfs_algoritm(graf);

	cout << endl;
	cout << "liste de adiacenta: " << endl;
	for (int i = 0; i < graf->nrNoduri; i++) {
		cout << graf->listaAdiacenta[i]->val << "->";

		//afisam vecinii de muchii
		for (int t = 0; t < graf->listaAdiacenta[i]->size; t++) {
			cout << graf->listaAdiacenta[i]->listaAdj[t]->val << "->";
		}
		cout << endl;
	}
	cout << endl;
	*/
	return 0;
}