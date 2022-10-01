/*
Burlacu Mihnea-Sebastian
Grupa 30229
Am avut de implementat Algoritmul lui Kruskal folosind multimi disjuncte
Mai intai am implementat functiile Make-Set(creearea unei multimi noi), Union(reuniunea dintre cele 2 multimi) si Find-Set(Cauta multimea in care se afla nodul)
Apoi Algoritmul lui Kruskal (Cautarea drumului de cost minim), care se bazeaza pe tehnica Greedy.
   Pentru sortarea muchiilor am folosit Quicksort (complexitate O(n log n)
   Algoritmul lui Kruskal are complexitatea: O(E log E): for-ul de la Make-Set se real. in O(V)
        sortarea in O(ElogE), iar while-ul pentru fiecare muchie in aprox. O(E), unde alfa este factorul atenuant
*/


#include <iostream>
#include "Profiler.h"

using namespace std;

Profiler p("Profiler");

#define MAX_SIZE 800
#define STEP_SIZE 100
#define FIRST_VALUE 0
#define NR_TESTE 5

int n;
int opTotal;

typedef struct Node { //struct pentru nod
	int rank;  //inaltimea maxima a subarborelui
	int p;  //valoarea
}Node;

typedef struct Edge { //structura pentru muchie
	int x; 
	int y; 
	int cost;  //cost-ul muchiei de la x la y
}Edge;

typedef struct Graph{
	int E;  //numar muchii
	int V;  //numar noduri
	struct Edge* edge;  //muchii
};

int compareCost(const void *a, const void* b)  //pentru qsort
{
	opTotal++;
	Edge* m1 = (Edge*)a;
	Edge* m2 = (Edge*)b;

	return m1->cost > m2->cost;
}

void makeSet(Node x[], int val)
{
	opTotal += 2;
	x[val].rank = 0;  //inaltimea la inceput este zero
	x[val].p = val;
}

int findSet(Node x[], int searchNode)
{
	opTotal++;
	if (x[searchNode].p != searchNode) { //daca nu l-am gasit
		x[searchNode].p = findSet(x, x[searchNode].p);
		opTotal++;
	}

	return x[searchNode].p;
}

void Union(Node x[], int first, int second)
{
	int val1 = findSet(x, first); 
	int val2 = findSet(x, second);

	opTotal++;
	if (x[val1].rank < x[val2].rank) {  //legam primul subarbore la al doilea
		x[val1].p = val2;
		opTotal++;
	}
	else {
		opTotal++;
		if (x[val1].rank > x[val2].rank) {  //legam al doilea subarbore la primul
			x[val2].p = val1;
			opTotal++;
		}
		else {  //daca sunt egale se incrementeaza inaltimea
			x[val1].p = val2;
			x[val2].rank++;
			opTotal++;
		}
	}
}

Graph* build_graph(int V, int E)
{
	Graph* g = (Graph*)malloc(sizeof(Graph));
	
	g->E = E; //initializam nr. muchii
	g->V = V; //initializam nr. noduri

	g->edge = (Edge*)malloc((E + 1) * sizeof(Edge)); //alocam fiecare muchie

	return g;
}

void kruskalMST(Graph* g)
{
	Edge m[40001]; //muchii noi

	int V = g->V; //numarul de noduri

	int e = 0; 
	int i = 0; //iterator pentru muchii

	qsort(g->edge, g->E, sizeof(g->edge[0]), compareCost); //sortam muchiile dupa cost
	Node* nod = (Node*)malloc(V * sizeof(struct Node)); //alocam nod nou

	for (int j = 0; j < V; j++) {
		makeSet(nod, j); //formam multimile
	}

	while (e < V - 1) {
		opTotal++;
		Edge nextEdge = g->edge[i++]; //trecem la urm. muchie

		int xNode = findSet(nod, nextEdge.x); //gasim nodurile ei
		int yNode = findSet(nod, nextEdge.y);

		opTotal++;
		if (xNode != yNode) {
			opTotal++;
			m[e++] = nextEdge;
			Union(nod, xNode, yNode);
		}
	}

	cout << "Muchiile arborelui minim de parcurgere in urma algoritmului lui Kruskal: " << endl;
	for (i = 0; i < e; i++) {
		cout << m[i].x << "  " << m[i].y << " cu costul: " << m[i].cost <<endl;
	}
	cout << endl;

	return;
}

