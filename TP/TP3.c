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

    while(i > 0 && h[i] < h[p]){     // pior caso do algoritmo é quando o elemento inserido é o menor de todos, pelo que o número de iterações é a quantidade de niveis da min-heap (O(logN))
        swap(h, i, p);              // o num de trocas/comparacoes pode ser dado por: T(N) = (sum_{i = 0, logN} c + s), sendo (s) o custo abstrato do algoritmo swap e (c) o custo de uma comparacao.
        i = p;                      // !!NOTA!! : neste tipo de exercicio de analise de complexidade, quando nao nos indicam um valor concreto para o custo de escritas, comparacoes e leituras num array e/ou entre variaveis, devemos sempre assumir um custo abstrato, i.e., uma constante alfabetica.
        p = (i - 1)/2;              // ainda mais, a analise assimptotica, na determicacao do limite superior de crescimento do custo do algoritmo em funcao do tamanho do input, requer uma analise matematica sobre o comportamento das funcoes. neste caso, o crescimento e maior para a funcao logaritmica, pelo que essa funcao (log) implica um limite superior relativamente a taxa de crescimento, por isso e que fica O(logN).
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

void empty(PriorityQueue *q){ // O(1) 
    q -> tamanho = 0;
}

int isEmpty(PriorityQueue *q){ // O(1) 
    return (q -> tamanho == 0);
}

int add(int x, PriorityQueue *q){ // Melhor Caso: O(1), i.e., queue cheia
    if(q -> tamanho == Max) return -1;
    q -> valores[q -> tamanho] = x;
    bubbleUp(q -> tamanho, q -> valores); // Pior Caso: Vamos a primeira posicao livre do array (custo constante porque vamos apenas inserir na posicao que queremos, sem consultar o array todo) e invocamos a funcao bubblUp logo: T(N) = c + 1 + T(N)bubbleUp + i = O(logN)
    q ->tamanho ++;             // sendo (c) o custo de uma comparacao, e (i) o custo da incrementacao 
}

int remove(PriorityQueue *q, int *rem){ //melhor caso O(1) (a raiz nova não precisa descer), pior caso O(logN) (bubbleDown desce até à folha).
    if(q ->tamanho == 0) return -1;
    *rem = q ->valores[0];
    q -> valores[0] = q -> valores[-- q -> tamanho];
    bubbleDown(0, q ->valores, q ->tamanho);
    return 1;
}

//5

void heapifyBBDown(int v[], int N){ // Melhor caso: para N fixo e dados já a satisfazer a propriedade de heap, o ciclo corre sempre N/2 vezes e cada bubbleDown custa → Θ(N).
    for(int i = (N - 2)/2; i >= 0; i--) bubbleDown(i, v, N); // Pior caso: Para um N suficiemntemente grande (ou N > 1), o pior caso acontece quando temo de ordenar todos os elementos do vetor. pelo que o custo e dado por: T(N) = sum_{k = 0 logN} [N/2^(k + 1)] * k -> O(N)
}

void heapifyBBUp(int v[], int N){ // Melhor Caso: semelhante ao anterior.
    int i = 0;
    for(; i < N; i++) bubbleUp(i, v); // Pior caso: precorremos o array todo para um N > 1. T(N) = sum_{i = 0 N} T(N)bbup = N * logN -> O(NlogN)
}

//6

void ordenaHeap(int h[], int N){ //acho que esta mal
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
// A analise de complexidade (tirando o caso medio) para tabelas de hash e muito parecida a analise de complexidade em arrays, pelo que nao e nada de extraordinario (e sempre a mesma coisa)
void initEmpty(THash t){
    for(int i = 0; i < Size; i++){ //O(N)
        t[i] = NULL;
    }
}

//2

void add(char *s, THash t){
    int p = hashString(&s, Size);
    int new;
    Nodo *curr;
    for(curr = t[p]; *curr != NULL && (*curr) -> chave != s; curr = &((*curr) -> prox)){ //Melhor caso: a chave a adicionar numa dada posicao p é, até ao momento, única pelo que e a primeira a ser inserida. Para isso, sem assumir custos da funcao malloc, temos: -> O(1) 
        if(curr != NULL){
            (*curr) -> ocorr ++;
            new = 0;
        } else {
            curr = (Nodo *) malloc(sizeof(struct nodo)); // Pior caso: Para uma dada posicao p, a lista ligada de chaves e suficientemente grande, pelo que temo de precorre-la até ao último nodo. Consideramos trivialmente um input para o numero de nodos da lista, geral, N (apesar de nao ser uma abordagem totalmente correta) -> O(N)
            (*curr) -> ocorr = 1;
            (*curr) -> chave = s;
            (*curr) -> prox = t[p];
            t[p] = curr;
            new = 1;
        }
    } return new;
}

//3

int lookup(char *s, THash t){ //Melhor caso: chave a consultar está, numa dada posicao p da tabela, no primeiro nodo da lista ligada. O(1) 
    int m;
    int found;

    int p = hashString(&s, Size);

    Nodo *curr;

    for(curr = t[p]; *curr != NULL && (*curr) -> chave != s; curr = &((*curr) -> prox)){ //Pior caso: a chave a consultar está, numa posicao p, no ultimo nodo da lista ligada. O(N) 
        if(curr != NULL){
            m = (*curr) -> ocorr; found = 1;
        } else found = 0;
    } return found;
}

//4 

int remove(char *s, THash t){
    int p = hashString(&s, Size); int removed; // Analise de complexidade semelhante ao exercicio anterior
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
    int p = hashString(&s, HSize); //Melhor caso: a primeira posicao p esta livre. O(1)

    int i = HSize;
    for(; i > 0 && t[p].status != Free && t[p].chave != s; i--){ // Pior caso: o array esta todo ocupado, pelo que vamos ter de o precorrer todo (da frente para tras) ate o algoritmo chegar a conclusao que nao existem posicoes free, logo: O(N)
        p = (p + 1) % HSize;
    }

    if(i == 0) p = -1;
    return p;
}

//2

void init(THashOpen t){
    int i = 0;

    for(; i < HSize; i++){ // O(N)
        t[i].status = Free;
    }
}

void add(char *s, THashOpen t){
    int p = where(s, HSize); //Melhor caso: o melhor caso deste algoritmo é o melhor caso do algorimto where -> O(1)
    if(p < 0) return;

    else if(t[p].chave == s){ // Pior caso: O(N) (obviamente tendo em conta as comparacoes, escritas, etc)
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
    int p = where(s, t); // semelhante ao anterior
    int info = -1;
    if(p >= 0 && t[p].chave == s){
        info = t[p].ocorr;
    } return info;
}

int remove(char *s, THashOpen t){
    int p = where(s, t); //semelhante ao anterior porque num array, a remocao tem custo constante. mas numa thash temos de usar o algoritmo where, logo ficamos com um custo, maioritariamente, linear
    if(p >= 0 && t[p].chave == s && t[p].status == Used){
        t[p].status = Del;
    } else return -1;
}

//3

void garbageC(THashOpen h){
    THashOpen temp = malloc(HSize * sizeof(struct bucket));

    for(int i = 0; i < HSize; i++){
        temp[i].status = Free;  // Nao existe bem o melhor ou pior caso. o custo desta funcao e quase sempre linear
    }

    rehash(HSize, h, HSize, temp); //rehash esta definida em datasAeC.

    free(h);
    h = temp;
}






