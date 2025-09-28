
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// --- Definições Globais e Estruturas de Dados ---

#define TAMANHO_FILA 5
#define TAMANHO_PILHA 3

typedef struct { char nome; int id; } Peca;
typedef struct { Peca pecas[TAMANHO_FILA]; int inicio, fim, contador; } Fila;
typedef struct { Peca pecas[TAMANHO_PILHA]; int topo; } Pilha;


// --- Protótipos das Funções ---

// Geração de Peça
Peca gerarPeca();

// Funções da Fila
void inicializarFila(Fila *f);
int filaEstaVazia(Fila *f);
void enqueue(Fila *f);
Peca dequeue(Fila *f);

// Funções da Pilha
void inicializarPilha(Pilha *p);
int pilhaEstaCheia(Pilha *p);
int pilhaEstaVazia(Pilha *p);
void push(Pilha *p, Peca peca);
Peca pop(Pilha *p);

// Novas Funções de Ação Estratégica
void trocarPecaAtual(Fila *f, Pilha *p);
void trocarMultiplasPecas(Fila *f, Pilha *p);

// Funções de Interface
void exibirEstado(Fila *f, Pilha *p);
void exibirFila(Fila *f);
void exibirPilha(Pilha *p);
void exibirMenu();


// --- Função Principal (main) ---

int main() {
    srand(time(NULL));

    Fila filaDePecas;
    Pilha pilhaDeReserva;
    int opcao;

    inicializarFila(&filaDePecas);
    inicializarPilha(&pilhaDeReserva);

    printf("Inicializando o jogo Tetris Stack (Nivel Avancado)...\n");
    for (int i = 0; i < TAMANHO_FILA; i++) { enqueue(&filaDePecas); }
    printf("--------------------------------------------------\n\n");

    do {
        exibirEstado(&filaDePecas, &pilhaDeReserva);
        exibirMenu();

        printf("Opcao escolhida: ");
        scanf("%d", &opcao);
        printf("\n");

        switch (opcao) {
            case 1: // Jogar peça
                if (!filaEstaVazia(&filaDePecas)) {
                    Peca pecaJogada = dequeue(&filaDePecas);
                    printf(">> Acao: Peca [ %c %d ] foi jogada.\n", pecaJogada.nome, pecaJogada.id);
                    enqueue(&filaDePecas); // Reabastece a fila.
                } else {
                    printf(">> Acao falhou: A fila de pecas esta vazia!\n");
                }
                break;

            case 2: // Reservar peça (Enviar para a pilha)
                if (pilhaEstaCheia(&pilhaDeReserva)) {
                    printf(">> Acao falhou: A pilha de reserva esta cheia!\n");
                } else if (filaEstaVazia(&filaDePecas)) {
                    printf(">> Acao falhou: Nao ha pecas na fila para reservar!\n");
                } else {
                    Peca pecaMovida = dequeue(&filaDePecas);
                    push(&pilhaDeReserva, pecaMovida);
                    printf(">> Acao: Peca [ %c %d ] movida para a reserva.\n", pecaMovida.nome, pecaMovida.id);
                    enqueue(&filaDePecas); // Reabastece a fila.
                }
                break;

            case 3: // Usar peça reservada
                if (pilhaEstaVazia(&pilhaDeReserva)) {
                    printf(">> Acao falhou: Nao ha pecas na reserva para usar!\n");
                } else {
                    Peca pecaUsada = pop(&pilhaDeReserva);
                    printf(">> Acao: Peca [ %c %d ] da reserva foi usada.\n", pecaUsada.nome, pecaUsada.id);
                    // A fila não é reabastecida aqui, pois a peça não saiu dela.
                }
                break;

            case 4: // Trocar peça atual com topo da pilha
                trocarPecaAtual(&filaDePecas, &pilhaDeReserva);
                break;

            case 5: // Troca Múltipla
                trocarMultiplasPecas(&filaDePecas, &pilhaDeReserva);
                break;

            case 0:
                printf("Encerrando o Tetris Stack. Ate a proxima!\n");
                break;

            default:
                printf("Opcao invalida! Por favor, escolha uma acao da lista.\n");
                break;
        }

        printf("--------------------------------------------------\n\n");

    } while (opcao != 0);

    return 0;
}


// --- Implementação das Funções ---

Peca gerarPeca() {
    static int proximoId = 0; Peca novaPeca;
    char tiposDePeca[] = {'I', 'O', 'T', 'L', 'J', 'S', 'Z'};
    novaPeca.nome = tiposDePeca[rand() % 7];
    novaPeca.id = proximoId++;
    return novaPeca;
}

