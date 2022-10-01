/*
Burlacu Mihnea-Sebastian
Grupa 9
Am implementat 2 transformari a unui arbore, pornind de la un vector de parinti
  Prima din vectorul de parinte in arbore multicai
  A doua din arbore multicai in B Tree
A trebuit sa afisam arborele in toate cele 3 reprezentari, deci am implementat 3 prettyprint-uri pornind de la fiecare structura
Toate reprezentarile se fac in O(n), deoarece am parcurs o singura data fiecare nod al arborelui
Pentru prima transformare am ajuns la un arbore multicai, care difera de arborele binar
    prin faptul ca fiecare nod poate sa aiba mai mult de 2 copii, pentru aceasta structura am avut nevoie de fii(copiii) si cheia(valoarea) fiecarui nod
	deci am luat fiecare nod din vectorul parinti si l-am adaugat la copiii din arbore, crescand de fiecare data numarul de copii
Pentru a doua transformare am facut o reprezentare binara, in care am avut nevoie de primul copil(cel mai din stanga) si cel mai apropiat frate(primul din dreapta) al fiecarui nod
   deci pentru fiecare nod din arbore i-am luat primul copil si primul frate
*/
#include <iostream>

using namespace std;

#define MAX_SIZE 20

int v[] = { 2, 7, 5, 2, 7, 7, -1, 6, 5 };  // vectorul de parinti

typedef struct Node2 {
	int key;
	int nrCopii; //numarul copiilor fiecarui nod
	struct Node2* vCopii[20]; //vectorul copiilor fiecarui nod
}NodeMultiWay;

typedef struct Node3 {
	int key;
	struct Node3* primulCopil; //cel mai din stanga copil al nodului
	struct Node3* urmatorulFrate; //fratele din dreapta nodului
}NodeBinary;

//afisare din vector parinti 
void prettyPrint_1(int parents[], int n, int val, int level)
{
	for (int i = 0; i < n; i++) {
		if (parents[i] == val) { // o sa testam cu -1 pentru radacina
			for (int l = 0; l < level; l++) {
				cout << "   ";
			}

			cout << i + 1 << " " << endl; //afiseaza valoarea
			prettyPrint_1(parents, n, i + 1, level + 1); //o sa mearga la valoarea respectiva de la parinti
		}
	}
}

//transformare 1: din vector parinti in arbore multicai
NodeMultiWay* transformare1(int parinti[], int n)
{
	NodeMultiWay* vMultiWay[MAX_SIZE];  //vectorul pentru arbore
	NodeMultiWay* root = 0;
	NodeMultiWay* parinte; //luam 2 noduri noi(parinte, copil) cu care sa lucram la construire
	NodeMultiWay* copil;

	//alocam vectorul
	for (int i = 1; i <= n; i++) {
		vMultiWay[i] = (NodeMultiWay*)malloc(sizeof(NodeMultiWay));
		vMultiWay[i]->key = i; 
		vMultiWay[i]->nrCopii = 0; //initial fiecare nod nu are copii
	}

	for (int i = 1; i <= n; i++) { //pentru parcurgerea initiala a vectorului de parinti
		if (parinti[i - 1] == -1) {
			root = vMultiWay[i]; //am gasit radacina arborelui
		}
		else {  //daca nu e radacina
			copil = vMultiWay[i];
			parinte = vMultiWay[parinti[i - 1]];

			if (parinte->nrCopii >= MAX_SIZE) {
				return 0;
			}

			else {  //daca mai avem loc sa adaugam noduri
				parinte->vCopii[parinte->nrCopii] = copil;  //adaugam copilul la vectorul de fii al parintelui
				parinte->nrCopii++;
			}
		}
	}

	return root;  //returnam radacina arborelui

}

void prettyPrint_2(NodeMultiWay* root, int level)
{
	for (int i = 0; i < level; i++) {
		cout << "   ";
	}

	cout << root->key << endl;

	for (int i = 0; i < root->nrCopii; i++) {
		prettyPrint_2(root->vCopii[i], level + 1);
	}
}

//transformare 2: din arbore multicai in reprezentare binara
NodeBinary* transformare2(NodeMultiWay* nodeMultiWay)
{
	NodeBinary* rootBinary = 0;
	NodeBinary* newNode;

	if (nodeMultiWay != 0) {
		rootBinary = (NodeBinary*)malloc(sizeof(NodeBinary));
		rootBinary->key = nodeMultiWay->key;
		rootBinary->urmatorulFrate = NULL;
		rootBinary->primulCopil = NULL;

		if (nodeMultiWay->nrCopii > 0) { //daca mai avem copii de adaugat
			rootBinary->primulCopil = transformare2(nodeMultiWay->vCopii[0]);
			newNode = rootBinary->primulCopil;  

			for (int i = 1; i < nodeMultiWay->nrCopii; i++) {  //parcurgem copiii
				newNode->urmatorulFrate = transformare2(nodeMultiWay->vCopii[i]);
				newNode = newNode->urmatorulFrate; //trecem la urmatorul
			}
		}
	}

	return rootBinary;

}

void prettyPrint_3(NodeBinary* root, int level) {
	if (root != 0) {
		for (int i = 1; i <= level; i++) {
			cout << "   ";
		}

		cout << root->key << endl;
		
		prettyPrint_3(root->primulCopil, level + 1);
		prettyPrint_3(root->urmatorulFrate, level);
	}
}

void testare()
{
	int n = sizeof(v) / sizeof(int);
	cout << "Prima reprezentare (vector de parinti):  " << endl;
	prettyPrint_1(v, n, -1, 0);
	cout << endl << endl;

	NodeMultiWay* root = transformare1(v, n);  //din vector parinti in arbore multicai
	cout << "A doua reprezentare (arbore multicai): " << endl;
	prettyPrint_2(root, 0);
	cout << endl << endl;

	NodeBinary* rootBinary = transformare2(root); //din multicai in binara
	cout << "A treia reprezentare (binara): " << endl;
	prettyPrint_3(rootBinary, 0);
	cout << endl << endl;

}

int main()
{
	testare();
	return 0;
}