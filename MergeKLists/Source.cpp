/*
Burlacu Mihnea-Sebastian
Grupa 9

Am implementat algoritmul de interclasare a k liste ordonate
Pentru eficientizare am folosit un MinHeap de dimensiune k pentru a scoate de fiecare data radacina(elementul minim din heap)
Pentru acest MinHeap am folosit functiile de Heapify si Build Bottom Up
Pentru lucrul cu listele am avut nevoie de operatiile newList (pentru a aloca o noua lista), insertLast (pentru a introduce in lista finala elemente pe rand la sfarsit) si removeFirst (pentru a sterge elementele din cele k liste introduse deja in lista finala)

Cream si construim un MinHeap in care inseram la inceput primul elem. din fiecare lista
Cat timp MinHeap-ul nu este gol: scoatem din MinHeap minimul(radacina) si o introducem in lista finala
   apoi, daca mai exista elemente in lista din care am ales minimul, scoatem urmatorul element din lista respectiva si-l introducem in heap
   populam lista finala cu elemente pana cand nu mai avem elemente in cele k liste

Complexitatea inserarii la sfarsitul listei este O(1)
Stergerea de la inceput este tot O(1)

Interclasarea se face in O(n * log k)
Spatiul auxiliar este O(k) pentru MinHeap-ul de dimensiune k

Am generat cazurile in care avem 5 liste (k1 = 5), 10 liste (k2 = 10), 100 liste (k3 = 100) si cazul in care numarul de liste (k) este variabil, iar n = 10000
*/

#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"

Profiler prfl("Lists");

#define MAX_SIZE 10000
#define STEP_SIZE 500
#define NR_TESTS 5

int opTotal; //pentru operatiile de la grafice

int n; // nr. elem. din liste
int k; // k liste

typedef struct NodeSL {
    int data;
    struct NodeSL* next;
} NodeSL;

typedef struct ListSL {
    NodeSL* first;
    NodeSL* last;
}ListSL;



ListSL* listSL_new()
{
    ListSL* list = (ListSL*)malloc(sizeof(ListSL));

    if (list == NULL) {  //lista nu s-a putut aloca
        printf("Eroare");
        exit(3);
    }

    list->first = NULL;
    list->last = NULL;

    return list;
}

void listSL_free(ListSL* list)
{
    if (list == NULL) { //lista nu e alocata
        return;
    }

    NodeSL* it = list->first;
    while (it != NULL) {
        NodeSL* next = it->next;
        free(it);
        it = next;
    }

    free(list);
}

void listSL_insert_last(ListSL* list, int givenKey)
{
    if (list == NULL) {  //lista nu e alocata
        return; 
    }

    NodeSL* p = (NodeSL*)malloc(sizeof(NodeSL));
    p->data = givenKey;
    p->next = NULL;

    opTotal++;
    //daca lista este goala noul nod este si primul si ultimul element
    if (list->last == NULL) {
        opTotal++;
        list->first = p;
        list->last = p;
    }
    //daca nu este goala, noul nod devine ultimul nod din lista
    else {
        opTotal++;
        list->last->next = p;
        list->last = p;
    }
}

void listSL_remove_first(ListSL* list)
{
    if (list == NULL) { //lista nu e alocata
        return;
    }

    opTotal++;
    if (list->first == NULL) {  //daca nu are elemente
        return;
    }

    opTotal++;
    //al doilea nod devine primul
    NodeSL* p = list->first;
    list->first = list->first->next;

    free(p);
}

void listSL_print(ListSL* list)
{
    if (list == NULL) {
        printf("Lista e goala");
    }

    NodeSL* p = list->first;

    //parcurgem lista element cu element
    while (p != NULL) {
        printf("%d->", p->data);
        p = p->next;
        if (p == NULL) {
            printf("NULL");
        }
    }
    printf("\n");
}

void swapListSL(ListSL* list1, ListSL* list2)
{
    ListSL listAux;
    listAux = *list1;
    *list1 = *list2;
    *list2 = listAux;
}

int compareFunction(int* a, int* b)
{
    int val1 = *a;
    int val2 = *b;
    if (val1 == val2) {
        return 1;
    }
    return 0;
}

int Parent(int i) //pentru parinte Heap
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

void minHeapify(ListSL* vList[], int i, int *heapSize)
{
    //Operation opTotal1 = prfl.createOperation("Heapify1-total", n);

    int minHeap = i;
    int left = Left(i);
    int right = Right(i);

    opTotal++;
    if (left < *heapSize && vList[left]->first->data < vList[minHeap]->first->data) {
        minHeap = left;
    }

    opTotal++;
    if (right < *heapSize && vList[right]->first->data < vList[minHeap]->first->data) {
        minHeap = right;
    }

    if (minHeap != i) {
        opTotal += 3;
        swapListSL(vList[i], vList[minHeap]);
        minHeapify(vList, minHeap, &(*heapSize));
    }
}

void buildHeap_bottumUp(ListSL* vList[], int *heapSize)
{
    for (int i = *heapSize / 2 - 1; i >= 0; i--)
    {
        minHeapify(vList, i, &(*heapSize));
    }
}