// --- Funções da Fila ---
void inicializarFila(Fila *f) { f->inicio = 0; f->fim = -1; f->contador = 0; }
int filaEstaVazia(Fila *f) { return f->contador == 0; }
int filaEstaCheia(Fila *f) { return f->contador == TAMANHO_FILA; }
void enqueue(Fila *f) {
    if (filaEstaCheia(f)) return;
    f->fim = (f->fim + 1) % TAMANHO_FILA;
    f->pecas[f->fim] = gerarPeca();
    f->contador++;
}
Peca dequeue(Fila *f) {
    Peca pecaRemovida = f->pecas[f->inicio];
    f->inicio = (f->inicio + 1) % TAMANHO_FILA;
    f->contador--;
    return pecaRemovida;
}

// --- Funções da Pilha ---
void inicializarPilha(Pilha *p) { p->topo = -1; }
int pilhaEstaCheia(Pilha *p) { return p->topo == TAMANHO_PILHA - 1; }
int pilhaEstaVazia(Pilha *p) { return p->topo == -1; }
void push(Pilha *p, Peca peca) {
    if (pilhaEstaCheia(p)) return;
    p->topo++; p->pecas[p->topo] = peca;
}
Peca pop(Pilha *p) {
    return p->pecas[p->topo--]; // Retorna a peça e depois decrementa o topo
}

// --- Novas Funções de Ação Estratégica ---

void trocarPecaAtual(Fila *f, Pilha *p) {
    if (filaEstaVazia(f) || pilhaEstaVazia(p)) {
        printf(">> Acao falhou: E necessario ter pecas na fila e na pilha para a troca.\n");
        return;
    }

    // Usa uma variável temporária para realizar a troca
    Peca temp = p->pecas[p->topo];
    p->pecas[p->topo] = f->pecas[f->inicio];
    f->pecas[f->inicio] = temp;

    printf(">> Acao: Troca realizada entre a frente da fila e o topo da pilha.\n");
}

void trocarMultiplasPecas(Fila *f, Pilha *p) {
    // A pilha precisa estar cheia (3 peças) e a fila precisa ter no mínimo 3.
    if (f->contador < 3 || p->topo < 2) { // p->topo < 2 significa menos de 3 peças
        printf(">> Acao falhou: E necessario ter 3 pecas na fila e 3 na pilha para a troca multipla.\n");
        return;
    }

    // Loop para trocar as 3 peças
    for (int i = 0; i < 3; i++) {
        // Calcula o índice da peça na fila de forma circular.
        // Começa em 'inicio', depois 'inicio+1', etc., com a volta no array.
        int indice_fila = (f->inicio + i) % TAMANHO_FILA;

        // Calcula o índice da peça na pilha.
        // A 1ª da fila troca com o topo, a 2ª com o topo-1, etc.
        int indice_pilha = p->topo - i;

        // Realiza a troca usando uma variável temporária.
        Peca temp = p->pecas[indice_pilha];
        p->pecas[indice_pilha] = f->pecas[indice_fila];
        f->pecas[indice_fila] = temp;
    }

    printf(">> Acao: Troca realizada entre os 3 primeiros da fila e os 3 da pilha.\n");
}


// --- Funções de Interface ---

void exibirEstado(Fila *f, Pilha *p) {
    printf("Estado atual:\n");
    exibirFila(f);
    exibirPilha(p);
    printf("\n");
}
void exibirFila(Fila *f) {
    printf("Fila de pecas:    ");
    if (filaEstaVazia(f)) { printf("[ Vazia ]\n"); return; }
    int indice_atual = f->inicio;
    for (int i = 0; i < f->contador; i++) {
        printf("[%c %d] ", f->pecas[indice_atual].nome, f->pecas[indice_atual].id);
        indice_atual = (indice_atual + 1) % TAMANHO_FILA;
    }
    printf("\n");
}
void exibirPilha(Pilha *p) {
    printf("Pilha de reserva: ");
    if (pilhaEstaVazia(p)) { printf("[ Vazia ]\n"); return; }
    printf("(Topo -> base): ");
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->pecas[i].nome, p->pecas[i].id);
    }
    printf("\n");
}
void exibirMenu() {
    printf("Opcoes disponiveis:\n");
    printf("1 - Jogar peca da frente da fila\n");
    printf("2 - Enviar peca da fila para a pilha de reserva\n");
    printf("3 - Usar peca da pilha de reserva\n");
    printf("4 - Trocar peca da frente da fila com o topo da pilha\n");
    printf("5 - Trocar os 3 primeiros da fila com as 3 pecas da pilha\n");
    printf("0 - Sair\n");
}