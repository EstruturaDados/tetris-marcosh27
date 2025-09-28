#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// --- Definições Globais e Estruturas de Dados ---

// Define os tamanhos máximos da fila e da pilha.
#define TAMANHO_FILA 5
#define TAMANHO_PILHA 3

typedef struct {
    char nome; // Tipo da peça ('I', 'O', 'T', 'L', 'J', 'S', 'Z')
    int id;    // Identificador numérico único da peça
} Peca;

typedef struct {
    Peca pecas[TAMANHO_FILA];
    int inicio;
    int fim;
    int contador;
} Fila;

typedef struct {
    Peca pecas[TAMANHO_PILHA];
    int topo; // -1 indica que a pilha está vazia.
} Pilha;


// --- Protótipos das Funções ---

// Funções de gerenciamento geral e da Peça
Peca gerarPeca();

// Funções de gerenciamento da Fila
void inicializarFila(Fila *f);
int filaEstaVazia(Fila *f);
void enqueue(Fila *f);
Peca dequeue(Fila *f); // Modificada para retornar a peça removida

// Funções de gerenciamento da Pilha
void inicializarPilha(Pilha *p);
int pilhaEstaCheia(Pilha *p);
int pilhaEstaVazia(Pilha *p);
void push(Pilha *p, Peca peca);
Peca pop(Pilha *p);

// Funções de interface com o jogador
void exibirEstado(Fila *f, Pilha *p);
void exibirFila(Fila *f);
void exibirPilha(Pilha *p);
void exibirMenu();


// --- Função Principal (main) ---

int main() {
    srand(time(NULL)); // Inicializa o gerador de números aleatórios

    Fila filaDePecas;
    Pilha pilhaDeReserva;
    int opcao;

    // Prepara a fila e a pilha para uso, definindo seus valores iniciais.
    inicializarFila(&filaDePecas);
    inicializarPilha(&pilhaDeReserva);

    // Preenche a fila inicial com o número definido de peças.
    printf("Inicializando o jogo Tetris Stack...\n");
    for (int i = 0; i < TAMANHO_FILA; i++) {
        enqueue(&filaDePecas);
    }
    printf("--------------------------------------------------\n\n");

    // Loop principal do jogo.
    do {
        // Mostra o estado atual completo (fila e pilha).
        exibirEstado(&filaDePecas, &pilhaDeReserva);
        exibirMenu();

        printf("Opcao: ");
        scanf("%d", &opcao);
        printf("\n");

        // Estrutura de decisão para executar a ação escolhida.
        switch (opcao) {
            case 1: // Jogar peça
                if (!filaEstaVazia(&filaDePecas)) {
                    Peca pecaJogada = dequeue(&filaDePecas);
                    printf(">> Acao: Peca [ %c %d ] foi jogada.\n", pecaJogada.nome, pecaJogada.id);
                    enqueue(&filaDePecas); // Adiciona nova peça para manter a fila cheia.
                } else {
                    printf(">> Acao falhou: A fila de pecas esta vazia!\n");
                }
                break;

            case 2: // Reservar peça
                if (pilhaEstaCheia(&pilhaDeReserva)) {
                    printf(">> Acao falhou: A pilha de reserva esta cheia!\n");
                } else if (filaEstaVazia(&filaDePecas)) {
                    printf(">> Acao falhou: Nao ha pecas na fila para reservar!\n");
                } else {
                    Peca pecaMovida = dequeue(&filaDePecas);
                    push(&pilhaDeReserva, pecaMovida);
                    printf(">> Acao: Peca [ %c %d ] movida para a reserva.\n", pecaMovida.nome, pecaMovida.id);
                    enqueue(&filaDePecas); // Adiciona nova peça para manter a fila cheia.
                }
                break;

            case 3: // Usar peça reservada
                if (pilhaEstaVazia(&pilhaDeReserva)) {
                    printf(">> Acao falhou: Nao ha pecas na reserva para usar!\n");
                } else {
                    Peca pecaUsada = pop(&pilhaDeReserva);
                    printf(">> Acao: Peca [ %c %d ] da reserva foi usada.\n", pecaUsada.nome, pecaUsada.id);
                    // A regra é que a fila é reabastecida a cada ação.
                    enqueue(&filaDePecas);
                }
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
    static int proximoId = 0;
    Peca novaPeca;
    char tiposDePeca[] = {'I', 'O', 'T', 'L', 'J', 'S', 'Z'};
    int numeroDeTipos = sizeof(tiposDePeca) / sizeof(tiposDePeca[0]);
    novaPeca.nome = tiposDePeca[rand() % numeroDeTipos];
    novaPeca.id = proximoId++;
    return novaPeca;
}

// --- Funções da Fila ---

void inicializarFila(Fila *f) {
    f->inicio = 0; f->fim = -1; f->contador = 0;
}
int filaEstaVazia(Fila *f) { return f->contador == 0; }
int filaEstaCheia(Fila *f) { return f->contador == TAMANHO_FILA; }

void enqueue(Fila *f) {
    if (filaEstaCheia(f)) return; // Se estiver cheia, não faz nada.
    f->fim = (f->fim + 1) % TAMANHO_FILA;
    f->pecas[f->fim] = gerarPeca();
    f->contador++;
}

Peca dequeue(Fila *f) {
    Peca pecaRemovida = f->pecas[f->inicio]; // Pega a peça do início.
    f->inicio = (f->inicio + 1) % TAMANHO_FILA;
    f->contador--;
    return pecaRemovida; // Retorna a peça que foi removida.
}

// --- Funções da Pilha ---

void inicializarPilha(Pilha *p) { p->topo = -1; }

int pilhaEstaCheia(Pilha *p) { return p->topo == TAMANHO_PILHA - 1; }

int pilhaEstaVazia(Pilha *p) { return p->topo == -1; }

void push(Pilha *p, Peca peca) {
    if (pilhaEstaCheia(p)) return; // Não faz nada se estiver cheia.
    p->topo++;                     // Move o ponteiro do topo para cima.
    p->pecas[p->topo] = peca;      // Adiciona a peça na nova posição do topo.
}

Peca pop(Pilha *p) {
    Peca pecaDoTopo = p->pecas[p->topo]; // Pega a peça do topo.
    p->topo--;                          // Move o ponteiro do topo para baixo.
    return pecaDoTopo;                  // Retorna a peça removida.
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
    if (filaEstaVazia(f)) {
        printf("[ Vazia ]\n");
        return;
    }
    int indice_atual = f->inicio;
    for (int i = 0; i < f->contador; i++) {
        printf("[%c %d] ", f->pecas[indice_atual].nome, f->pecas[indice_atual].id);
        indice_atual = (indice_atual + 1) % TAMANHO_FILA;
    }
    printf("\n");
}

void exibirPilha(Pilha *p) {
    printf("Pilha de reserva: ");
    if (pilhaEstaVazia(p)) {
        printf("[ Vazia ]\n");
        return;
    }
    printf("(Topo -> Base): ");
    for (int i = p->topo; i >= 0; i--) { // Itera de cima para baixo.
        printf("[%c %d] ", p->pecas[i].nome, p->pecas[i].id);
    }
    printf("\n");
}

void exibirMenu() {
    printf("Opcoes de Acao:\n");
    printf("1 - Jogar peca\n");
    printf("2 - Reservar peca\n");
    printf("3 - Usar peca reservada\n");
    printf("0 - Sair\n");
}