void kruskalMSTPerf(Graph* g)
{
	Edge m[10001]; //muchii noi

	int V = g->V; //numarul de noduri

	int e = 0;
	int i = 0; //iterator pentru muchii

	qsort(g->edge, g->E, sizeof(g->edge[0]), compareCost); //sortam muchiile dupa cost
	Node* nod = (Node*)malloc(V * sizeof(struct Node)); //alocam nod nou

	for (int j = 0; j < V; j++) {
		makeSet(nod, j); //formam multimile
	}

	while (e < V - 1) {
		opTotal++;
		Edge nextEdge = g->edge[i++]; //trecem la urm. muchie

		int xNode = findSet(nod, nextEdge.x); //gasim nodurile ei
		int yNode = findSet(nod, nextEdge.y);

		opTotal++;
		if (xNode != yNode) {
			opTotal++;
			m[e++] = nextEdge;
			Union(nod, xNode, yNode);
		}
	}

	return;

}

void testareAlgoritmiMultimi()
{
	Node* v = (Node*)malloc(12 * sizeof(Node));

	for (int i = 1; i <= 10; i++) {
		makeSet(v, i);  //noduri de la 1 la 10
	}

	Union(v, 1, 4);
	Union(v, 3, 6);
	Union(v, 8, 2);
	Union(v, 5, 2);
	Union(v, 10, 7);

	cout << "Reprezentantul pentru nodul 1: " << findSet(v, 1) << endl;
	cout << "Reprezentantul pentru nodul 3: " << findSet(v, 3) << endl;
	cout << "Reprezentantul pentru nodul 5: " << findSet(v, 5) << endl;
	cout << "Reprezentantul pentru nodul 8: " << findSet(v, 8) << endl;
	cout << "Reprezentantul pentru nodul 10: " << findSet(v, 10) << endl;
	cout << endl;
}

void testareAlgoritmKruskal()
{
	int V = 8; //8 noduri
	int E = 10; //10 muchii

	Graph* G = build_graph(V, E);

	G->edge[0].x = 0;
	G->edge[0].y = 2;
	G->edge[0].cost = 3;

	G->edge[1].x = 1;
	G->edge[1].y = 2;
	G->edge[1].cost = 4;

	G->edge[2].x = 1;
	G->edge[2].y = 3;
	G->edge[2].cost = 2;

	G->edge[3].x = 3;
	G->edge[3].y = 4;
	G->edge[3].cost = 3;

	G->edge[4].x = 2;
	G->edge[4].y = 5;
	G->edge[4].cost = 8;

	G->edge[5].x = 3;
	G->edge[5].y = 5;
	G->edge[5].cost = 4;

	G->edge[6].x = 5;
	G->edge[6].y = 6;
	G->edge[6].cost = 5;

	G->edge[7].x = 6;
	G->edge[7].y = 7;
	G->edge[7].cost = 9;

	G->edge[8].x = 5;
	G->edge[8].y = 7;
	G->edge[8].cost = 6;

	G->edge[9].x = 0;
	G->edge[9].y = 1;
	G->edge[9].cost = 2;

	for (int i = 0; i < E; i++) {
		cout << "Muchia intre nodurile " << G->edge[i].x << "-" << G->edge[i].y << " are costul: " << G->edge[i].cost << endl;
	}
	cout << endl;

	kruskalMST(G);
}

