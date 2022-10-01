/*
Burlacu Mihnea-Sebastian
Grupa 9
Am implementat 2 algoritmi pentru constructia unui Heap: Top-Down, Bottom-up si algoritmul de sortare: Heap-Sort
La analiza dintre cei 2 algoritmi de constructie (in average case) am observat ca Bottom-up e mai eficient decat Top-Down
Heapify are complexitatea O(log n)
Pentru BottomUp: avem O(n) apeluri ale functiei Heapify => O(n log n) complexitate
De asemenea, HeapSort-ul are complexitatea tot O(n log n)
Functia Increase Key are complexitatea O(log n) (inaltimea heap-ului este log n), Insert tot O(log n)
Prin inserarile repetate, Top-Down-ul are complexitatea O(n log n)
*/

#include <iostream>
#include "Profiler.h"

Profiler p("Heap");

#define MAX_SIZE 10000
#define STEP_SIZE 500
#define NR_TESTS 5

using namespace std;

void afisareHeap(int v[], int n)
{
	for (int i = 0; i < n; i++) {
		cout << v[i] << " ";
	}
	cout << "\n";
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

void maxHeapify(int v[], int n, int i)
{
	Operation opComp = p.createOperation("BottomUp-comp", n);
	Operation opAtr = p.createOperation("BottomUp-atr", n);

	int maxH = i;
	int left = Left(i);
	int right = Right(i);

	opComp.count();
	if (left < n && v[left] > v[maxH]) {
		maxH = left;
	}
	opComp.count();
	if (right < n && v[right] > v[maxH]) {
		maxH = right;
	}
	if (maxH != i) {
		opAtr.count(3);
		swap(&v[i], &v[maxH]);
		maxHeapify(v, n, maxH);
	}
}

void build_maxHeap_bottomUp(int v[], int n)
{
	int start = (n / 2) - 1;
	for (int i = start; i >= 0; i--) {
		maxHeapify(v, n, i);
	}
}

void maxHeap_increaseKey(int v[], int i, int key, int n)
{
	Operation opComp = p.createOperation("TopDown-comp", n);
	Operation opAtr = p.createOperation("TopDown-atr", n);

	opComp.count();
	if(key < v[i]) {
		exit(10);
	}
	opAtr.count();
	v[i] = key;
	//int parent = Parent(i);
	opComp.count();
	while (i > 0 && v[Parent(i)] < v[i]) {
		opAtr.count(3);
		swap(&v[i], &v[Parent(i)]);
		i = Parent(i);
	}
}

void maxHeap_insert(int v[], int *heapSize, int key, int n)
{
	v[*heapSize] = INT_MIN;
	maxHeap_increaseKey(v, *heapSize, key, n);
	*heapSize = *heapSize + 1;
}

void build_maxHeap_topDown(int v[], int n)
{
	int heapSize = 0;
	for (int i = 0; i < n; i++) {
		maxHeap_insert(v, &heapSize, v[i], n);
	}
}

void heapSort(int v[], int n)
{
	build_maxHeap_bottomUp(v, n);
	for (int i = n - 1; i >= 1; i--) {
		swap(&v[0], &v[i]);
		maxHeapify(v, i, 0);
	}
}

void afisareVector(int v[], int n)
{
	cout << "Vectorul sortat (heapSort) este: ";
	for (int i = 0; i < n; i++) {
		cout << v[i] << " ";
	}
	cout << endl;
}

void testareAlgoritmi()
{
	int v[] = {4, 7, 3, 30, 15, 2, 42, 13};
	int n = sizeof(v) / sizeof(v[0]);
	int copy1[100];
	int copy2[100];

	for (int i = 0; i < n; i++) {
		copy1[i] = v[i];
		copy2[i] = v[i];
	}

	cout << "Vectorul initial pentru Heap este: " << endl;
	for (int i = 0; i < n; i++) {
		cout << v[i] << " ";
	}
	cout << endl << endl;
	cout << "Heap-ul sub forma de array: " << "\n";

	cout << "Bottom-up: ";
	build_maxHeap_bottomUp(v, n);
	afisareHeap(v, n);

	cout << "Top-down: ";
	build_maxHeap_topDown(copy1, n);
	afisareHeap(copy1, n);
	cout << endl << endl;

	heapSort(copy2, n);
	afisareVector(copy2, n);

}

void perf(int order)
{
	int v[MAX_SIZE];
	int copy1[MAX_SIZE];
	int NR;
	for (int NR = 100; NR <= MAX_SIZE; NR += STEP_SIZE) {
		for (int t = 0; t < NR_TESTS; t++) {
			FillRandomArray(v, NR, 100, 50000, false, order);
			for (int i = 0; i < NR; i++) {
				copy1[i] = v[i];
			}
			build_maxHeap_bottomUp(v, NR);
			build_maxHeap_topDown(copy1, NR);
		}
	}

	p.divideValues("BottomUp-comp", NR_TESTS);
	p.divideValues("BottomUp-atr", NR_TESTS);
	p.addSeries("BottomUp", "BottomUp-comp", "BottomUp-atr");

	p.divideValues("TopDown-comp", NR_TESTS);
	p.divideValues("TopDown-atr", NR_TESTS);
	p.addSeries("TopDown", "TopDown-comp", "TopDown-atr");

	p.createGroup("Atribuiri", "BottomUp-atr", "TopDown-atr");
	p.createGroup("Comparatii", "BottomUp-comp", "TopDown-comp");
	p.createGroup("Total", "BottomUp", "TopDown");
}

void perf_all()
{
	//doar pentru average:
	perf(UNSORTED);
	p.showReport();
}


int main()
{
	//testareAlgoritmi();
	perf_all();
	return 0;
}