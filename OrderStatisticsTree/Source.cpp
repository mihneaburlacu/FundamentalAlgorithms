/*
Burlacu Mihnea-Sebastian
Grupa 9
Am avut de implementat operatiile de management a unui arbore de statistica de ordine:
    BuildTree, OS_Select si OS_Delete
Am implementat arborele de statistica ca un arbore binar de cautare perfect echilibrat care contine campul size

Pentru BuildTree introducem elementele dintr-un vector in arborele nostru binar de cautare, iar campul size o sa fie pentru fiecare nod 
    size-ul subarborelui stang + size-ul subarborelui drept + 1

Pentru OS_Select complexitatea este O(log n) pentru ca fiecare apel recursiv al lui merge cu un nivel mai jos in arbore,
    ceea ce este echivalent cu inaltimea arborelui

Pentru OS_Delete avem 3 cazuri:
    1. Stergem o frunza
	2. Stergem un nod cu un copil: va trebui sa inlocuim cu fiul lui
	3. Stergem un nod cu 2 copii: stergem nodul propriu-zis al succesorului (inordine) lui, copiem valoarea de la succesor la nodul care trebuie sters
	        deci succesorul(de la parcurgerea inordine) o sa vina in locul lui
	In toate cazurile totul se face in timp constant, in afara de apelul functiei pentru minim care se face in O(log n)
	Deci are complexitatea O(log n)
*/
#include <iostream>
#include "Profiler.h"

using namespace std;

Profiler p("Statistici");

#define MAX_SIZE 10000
#define NR_TESTE 5
#define NR_PRAG 500

int opTotalBuild; //numarul total pentru functia de construire arbore
int opTotalSelect; //numarul total pt OS_Select
int opTotalDelete; //numarul total pt OS_Delete

typedef struct Node {
	int key; //data
	int size; //pentru dimensiune
	struct Node* left; //fiu stanga
	struct Node* right; //fiu dreapta
}NodeTree;

NodeTree* createNode()
{
	NodeTree* newNode = (NodeTree*)malloc(sizeof(NodeTree));

	newNode->size = 1; // la inceput nu este decat el
	newNode->left = NULL;
	newNode->right = NULL;

	return newNode;
}

int countNodes(NodeTree* node)
{
	if (node == NULL) {
		return 0;
	}
	else {
		return 1 + countNodes(node->left) + countNodes(node->right);
	}
}

NodeTree* minim_element(NodeTree* node)
{
	NodeTree* min = node; //minimul
	
	while (min->left != NULL) { //cat timp mai avem elemente
		opTotalDelete++;
		min = min->left; //parcurgem arborele pana il gasim pe minimul de pe stanga
	}

	return min;
}

void pretty_print(NodeTree* node, int level)
{
	if (node == NULL) {
		return;
	}

	level = level + 3;
	pretty_print(node->right, level);
	cout << endl;

	for (int i = 3; i < level; i++) {
		cout << " ";
	}
	cout << node->key << endl;

	pretty_print(node->left, level);
}

void print_tree_inorder(NodeTree* node)
{
	if (node == NULL) {
		return;
	}

	print_tree_inorder(node->left);  //stanga
	cout << node->key << "  ";  //curent
	print_tree_inorder(node->right);  //dreapta
}

void print_tree_preorder(NodeTree* node)
{
	if (node == NULL) {
		return;
	}

	cout << node->key << "  ";  //curent
	print_tree_inorder(node->left);  //stanga
	print_tree_inorder(node->right);  //dreapta
}

NodeTree* BuildTree(int v[], int l, int r)
{
	NodeTree* root = new NodeTree(); //cream nodul de inceput

	if (l > r) {
		return NULL;
	}
	else {
		int m = (l + r) / 2; //obtinem radacina din mijloc
		opTotalBuild++;
		root->size = r - l + 1; //size-ul devine dimensiunea arborelui stang cu cel drept
		opTotalBuild++;
		root->key = v[m]; //primeste radacina
		opTotalBuild++;
		root->left = BuildTree(v, l, m - 1); //arbore stang
		opTotalBuild++;
		root->right = BuildTree(v, m + 1, r); //arbore drept
		return root;
	}
}

NodeTree OS_Select(NodeTree* x, int i) //i este rank-ul, iar x elem. de unde pornim
{
	opTotalSelect++;
	int r = countNodes(x->left) + 1; //rank-ul nodului x
	if (i == r) {
		return *x;  //am gasit
	}
	else if (i < r) {
		return OS_Select(x->left, i); //i-ul se afla in subraborele stang
	}
	else {
		return OS_Select(x->right, i - r); //noul i este rank-ul lui din subarborele dreapta
	}
}

