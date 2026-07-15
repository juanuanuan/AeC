#include <stdlib.h> 

#define STATUS_FREE 0 
#define STATUS_USED 1
#define STATUS_DELETED 2
#define HSIZE 11

/*
This code contains the basic algorithms to work with some data structures.
*/

void swap(int v[], int i,  int j){
    int temp;

    temp = v[i]; 
    v[i] = v[j];
    v[j] = temp;
}


typedef struct bucket{
    int status;
    int key, info;
    struct bucket *next;
} *Bucket;

typedef struct thash{
    int size;
    Bucket *tabela;
} *THash;

typedef Bucket HashTable [HSIZE];

int hash(int k, int N){
    return k % N;
}

THash createT(int N){
    THash t = malloc(sizeof(struct thash));

    t->size = N;
    t -> tabela = malloc(N*sizeof(Bucket));
    for(int i = 0; i < N; t->tabela[i++] = NULL);

    return t;
}

int duplicateT(THash t){
    int i = 0;
    Bucket *new = malloc(2 * t -> size *sizeof(Bucket));

    if(new == NULL) return -1;

    for(; i < t -> size; i++) new[i] = t -> tabela[i];

    for(; i < 2 * t ->size; i++) new[i] = NULL;

    t -> size *= 2;
    free(t ->tabela);
    t -> tabela =  new;

    return 0;
}

void initHT(HashTable h){
    for(int i = 0; i < HSIZE; h[i++]->status = STATUS_FREE);
}

int find_probe(HashTable h, int k){
    int p = hash(k, HSIZE);
    int count;

    for(count = HSIZE; count >= 0 && h[p]->status != STATUS_FREE && h[p]->key != k; count --) p = (p+1) % HSIZE;

    int temp;

    if(h[p]->key != k){
        if(count == 0) p = -1;

        else {
            temp = p;
            while(count > 0 && h[p]->status != STATUS_FREE && h[p]->key != k){
                count --;
                p = (p+1) % HSIZE;
            }

            if(h[p]->key != k) p = temp;
        }

    }

    return p;
}

int lookup(HashTable h, int k, int *i){
    int p = find_probe(h,k);
    int found = 0;

    if(p >= 0 && h[p]->key == k && h[p]->status == STATUS_USED){
        *i = h[p]->info;
        found = 1;
    } return found;
}

int update(HashTable h, int k, int i){
    int p = find_probe(h,k);
    if(p < 0) return -1;
    if(h[p]->key == k) {
        h[p]->info = i;
        h[p]->status = STATUS_USED;
    } else {
        h[p]->info = i;
        h[p]->key = k;
        h[p]->status = STATUS_USED;
    }

    return 0;
}

int delete(HashTable h, int k){
    int p = find_probe(h,k);

    if(p >= 0 && h[p]->key == k && h[p]->status == STATUS_USED){
        h[p]->status = STATUS_DELETED;
    } else return -1;

    return 0;
}

void rehash(int S, Bucket source[S], int T, Bucket target[T]){
    int is, it;

    for(it = 0; it < T; it++){
        target[it]->status = STATUS_FREE;
    }

    for(is = 0; is < S; is++){
        if(source[is]->status == STATUS_USED) it = hash(source[is]->key, T);
        while(target[it]->status != STATUS_FREE) it = (it + 1) % HSIZE;

        target[it]->status = STATUS_USED;
        target[it]->key = source[is]->key;
        target[it]->info = source[is]->info;
    }
}


void gargabeC(HashTable h){
    Bucket *temp = malloc(HSIZE * sizeof(Bucket));

    for(int i = 0; i < HSIZE; i++){
        temp[i] = malloc(sizeof(Bucket));
    }

    rehash(HSIZE, h, HSIZE, temp);

    free(h);
    h = temp;
}

typedef struct hashtabledyn{
    int size;
    int used;
    Bucket *Table;
} HashTableDyn;


void doubleTable(HashTableDyn *h){
    HashTableDyn *temp = malloc(h -> size * 2 * sizeof(Bucket));

    for(int i = 0; i < temp->size; i++){
        temp[i].Table = malloc(sizeof(Bucket));
    }

    rehash(h, HSIZE, temp, temp->size);

    free(h);
    h = temp;
}

typedef struct bst{
    int key, info;
    struct bst *left, *right;
} *BST;


int update(BST *a, int k, int i){

    while(*a != NULL && (*a) -> key != k){
        if((*a)->key > k) a = &((*a)->left);
        else a = &((*a)->right);
    }

    if(*a == NULL){
        *a = (BST) malloc(sizeof(struct bst));
        if(*a == NULL) return -1;
        (*a)->key = k;
        (*a)->info = i;
        (*a)->left = (*a)->right = NULL;
        return 1;
    } else (*a)->info = i;

    return 0;
}

#define NV 6

typedef struct edge{
    int dest;
    int cost;
    struct edge *next;
} *EList;

typedef EList Graph [NV];

int indegree(Graph g, int v){
    int i, r; EList curr;

    for(i = r = 0; i < NV; i++){
        for(curr = g[i]; curr != NULL; curr = curr -> next)
            if(curr -> dest == v) r++;
    }

    return r;
}

int outdegree(Graph g, int v){
    int i = 0; EList curr;

    for(curr = g[v]; curr != NULL; curr = curr -> next) i++;

    return i;
}

int capacidade(Graph g, int v){ //indegree and outdegree capacity
    EList curr;
    int sumIn = 0;
    int sumOut = 0;
    int i;

    for(curr = g[v]; curr != NULL; curr = curr -> next){
        sumOut += curr -> cost;
    }

    for(int i = 0; i < NV; i++){
        for(curr = g[i]; curr != NULL; curr = curr -> next){
            if(curr -> dest = v) sumIn += curr->cost;
        }
    }

    return sumIn - sumOut;
}

int maxCap(Graph g){
    int maxSum = 0;
    int sum = 0;
    int i;
    int maxI;

    for(i = 0; i < NV; i++){
        sum = capacidade(g, i);

        if(maxSum < sum) {
            maxI = i;
            maxSum = sum;
        }
        sum = 0;
    }

    return maxI;
}

typedef struct fringe *Fringe;

int bft(Graph g, int o, int ant[]){ //Breadth-First Traversal
    int v; int q[NV];
    EList curr;
    int count = 0, inicio, fim;

    for(v = 0; v < NV; v++) ant[v] = -2;

    inicio = fim = 0;

    q[fim++] = 0;
    ant[0] = -1;

    while(inicio < fim){
        o = q[inicio++];  
        count ++;

        for(curr = g[o]; curr != NULL; curr = curr -> next)
            if(g[curr -> dest] == 0)
                ant[curr->dest] = 0;
                q[fim++] = curr -> dest;  
    } return count;


}

void bubbleup(int v[], int i){
    int k = (i-1)/2;
    int p = k;

    while(i > 0 && v[i] < v[p]){
        swap(v,i,p);
        i = p;
        p = k;
    }
}

void bubbledown(int v[], int N, int i){
    int root = 2*i+1;
    while(root < N){
        if(root + 1 < N && v[root] > v[root + 1])
            root = root + 1;
        if(v[i] < v[root])
            break;
        swap(v,i,root);
        i = root;
        root = 2*i+1;
    }
}

void heapify(int v[], int N){
    int i = 0;
    for(i = (N-2)/2; i>=0; i--) bubbledown(v,N,i);
}