ListSL* mergeKLists(ListSL* vList[], int k)
{

    //Operation opTotal2 = prfl.createOperation("MergeKLists-total", n);

    buildHeap_bottumUp(vList, &k);

    int copyK = k; //ca sa pastram numar de liste k intact

    ListSL* bigList = listSL_new();  //Lista finala o alocam

    while (copyK > 0) {
        listSL_insert_last(bigList, vList[0]->first->data);
        listSL_remove_first(vList[0]);

        if (vList[0]->first == NULL) {
            opTotal++;
            free(vList[0]);
            vList[0] = vList[copyK - 1];
            copyK--;
        }
        minHeapify(vList, 0, &copyK);
    }

    return bigList;

}

void testareAlgoritmi()
{
    n = 5; //nr elem
    k = 5; //nr liste

    int v[100];
    ListSL* vList[100];

    for (int i = 0; i < k; i++) {
        FillRandomArray(v, n, 0, 100, false, true);

        vList[i] = listSL_new();

        for (int j = 0; j < n; j++) {
            listSL_insert_last(vList[i], v[j]);
        }
    }

    for (int t = 0; t < k; t++) {
        listSL_print(vList[t]);
    }
    printf("\n");

    ListSL* bigList = mergeKLists(vList, k);

    printf("Lista finala: ");
    listSL_print(bigList);
}

void perf()
{
    //facem teste pentru 5, 10, 100 liste si respectiv nr. variabil de liste cu n = 10 000
    //pentru 5 liste
    int k1 = 5;

    for (int t = 0; t < NR_TESTS; t++)
    {
        for (int n = 100; n <= MAX_SIZE; n += STEP_SIZE)
        {
            opTotal = 0;
            ListSL* vList[7];
            int v[20001];

            for (int i = 0; i < k1; i++)
            {
                FillRandomArray(v, n / k1, 10, 10000, false, ASCENDING);
                vList[i] = listSL_new();

                for (int j = 0; j < n / k1; j++)
                {
                    listSL_insert_last(vList[i], v[j]);
                }
            }

            ListSL* bigList = mergeKLists(vList, k1);
            Operation nrOpK5 = prfl.createOperation("nrOp k=5", n);
            nrOpK5.count(opTotal);
        }
    }

    //pentru 10 liste
    int k2 = 10;

    for (int t = 0; t < NR_TESTS; t++)
    {
        for (int n = 100; n <= MAX_SIZE; n += STEP_SIZE)
        {
            opTotal = 0;
            ListSL* vList[15];
            int v[20001];

            for (int i = 0; i < k2; i++)
            {
                FillRandomArray(v, n / k2, 10, 10000, false, ASCENDING);
                vList[i] = listSL_new();

                for (int j = 0; j < n / k2; j++)
                {
                    listSL_insert_last(vList[i], v[j]);
                }
            }

            ListSL* bigList = mergeKLists(vList, k2);
            Operation nrOpK10 = prfl.createOperation("nrOp k=10", n);
            nrOpK10.count(opTotal);
        }
    }

    //pentru 100 liste
    int k3 = 100;

    for (int t = 0; t < NR_TESTS; t++)
    {
        for (int n = 100; n <= MAX_SIZE; n += STEP_SIZE)
        {
            opTotal = 0;
            ListSL* vList[103];
            int v[20001];

            for (int i = 0; i < k3; i++)
            {
                FillRandomArray(v, n / k3, 10, 10000, false, ASCENDING);
                vList[i] = listSL_new();

                for (int j = 0; j < n / k3; j++)
                {
                    listSL_insert_last(vList[i], v[j]);
                }
            }

            ListSL* bigList = mergeKLists(vList, k3);
            Operation nrOpK100 = prfl.createOperation("nrOp k=100", n);
            nrOpK100.count(opTotal);
        }
    }

    //pentru k variabil cu n = 10000
    for (int t = 0; t < NR_TESTS; t++)
    {
        n = MAX_SIZE; //10 000
        for (k = 10; k <= 500; k += 10)
        {
            opTotal = 0;
            ListSL* vList[600];
            int v[20001];

            for (int i = 0; i < k; i++)
            {
                FillRandomArray(v, n / k, 10, 10000, false, ASCENDING);
                vList[i] = listSL_new();

                for (int j = 0; j < n / k; j++)
                {
                    listSL_insert_last(vList[i], v[j]);
                }
            }

            ListSL* bigList = mergeKLists(vList, k);
            Operation nrOpN10000 = prfl.createOperation("nrOp variabil", k);
            nrOpN10000.count(opTotal);
        }
    }

    prfl.divideValues("nrOp k=5", 5);
    prfl.divideValues("nrOp k=10", 5);
    prfl.divideValues("nrOp k=100", 5);
    prfl.createGroup("nrOp", "nrOp k=5", "nrOp k=10", "nrOp k=100");

    prfl.divideValues("nrOp variabil", 5);
    prfl.showReport();

}

int main()
{
    //testareAlgoritmi();
    perf();
    return 0;
}