NodeTree* OS_Delete(NodeTree* root, int key)
{
	if (root == NULL) {
		return NULL;
	}

	if (key < root->key) {
		opTotalDelete++;
		root->left = OS_Delete(root->left, key); //cautam in stanga
	}
	else {
		if (key > root->key) { 
			opTotalDelete++;
			root->right = OS_Delete(root->right, key); //cautam in dreapta
		}
		else { //daca l-am gasit
			if (root->left == NULL) { //daca nu avem copil stanga
				opTotalDelete++;
				NodeTree* newNode = root->right; //schimbam cu dreapta
				free(root);
				return newNode;
			}
			else if (root->right == NULL) { //daca nu avem copil dreapta
				opTotalDelete++;
				NodeTree* newNode = root->left; //schimbam cu stanga
				free(root);
				return newNode;
			}

			NodeTree* nodeM = minim_element(root->right); //luam elementul minim de pe stanga
			root->key = nodeM->key;
			root->right = OS_Delete(root->right, nodeM->key);
		}
	}

	root->size = countNodes(root);
	return root;
}

void testareAlgoritmi()
{
	NodeTree* root = new NodeTree();
	NodeTree* root_copy1 = root; //copie pentru root
	NodeTree* root_copy2 = root; //copie pentru root

	int v[11];
	int nr = 10;

	for (int i = 0; i < nr; i++) {
		v[i] = i;
	}

	root = BuildTree(v, 0, nr - 1);
	root_copy1 = BuildTree(v, 0, nr - 1); //construim si arborii copie
	root_copy2 = BuildTree(v, 0, nr - 1); // construim si arborii copie
	cout << "Arborele parcurs inordine: " << endl;
	print_tree_inorder(root);
	cout << endl;
	cout << "Arborele parcurs cu preordine: " << endl;
	print_tree_preorder(root);
	cout << endl << endl;
	cout << "Arborele initial: " << endl;
	pretty_print(root, 0);
	
	//pentru OS_Select:
	cout << endl << endl;
	int i = 10;
	int i2 = 13;
	if (i > nr) {
		cout << "Elementul cu rank-ul " << i << " nu exista" << endl;
	}
	if (i2 > nr) {
		cout << "Elementul cu rank-ul " << i2 << " nu exista" << endl;
	}
	NodeTree sel = OS_Select(root, i);
	cout << "Elementul cu rank-ul " << i << " este: " << sel.key << endl;
	cout << endl << endl;

	//pentru OS_Delete:
	NodeTree mid = OS_Select(root, nr / 2); //radacina e la mijloc si are 2 copii
	OS_Delete(root, mid.key);
	cout << "OS_Delete nod cu 2 copii: " << endl;
	pretty_print(root, 0);

	OS_Delete(root_copy1, 9); //ultimul element e frunza 
	cout << "OS_Delete frunza: " << endl;
	pretty_print(root_copy1, 0);

	OS_Delete(root_copy2, 8); //8 are un singur copil
	cout << "OS_Delete nod cu un singur copil: " << endl;
	pretty_print(root_copy2, 0);
	
}

void perf()
{
	int v[10001];
	srand(time(NULL));
	NodeTree* rootTree;

	for (int n = 100; n <= MAX_SIZE; n = n + NR_PRAG) {
		Operation op1 = p.createOperation("OS_Select", n);
		Operation op2 = p.createOperation("OS_Delete", n);
		//Operation op3 = p.createOperation("OS_Build", n);
		
		opTotalBuild = 0;
		opTotalSelect = 0;
		opTotalDelete = 0;

		for (int t = 0; t < NR_TESTE; t++) {

			for (int i = 0; i < n; i++) {
				v[i] = i;
			}

			rootTree = BuildTree(v, 0, n);

			for (int size = n; size > 0; size--) {
				int elem = rand() % size + 1;
				NodeTree del = OS_Select(rootTree, elem);
				OS_Delete(rootTree, elem);
			}
		}

		op1.count(opTotalSelect);
		op2.count(opTotalDelete);
		//op3.count(opTotalBuild);
	}

	p.divideValues("OS_Select", NR_TESTE);
	p.divideValues("OS_Delete", NR_TESTE);
	
	p.createGroup("Operatii total", "OS_Select", "OS_Delete");

	p.showReport();
}

int main()
{
	//testareAlgoritmi();
	perf();
	return 0;
}