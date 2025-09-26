#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define FILA_MAX 5
#define PILHA_MAX 3

// --- Structs ---
typedef struct {
    char tipo; // 'I', 'O', 'T', 'L'
    int id;    // identificador único
} Peca;

typedef struct {
    Peca elementos[FILA_MAX];
    int inicio;
    int fim;
    int total;
} FilaCircular;

typedef struct {
    Peca elementos[PILHA_MAX];
    int topo;
} Pilha;

// --- Funções utilitárias ---
char gerarTipo() {
    char tipos[] = {'I','O','T','L'};
    return tipos[rand() % 4];
}

Peca gerarPeca(int id) {
    Peca p;
    p.tipo = gerarTipo();
    p.id = id;
    return p;
}

// --- Funções da Fila Circular ---
void inicializaFila(FilaCircular *f) {
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
}

int filaCheia(FilaCircular *f) {
    return f->total == FILA_MAX;
}

int filaVazia(FilaCircular *f) {
    return f->total == 0;
}

void enfileirar(FilaCircular *f, Peca p) {
    if(filaCheia(f)) {
        printf("Fila cheia! Impossível adicionar.\n");
        return;
    }
    f->elementos[f->fim] = p;
    f->fim = (f->fim + 1) % FILA_MAX;
    f->total++;
}

Peca desenfileirar(FilaCircular *f) {
    Peca vazio = {'X', -1};
    if(filaVazia(f)) return vazio;

    Peca p = f->elementos[f->inicio];
    f->inicio = (f->inicio + 1) % FILA_MAX;
    f->total--;
    return p;
}

void exibirFila(FilaCircular *f) {
    printf("Fila de peças: ");
    for(int i=0; i<f->total; i++){
        int idx = (f->inicio + i) % FILA_MAX;
        printf("[%c %d] ", f->elementos[idx].tipo, f->elementos[idx].id);
    }
    printf("\n");
}

// --- Funções da Pilha ---
void inicializaPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == PILHA_MAX-1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

void push(Pilha *p, Peca e) {
    if(pilhaCheia(p)) {
        printf("Pilha cheia! Impossível adicionar.\n");
        return;
    }
    p->elementos[++p->topo] = e;
}

Peca pop(Pilha *p) {
    Peca vazio = {'X', -1};
    if(pilhaVazia(p)) return vazio;
    return p->elementos[p->topo--];
}

Peca peek(Pilha *p) {
    Peca vazio = {'X', -1};
    if(pilhaVazia(p)) return vazio;
    return p->elementos[p->topo];
}

void exibirPilha(Pilha *p) {
    printf("Pilha de reserva (Topo->Base): ");
    for(int i=p->topo; i>=0; i--){
        printf("[%c %d] ", p->elementos[i].tipo, p->elementos[i].id);
    }
    printf("\n");
}

// --- Funções de Troca ---
void trocarTopoFilaPilha(FilaCircular *f, Pilha *p) {
    if(filaVazia(f) || pilhaVazia(p)) {
        printf("Não é possível trocar: fila ou pilha vazia.\n");
        return;
    }
    int idxFila = f->inicio;
    Peca temp = f->elementos[idxFila];
    f->elementos[idxFila] = pop(p);
    push(p, temp);
}

void trocarTres(FilaCircular *f, Pilha *p) {
    if(f->total < 3 || p->topo < 2) {
        printf("Não há elementos suficientes para troca múltipla.\n");
        return;
    }
    for(int i=0; i<3; i++){
        int idxFila = (f->inicio + i) % FILA_MAX;
        Peca temp = f->elementos[idxFila];
        f->elementos[idxFila] = p->elementos[p->topo];
        p->elementos[p->topo--] = temp;
    }
}

// --- Main ---
int main() {
    srand(time(NULL));
    FilaCircular fila;
    Pilha pilha;
    int opcao;
    int idCounter = 0;

    inicializaFila(&fila);
    inicializaPilha(&pilha);

    // Preencher fila inicial
    for(int i=0; i<FILA_MAX; i++){
        enfileirar(&fila, gerarPeca(idCounter++));
    }

    do {
        printf("\n");
        exibirFila(&fila);
        exibirPilha(&pilha);
        printf("\nMenu:\n");
        printf("1 - Jogar peça (remover da fila)\n");
        printf("2 - Reservar peça (mover para pilha)\n");
        printf("3 - Usar peça da pilha\n");
        printf("4 - Trocar topo da fila com topo da pilha\n");
        printf("5 - Trocar 3 primeiros da fila com 3 da pilha\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch(opcao){
            case 1: {
                Peca jogada = desenfileirar(&fila);
                if(jogada.id != -1){
                    printf("Peça jogada: [%c %d]\n", jogada.tipo, jogada.id);
                    enfileirar(&fila, gerarPeca(idCounter++));
                }
                break;
            }
            case 2: {
                Peca reservada = desenfileirar(&fila);
                if(reservada.id != -1){
                    push(&pilha, reservada);
                    printf("Peça reservada: [%c %d]\n", reservada.tipo, reservada.id);
                    enfileirar(&fila, gerarPeca(idCounter++));
                }
                break;
            }
            case 3: {
                Peca usada = pop(&pilha);
                if(usada.id != -1){
                    printf("Peça usada: [%c %d]\n", usada.tipo, usada.id);
                }
                break;
            }
            case 4:
                trocarTopoFilaPilha(&fila, &pilha);
                break;
            case 5:
                trocarTres(&fila, &pilha);
                break;
            case 0:
                printf("Encerrando programa.\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while(opcao != 0);

    return 0;
}
