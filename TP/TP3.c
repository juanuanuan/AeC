#include <stdlib.h>
//funcao de troca
void swap(int v[], int i, int j){
    int temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}
// funcao tipica de hash
int hash(int i, int j){
    return i % j;
}

int hashString(char i, int j){ 
    return atoi(i) % j;
}

//Min-Heaps

//2 


void bubbleUp(int i, int h[]){
    int p = (i - 1)/2;

    while(i > 0 && h[i] < h[p]){ // pior caso do algoritmo é quando o elemento inserido é o menor de todos, pelo que o número de iterações é a quantidade de niveis da min-heap (O(logN))
        swap(h, i, p);              // o num de trocas/comparacoes pode ser dado por: T(N) = (sum_{i = 0, logN} c + s), sendo (s) o custo abstrato do algoritmo swap e (c) o custo de uma comparacao.
        i = p;
        p = (i - 1)/2;
    }
}

//3

void bubbleDown(int i, int h[], int N){
    int f = 2 * i + 1;
    while(f < N){
        if(f + 1 < N && h[f+1] < h[f]){ // o pior caso do algoritmo e quando o elemento que queremos empurrar para baixo e o maior elemento da heap. pelo que a condicao do primeiro if e sempre verdade. O(logN). 
            f = f + 1;                  // T(N) = (sum_{i = 0, logN} 2 * c + s) -> O(logN), novamente tendo em conta que: (s) o custo abstrato do algoritmo swap e (c) o custo de uma comparacao.
        }

        if(h[f] > h[i]) break;
        swap(h, i, f);
        i = f; f = 2 * i + 1;
    }
}

//4

#define Max 100

typedef struct pQueue{
    int valores [Max];
    int tamanho;
} PriorityQueue;

void empty(PriorityQueue *q){
    q -> tamanho = 0;
}

int isEmpty(PriorityQueue *q){
    return (q -> tamanho == 0);
}

int add(int x, PriorityQueue *q){
    if(q -> tamanho == Max) return -1;
    q -> valores[q -> tamanho] = x;
    bubbleUp(q -> tamanho, q -> valores);
    q ->tamanho ++;
}

int remove(PriorityQueue *q, int *rem){
    if(q ->tamanho == 0) return -1;
    *rem = q ->valores[0];
    q -> valores[0] = q -> valores[-- q -> tamanho];
    bubbleDown(0, q ->valores, q ->tamanho);
    return 1;
}

//5

void heapifyBBDown(int v[], int N){
    int i = 0;
    for(i = (N - 2)/2; i >= 0; i--) bubbleDown(i, v, N);
}

void heapifyBBUp(int v[], int N){
    int i = 0;
    for(; i < N; i++) bubbleUp(i, v);
}

//6

void ordenaHeap(int h[], int N){
    int i = 0;

    while(i < N - 1){
        if(h[i] > h[i + 1]) bubbleDown(i, h, N);
        i++;
    }
}

//THash 
//Chaining (Closed Adressing)


#define Size 11

typedef struct nodo{
    char *chave; int ocorr;
    struct nodo *prox;
} *Nodo, *THash [Size];

//1

void initEmpty(THash t){
    for(int i = 0; i < Size; i++){
        t[i] = NULL;
    }
}

//2

void add(char *s, THash t){
    int p = hashString(&s, Size);
    int new;
    Nodo *curr;
    for(curr = t[p]; *curr != NULL && (*curr) -> chave != s; curr = &((*curr) -> prox)){
        if(curr != NULL){
            (*curr) -> ocorr ++;
            new = 0;
        } else {
            curr = (Nodo *) malloc(sizeof(struct nodo));
            (*curr) -> ocorr = 1;
            (*curr) -> chave = s;
            (*curr) -> prox = t[p];
            t[p] = curr;
            new = 1;
        }
    } return new;
}

//3

int lookup(char *s, THash t){
    int m;
    int found;

    int p = hashString(&s, Size);

    Nodo *curr;

    for(curr = t[p]; *curr != NULL && (*curr) -> chave != s; curr = &((*curr) -> prox)){
        if(curr != NULL){
            m = (*curr) -> ocorr; found = 1;
        } else found = 0;
    } return found;
}

//4 

int remove(char *s, THash t){
    int p = hashString(&s, Size); int removed;
    Nodo *curr, temp;
    for(curr = t[p]; *curr != NULL && (*curr) -> chave != s; curr = &((*curr) -> prox)){
        if(curr != NULL){
            temp = *curr;
            (*curr) = (*curr) -> prox;
            free(temp); removed = 1;
        } else removed = 0;
    } return removed;
}


//Open Adressing

//1

#define HSize 11
#define Free 0
#define Used 1
#define Del 2

typedef struct bucket{
    int status;
    char *chave; int ocorr;
} THashOpen [HSize];

int where(char *s, THashOpen t){
    int p = hashString(&s, HSize);

    int i = HSize;
    for(; i > 0 && t[p].status != Free && t[p].chave != s; i--){
        p = (p + 1) % HSize;
    }

    if(i == 0) p = -1;
    return p;
}

//2

void init(THashOpen t){
    int i = 0;

    for(; i < HSize; i++){
        t[i].status = Free;
    }
}

void add(char *s, THashOpen t){
    int p = where(s, HSize);
    if(p < 0) return;

    else if(t[p].chave == s){
        t[p].ocorr ++;
        t[p].status = Used;
        return 1;
    } else {
        t[p].chave = s;
        t[p].ocorr = 1;
        t[p].status = Used;
    }
}

int lookup(char *s, THashOpen t){
    int p = where(s, t);
    int info = -1;
    if(p >= 0 && t[p].chave == s){
        info = t[p].ocorr;
    } return info;
}

int remove(char *s, THashOpen t){
    int p = where(s, t);
    if(p >= 0 && t[p].chave == s && t[p].status == Used){
        t[p].status = Del;
    } else return -1;
}

//3

void garbageC(THashOpen h){
    THashOpen temp = malloc(HSize * sizeof(struct bucket));

    for(int i = 0; i < HSize; i++){
        temp[i].status = Free;
    }

    rehash(HSize, h, HSize, temp); //rehash esta definida em datasAeC.

    free(h);
    h = temp;
}






