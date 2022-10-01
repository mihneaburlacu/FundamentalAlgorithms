/*
Burlacu Mihnea-Sebastian Grupa 9
Am implementat 3 algoritmi de sortare: BubbleSort, InsertionSort si SelectionSort
BubbleSort are o complexitate O(n^2) in fiecare caz
InsertionSort are o complexitate O(n^2) in average si worst case, iar in best case(cand sirul este sortat) este O(n) (executa doar for-ul, fara while)
SelectionSort are complexitatea O(n^2) in cele 3 cazuri
Pe best case cel mai rapid algoritm (in total) este InsertionSort, ca numar de atribuiri Bubble si Selection, iar ca numar de comparatii Insertion
Pe average case cel mai rapid algoritm este InsertionSort, ca numar de atribuiri Selection, iar ca numar de comparatii Insertion
Pe worst case cel mai rapid algoritm este SelectionSort,  ca numar de atribuiri Selection, iar ca numar de comparatii Insertion
Deci, am observat ca mereu (in cei 3 algoritmi de sortare) ca numar de atribuiri cel care face cele mai putine atribuiri este Selection, iar cel care cu cele mai putine comparatii este Insertion
Insertion si Bubble sunt stabili, Selection nu e
*/

#include <stdio.h>
#include "Profiler.h"

Profiler p("Sortare");

#define MAX_SIZE 10000
#define STEP_SIZE 500
#define NR_TESTS 5

void afisareVector(int v[], int n)
{
	for (int i = 0; i < n; i++) {
		printf("%d ", v[i]);
	}
	printf("\n");
}

void swap(int* a, int* b)
{
	int aux = *a;
	*a = *b;
	*b = aux;
}

void BubbleSort(int v[], int n)
{
	Operation opComp = p.createOperation("bubbleSort-comp", n);
	Operation opAtr = p.createOperation("bubbleSort-atr", n);

	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - i - 1; j++) {
			opComp.count();
			if (v[j] > v[j + 1]) {
				opAtr.count(3);
				swap(&v[j], &v[j + 1]);
			}
		}
	}
}

void InsertionSort(int v[], int n)
{
	Operation opComp = p.createOperation("insertionSort-comp", n);
	Operation opAtr = p.createOperation("insertionSort-atr", n);

	for (int i = 1; i < n; i++)
	{
		opAtr.count();
		int k = v[i];
		int j = i - 1;
		opComp.count();
		while (j >= 0 && v[j] > k) {
			opAtr.count();
			v[j + 1] = v[j];
			j--;
		}
		opAtr.count();
		v[j + 1] = k;
	}
}

void SelectionSort(int v[], int n)
{
	Operation opComp = p.createOperation("selectionSort-comp", n);
	Operation opAtr = p.createOperation("selectionSort-atr", n);

	for (int i = 0; i < n-1; i++)
	{
		int index = i;
		for (int j = i + 1; j < n; j++)
		{
			opComp.count();
			if (v[index] > v[j]) {
				index = j;
			}
		}
		if (i != index) {
			opAtr.count();
			swap(&v[i], &v[index]);
		}
	}
}

void testareAlgoritmi()
{
	int destinatie[MAX_SIZE];
	int destinatie2[MAX_SIZE];
	int v[] = { 10, 15, 33, 2, 7};
	int n = sizeof(v) / sizeof(v[0]);

	for (int i = 0; i < n; i++)  //pentru a nu schimba valorile din vectorul initial
	{
		destinatie[i] = v[i];
	}


	for (int i = 0; i < n; i++)  //pentru a nu schimba valorile din vectorul initial
	{
		destinatie2[i] = v[i];
	}

	printf("Sirul initial este: ");
	afisareVector(v, n);
	printf("\n");

	printf("Sirul dupa sortare:\n");
	BubbleSort(v, n);
	printf("BubbleSort: ");
	afisareVector(v, n);

	InsertionSort(destinatie, n);
	printf("InsertionSort: ");
	afisareVector(destinatie, n);

	SelectionSort(destinatie2, n);
	printf("SelectionSort: ");
	afisareVector(destinatie2, n);
}

void perf(int order)
{
	int v[MAX_SIZE];
	int copy1[MAX_SIZE];
	int copy2[MAX_SIZE];
	int n;
	for (n = 10; n <= MAX_SIZE; n += STEP_SIZE) {
		for (int t = 0; t < NR_TESTS; t++) {
			FillRandomArray(v, n, 10, 50000, false, order);
			for (int i = 0; i < n; i++)  //pentru a nu schimba valorile din vectorul initial
			{
				copy1[i] = v[i];
				copy2[i] = v[i];
			}
			BubbleSort(v, n);
			InsertionSort(copy1, n);
			SelectionSort(copy2, n);
		}
	}

	p.divideValues("bubbleSort-atr", NR_TESTS);
	p.divideValues("bubbleSort-comp", NR_TESTS);
	p.addSeries("bubbleSort", "bubbleSort-atr", "bubbleSort-comp");

	p.divideValues("insertionSort-atr", NR_TESTS);
	p.divideValues("insertionSort-comp", NR_TESTS);
	p.addSeries("insertionSort", "insertionSort-atr", "insertionSort-comp");

	p.divideValues("selectionSort-atr", NR_TESTS);
	p.divideValues("selectionSort-comp", NR_TESTS);
	p.addSeries("selectionSort", "selectionSort-atr", "selectionSort-comp");

	p.createGroup("atribute", "bubbleSort-atr", "insertionSort-atr", "selectionSort-atr");
	p.createGroup("comparatii", "bubbleSort-comp", "insertionSort-comp", "selectionSort-comp");
	p.createGroup("total", "bubbleSort", "insertionSort", "selectionSort");
}

void perf_all()
{
	perf(UNSORTED);

	p.reset("Best");
	perf(ASCENDING);

	p.reset("worst");
	perf(DESCENDING);

	p.showReport();
}

int main()
{
	//testareAlgoritmi();
	perf_all();
	return 0;
}