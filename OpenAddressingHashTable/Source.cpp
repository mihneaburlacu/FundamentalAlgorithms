/*
Burlacu Mihnea-Sebastian
Grupa 9

Am implementat operatiile de cautare si inserare intr-o tabela de dispersie (HashTable) care foloseste adresarea deschisa cu verificare patratica
Iar apoi am generat tabele pentru efortul operatiilor de cautare si inserare in diferite cazuri ale factorului de umplere(0.80, 0.85, 0.90, 0.95, 0.99)
Adresarea deschisa se refera la faptul ca toate elementele ocupa tabela propriu-zisa, fara alte constructii auxiliare
Memoria eliberata prin folosirea adresarii deschisa in locul inlantuiri (care ar folosi mai multi pointeri), poate oferi posibilitatea ca HashTable-ul sa contina mai multe slot-uri cu elemente
Un alt caz in care este mai bine sa folosim adresarea deschisa este atunci cand stim numarul de key care urmeaza sa fie inserate in tabela

Factorul de umplere (alfa) nu poate depasi 1, iar el este egal cu numarul de elemente propriu-zise in tabela / capacitatea maxima a tabelei (alfa=n/m),
   unde intotdeauna nr. elementelor este mai mic sau egal cu capacitatea maxima

Pentru o cautare nereusita, nr. asteptat de probe este 1/(1-alfa)
Iar pentru una reusita, cel mult asteptam 1/alfa * ln(1 / (1-alfa))
*/

#include <iostream>
#include "Profiler.h"

#define MAX_SIZE 10007;

using namespace std;

int efort; // pentru calculul efortului la operatiile de cautare

typedef struct Slot {
	int key;  //pentru valoare
	int status;  //ocupat sau liber
}SlotHashTable;

enum{FREE, TAKEN}; //0 - free, 1 - taken: pentru statusul slot-ului

void hashTable_new(SlotHashTable* H, int m)
{
	//initializam tabela de dispersie
	for (int i = 0; i < m; i++) {
		H[i].key = NULL; 
		H[i].status = FREE;
	}
}

void hashTable_free(SlotHashTable* H, int m)
{
	//setam fiecare slot din tabela pe liber
	for (int i = 0; i < m; i++) {
		H[i].status = FREE;
	}
}

void hashTable_insert(SlotHashTable* H, int k, int m, int (*hash_function)(int k, int m, int i)) 
{
	for (int i = 0; i < m; i++) {
		int j = hash_function(k, m, i); //cautam poz. libera pt inserare

		if (H[j].status == FREE) {  //daca am gasit pozitie libera
			H[j].key = k;
			H[j].status = TAKEN;
			cout << "Am inserat elementul " << k << " pe pozitia " << j << endl;
			return;  //am reusit  sa inseram
		}
	}

	cout << "Tabela de dispersie este plina. Nu am putut insera elementul " << k;

}

void hashTable_insert1(SlotHashTable* H, int k, int m, int (*hash_function)(int k, int m, int i))  //fara afisari, pentru functia de tabele
{
	for (int i = 0; i < m; i++) {
		int j = hash_function(k, m, i); //cautam poz. libera pt inserare

		if (H[j].status == FREE) {  //daca am gasit pozitie libera
			H[j].key = k;
			H[j].status = TAKEN;
			//cout << "Am inserat elementul " << k << " pe pozitia " << j << endl;
			return;  //am reusit  sa inseram
		}
	}

	//cout << "Tabela de dispersie este plina. Nu am putut insera elementul " << k;

}

int hashTable_search(SlotHashTable* H, int k, int m, int (*hash_function)(int k, int m, int i)) 
{
	for (int i = 0; i < m; i++) {
		int j = hash_function(k, m, i); //pozitia unde cautam elem.

		efort = efort + 1;
		if (H[j].status == FREE) {
			cout << "Nu am gasit elementul " << k << endl;
			return -1; //daca slot-ul este liber, inseamna ca elementul nu este in tabela
		}
		else {
			if (H[j].key == k) {
				cout << "Am gasit elementul " << k << " pe pozitia "<< j << endl; //am gasit elem. pe poz. j
				return j;
			}
		}
	}

	cout << "Nu am gasit elementul " << k << endl;  //am ajuns la capatul tabelei si nu am gasit elem.
	return -1;

}

int hashTable_search1(SlotHashTable* H, int k, int m, int (*hash_function)(int k, int m, int i)) //fara afisari, pentru functia de tabele
{
	for (int i = 0; i < m; i++) {
		int j = hash_function(k, m, i); //pozitia unde cautam elem.

		efort = efort + 1;
		if (H[j].status == FREE) {
			//cout << "Nu am gasit elementul " << k << endl;
			return -1; //daca slot-ul este liber, inseamna ca elementul nu este in tabela
		}
		else {
			if (H[j].key == k) {
				//cout << "Am gasit elementul " << k << " pe pozitia " << j << endl; //am gasit elem. pe poz. j
				return j;
			}
		}
	}

	//cout << "Nu am gasit elementul " << k << endl;  //am ajuns la capatul tabelei si nu am gasit elem.
	return -1;

}

