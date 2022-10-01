/*
Burlacu Mihnea-Sebastian
Grupa 9
Am implementat 2 algoritmi de sortare(HeapSort, QuickSort) si i-am analizat
Pe langa acestia am implementat si algoritmul QuickSelect sau RandomizeSelect cu care aflam care este al k-lea cel mai mic numar dintr-un array

Am observat ca QuickSort-ul este mai rapid decat HeapSort

HeapSort-ul are complexitatea O(n log n)

QuickSort-ul functioneaza astfel: se ia pivotul, incepe compararea elementelor => elementele mai mici se pun in stanga, iar cele mai mari se pun in dreapta
QuickSort-ul are in best case O(n log n) complexitatea (avem partitionarea n/2 si n/2-1). Deci, best case nu este atunci cand elementele sunt sortate crescator, 
        ci atunci cand dupa partitionare pivotul ajunge in mijloc pentru a avea cele 2 parti n/2, adica pivotul este la pozitita (left+right)/2 ca sa se faca cat mai putine operatii
In average case O(n log n)  (partitionarea 9n/10 si n/10)
Iar in worst case, QuickSort-ul are O(n^2) (partitionarea n-1 si 0 elem) si se produce atunci cand array-ul este sortat (crescator in exemplul meu)

In forma in care l-am implementat QuickSort-ul nu este stabil
Nici HeapSort-ul nu este stabil
*/

#include <iostream>
#include "Profiler.h"

Profiler p("Sortare");

#define MAX_SIZE 10000
#define STEP_SIZE 500
#define NR_TESTS 5

using namespace std;

int n;

void afisareVector(int v[], int size)
{
	for (int i = 0; i < size; i++) {
		cout << v[i] << " ";
	}
	cout << endl;
}

void swap(int* a, int* b)
{
	int aux = *a;
	*a = *b;
	*b = aux;
}

int Parent(int i) //pentru parinte
{
	return i / 2;
}

int Left(int i)  //pentru copilul stang
{
	return (2 * i + 1);
}

int Right(int i) //pentru copilul drept
{
	return (2 * i + 2);
}

void maxHeapify(int v[], int size, int i)
{
	Operation opTotal1 = p.createOperation("HeapSort1-total", n);

	int maxH = i;
	int left = Left(i);
	int right = Right(i);

	opTotal1.count();
	if (left < size && v[left] > v[maxH]) {
		maxH = left;
	}
	opTotal1.count();
	if (right < size && v[right] > v[maxH]) {
		maxH = right;
	}
	if (maxH != i) {
		opTotal1.count(3);
		swap(&v[i], &v[maxH]);
		maxHeapify(v, size, maxH);
	}
}

void build_maxHeap_bottomUp(int v[], int size)
{
	int start = (size / 2) - 1;
	for (int i = start; i >= 0; i--) {
		maxHeapify(v, size, i);
	}
}

void heapSort(int v[], int size)
{
	Operation opTotal2 = p.createOperation("HeapSort2-total", n);

	build_maxHeap_bottomUp(v, n);
	for (int i = size - 1; i >= 1; i--) {
		opTotal2.count(3);
		swap(&v[0], &v[i]);
		maxHeapify(v, i, 0);
	}
}

int partitionQuickSort(int v[], int l, int r)
{
	Operation opTotal = p.createOperation("QuickSort-total", n);

	opTotal.count();
	int x = v[r];
	int i = l - 1;

	for (int j = l; j <= r - 1; j++) {
		opTotal.count();
		if (v[j] < x) {
			opTotal.count(3);
			i = i + 1;
			swap(&v[i], &v[j]);
		}
	}

	opTotal.count(3);
	swap(&v[i + 1], &v[r]);

	return (i + 1);
}

void quickSort(int v[], int l, int r)
{
	if (l < r) {
		int q = partitionQuickSort(v, l, r);

		quickSort(v, l, q - 1);
		quickSort(v, q + 1, r);
	}
}

int partitionQuickSelect(int v[], int l, int r)
{
	int x = v[r];
	int i = l;

	for (int j = l; j <= r; j++) {
		if (v[j] < x) {
			swap(&v[i], &v[j]);
			i = i + 1;
		}
	}

	swap(&v[i], &v[r]);

	return i;
}

int quickSelect(int v[], int l, int r, int i)
{
	if (l == r) {
		return v[l];
	}

	int q = partitionQuickSelect(v, l, r);

	if (q == i - 1) {
		return v[q];
	}
	else if (q < i - 1) {
		return quickSelect(v, q + 1, r, i);
	}
	else {
		return quickSelect(v, l, q - 1, i);
	}
}

void testareAlgoritmi()
{
	int v[] = { 100, 120, 3, 15, 44, 77, 12 };
	n = sizeof(v) / sizeof(v[0]);
	int copy1[100];
	int copy2[100];

	for (int i = 0; i < n; i++) {
		copy1[i] = v[i];
		copy2[i] = v[i];
	}

	cout << "Vectorul initial este: " << endl;
	for (int i = 0; i < n; i++) {
		cout << v[i] << " ";
	}
	cout << endl << endl;

	cout << "Dupa sortare: " << endl;

	cout << "Heap Sort: ";
	heapSort(v, n);
	afisareVector(v, n);

	cout << "Quick Sort: ";
	quickSort(copy1, 0, n - 1);
	afisareVector(copy1, n);

	//al catelea element cel mai mic
	int i = 4;

	cout << "Elementul al " << i << "-lea cel mai mic este: ";
	cout << quickSelect(copy2, 0, n - 1, i);;

}

//void perf1(int order)
void perf1()
{
	int v[MAX_SIZE];
	int copy1[MAX_SIZE];
	for (int t = 0; t < NR_TESTS; t++) {
		for (n = 100; n <= MAX_SIZE; n += STEP_SIZE){
			//FillRandomArray(v, n, 100, 50000, false, order);
			FillRandomArray(v, n);
			for (int i = 0; i < n; i++) {
				copy1[i] = v[i];
			}
			heapSort(v, n);
			quickSort(copy1, 0, n - 1);
		}
	}

	p.divideValues("HeapSort1-total", NR_TESTS);
	p.divideValues("HeapSort2-total", NR_TESTS);
	p.addSeries("HeapSort-total", "HeapSort1-total", "HeapSort2-total");

	p.divideValues("QuickSort-total", NR_TESTS);
	
	p.createGroup("Total", "HeapSort-total", "QuickSort-total");

	p.showReport();
}

void perf2(int order)
{
	int v[MAX_SIZE];
	for (int t = 0; t < NR_TESTS; t++) {
		for (n = 100; n <= MAX_SIZE; n += STEP_SIZE) {
			FillRandomArray(v, n, 100, 50000, false, order);
			quickSort(v, 0, n - 1);
		}
	}

	p.divideValues("QuickSort-total", NR_TESTS);
}


void perf_all()
{
	//worst cand e sortat vectorul
	p.reset("WORST");
	perf2(ASCENDING);

	//best cand obtinem cu ajutorul pivotului cele 2 partitii n/2 
	//pivotul ar trebui sa fie la l + r / 2
	p.reset("BEST");
	perf2(UNSORTED);

	p.showReport();
}



int main()
{
	//testareAlgoritmi();
	perf1();
	perf_all();
	return 0;
}