int** generate_random_connected_graph(int dim)
{
	int ok = 0;
	int nrMuchii;
	int connected_nodes[10001];

	memset(connected_nodes, 0, dim * sizeof(int*));

	int** mat = (int**)calloc(dim, sizeof(int*));

	if (mat == NULL) { //nu s-a putut aloca
		ok = 1;
		goto labelEnd;
	}

	for (int i = 0; i < dim; i++) {
		mat[i] = (int*)calloc(dim, sizeof(int));
		if (!mat[i]) { //nu s-au putut aloca elem.
			ok = 1;
			goto labelEnd;
		}
	}

	while (1) {
		int a = rand() % dim; //generam nr. aleatorii
		int b = rand() % dim;

		if (a != b) {
			mat[a][b] = rand(); //introducem in matricea nodurilor elem. aleator
			mat[b][a] = mat[a][b]; 

			connected_nodes[a] = 1;
			connected_nodes[b] = 1;

			break;
		}
	}

	nrMuchii = 1;

	while (nrMuchii < dim - 1) {
		int a = rand() % dim; //generam nr. aleator
		if (connected_nodes[a]) {
			continue;
		}

		int b;
		
		do {
			b = rand() % dim; //generam nr aleator
		} while (!connected_nodes[b]);

		mat[a][b] = rand();  //punem in matricea nodurilor elem. aleator
		mat[b][a] = mat[a][b];

		connected_nodes[a] = 1;
		nrMuchii = nrMuchii + 1;
	}

	while (nrMuchii < 4 * dim) {
		int a = rand() % dim;
		int b = rand() % dim;

		if (a == b || mat[a][b]) {
			continue;
		}

		mat[a][b] = rand();
		mat[b][a] = mat[a][b];

		nrMuchii = nrMuchii + 1;
	}

labelEnd:
	if (ok == 1) {
		if (mat) {
			for (int i = 0; i < dim; i++) {
				if (mat[i] != NULL) {  //eliberam memoria
					free(mat[i]); 
				}
				free(mat);
			}
		}
		return 0;
	}

	return mat;
}

void perf() {
	Graph* G;
	int j;

	for (n = FIRST_VALUE; n <= MAX_SIZE; n = n + STEP_SIZE) {
		j = 0;
		opTotal = 0;

		Operation opKruskal = p.createOperation("Total Kruskal", n);

		int m = n * 4;
		G = build_graph(n, m);

		while (j < m) {
			int first = rand() % n;
			int second = rand() % n;
			G->edge[j].x = first;
			G->edge[j].y = second;
			G->edge[j].cost = rand() % 100;
			j++;
		}

		kruskalMSTPerf(G);

		opKruskal.count(opTotal);
	}

	p.showReport();
}


/*
void perf()
{
	Graph* g;
	int a, b, j;
	for (int t = 0; t < NR_TESTE; t++) {
		for (n = STEP_SIZE; n <= 800; n += STEP_SIZE) {
			int j = 0;

			compFindSet = 0; compMakeSet = 0; compUnion = 0; compK = 0;
			atrFindSet = 0; atrMakeSet = 0; atrUnion = 0; atrK = 0;

			Operation atrUnionTotal = p.createOperation("Atribuiri Union", n);
			Operation atrFindSetTotal = p.createOperation("Atribuiri FindSet", n);
			Operation atrMakeSetTotal = p.createOperation("Atribuiri MakeSet", n);
			Operation atrKTotal = p.createOperation("Atribuiri Kruskal", n);

			Operation compUnionTotal = p.createOperation("Comparatii Union", n);
			Operation compFindSetTotal = p.createOperation("Comparatii FindSet", n);
			Operation compMakeSetTotal = p.createOperation("Comparatii MakeSet", n);
			Operation compKTotal = p.createOperation("Comparatii Kruskal", n);

			g = build_graph(n, 4 * n);

			while (j < 4 * n)
			{

				a = rand() % n;
				b = rand() % n;
				g->edge[j].x = a;
				g->edge[j].y = b;
				g->edge[j].cost = rand() % 100;
				j++;
			}

			kruskalMSTPerf(g);

			atrFindSetTotal.count(atrFindSet);
			atrMakeSetTotal.count(atrMakeSet);
			atrUnionTotal.count(atrUnion);
			atrKTotal.count(atrK);

			compFindSetTotal.count(compFindSet);
			compMakeSetTotal.count(compMakeSet);
			compUnionTotal.count(compUnion);
			compKTotal.count(compK);

		}

	}

	p.divideValues("Atribuiri FindSet", NR_TESTE);
	p.divideValues("Atribuiri MakeSet", NR_TESTE);
	p.divideValues("Atribuiri Union", NR_TESTE);
	p.divideValues("Atribuiri Kruskal", NR_TESTE);

	p.divideValues("Comparatii FindSet", NR_TESTE);
	p.divideValues("Comparatii MakeSet", NR_TESTE);
	p.divideValues("Comparatii Union", NR_TESTE);
	p.divideValues("Comparatii Kruskal", NR_TESTE);

	p.showReport();

}
*/

int main()
{
	//testareAlgoritmiMultimi();
	//testareAlgoritmKruskal();
	perf();

	return 0;
}