int h_mod(int k, int m)
{
	int position = k % m;

	if (position < 0) {
		position = position + m;
	}

	return position;
}

int linear_probing(int k, int m, int i)
{
	int position = (h_mod(k, m) + i) % m;  

	return position;
}

int quadratic_probing(int k, int m, int i)  //verificare patratica
{
	int position = (h_mod(k, m) + i + i * i) % m;  //h(k, i) = (k%m + c1 i + c2 i^2) mod m

	return position;
}

void afisareHashTable(SlotHashTable* H, int m)
{
	cout << "Tabela de dispersie este: " << endl;
	for (int i = 0; i < m; i++) {
		if (H[i].status == TAKEN) {
			cout << "H[" << i << "] = " << H[i].key << endl;
		}
		else {
			cout << "H[" << i << "] FREE" << endl;
		}
	}
}

void afisareFactorUmplere(SlotHashTable* H, int m)
{
	int nr = 0;
	double alfa; //fact. de umplere

	for (int i = 0; i < m; i++) {
		if (H[i].status == TAKEN) {
			nr++;
		}
	}

	alfa = nr / m;

	cout << "Factorul de umplere este: " << alfa;

}

int hashTable_usedSize(SlotHashTable* H, int m)
{
	int n = 0;

	for (int i = 0; i < m; i++) {
		if (H[i].status == TAKEN) {
			n++;
		}
	}

	return n;
}

void testareAlgoritmi()
{
	int m = 8; //dimensiunea max a tabelei
	SlotHashTable* H = (SlotHashTable*)malloc(m * sizeof(SlotHashTable)); //alocam tabela de dispersie

	hashTable_new(H, m); //initializam tabela

	int valIntrodus[] = { 15, 23, 78, 5, 10, 52, 81, 62, 32 }; //valorile pe care vrem sa le introducem in tabela
	int dim = sizeof(valIntrodus) / sizeof(int);

	for(int i = 0; i < dim; i++) {
		hashTable_insert(H, valIntrodus[i], m, quadratic_probing);
	}

	cout << endl << endl;
	afisareHashTable(H, m);
	cout << endl;
	afisareFactorUmplere(H, m);
	cout << endl << endl;

	//cautare in tabela de dispersie
	int search1 = hashTable_search(H, 78, m, quadratic_probing);
	int search2 = hashTable_search(H, 120, m, quadratic_probing);
	cout << endl;
}

void perf()  //functia pentru tabele
{
	int v[10007];
	int m = MAX_SIZE;

	float cazuri[5] = { 0.8, 0.85, 0.9, 0.95, 0.99 };
	float medGas[5], medNegas[5];
	float maxGas[5], maxNegas[5];

	for (int i = 0; i < 5; i++) {
		medGas[i] = 0;
		medNegas[i] = 0;
		maxGas[i] = 0;
		maxNegas[i] = 0;
	}

	for (int t = 0; t < 5; t++) {
		SlotHashTable* H = (SlotHashTable*)malloc(m * sizeof(SlotHashTable)); //H tabela de dispersie pe care o sa lucram
		hashTable_new(H, m);
		int n = 0; //pentru cate elemente sunt in tabela, initial tabela e goala

		FillRandomArray(v, m, 1, 50000);
		int j = 0;

		for (int caz = 0; caz < 5; caz++) {
			while ((float)n / m < cazuri[caz]) {
				hashTable_insert1(H, v[j], m, quadratic_probing);
				n++;
				j++;
			}

			//afisareHashTable(H, m);

			for (int i = 0; i < 3000; i++) {
				efort = 0;
				if (i % 2 != 0) {
					hashTable_search1(H, v[rand() % hashTable_usedSize(H, m)], m, quadratic_probing);
					medGas[caz] = medGas[caz] + efort;

					if (efort > maxGas[caz]) {
						maxGas[caz] = efort;
					}
				}
				else {
					hashTable_search1(H, v[rand() % hashTable_usedSize(H, m)] + 50000, m, quadratic_probing); //adaugam 50000 ca sa nu mai dam de elemente adaugate deja
					medNegas[caz] = medNegas[caz] + efort;

					if (efort > maxNegas[caz]) {
						maxNegas[caz] = efort;
					}

				}
			}
		}

		hashTable_free(H, m); //eliberam tabela de dispersie

	}

	for (int i = 0; i < 5; i++) {
		medGas[i] = medGas[i] / (3000 / 2 * 5);
		medNegas[i] = medNegas[i] / (3000 / 2 * 5);
	}
	
	//generam tabelul
	cout << "Cazuri | Efort mediu elemente gasite | Efort mediu elemente negasite | Efort maxim elemente gasite | Efort maxim elemente negasite" << endl;

	for (int j = 0; j < 5; j++) {
		cout << cazuri[j] << " | " << medGas[j] << " | " << medNegas[j] << " | " << maxGas[j] << " | " << maxNegas[j];
		cout << endl;
	}
	cout << endl;	

}

int main()
{
	//testareAlgoritmi();
	perf();
	return 0;
}

