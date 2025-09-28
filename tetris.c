#include <stdio.h>
#include <stdlib.h>
#include <time.h> // Necessário para inicializar o gerador de números aleatórios

// --- Definições Globais e Estruturas de Dados ---

// Define o tamanho máximo da nossa fila de peças.
// Usar uma constante torna o código mais fácil de manter.
#define TAMANHO_FILA 5

typedef struct {
    char nome; // Tipo da peça ('I', 'O', 'T', 'L', 'J', 'S', 'Z')
    int id;    // Identificador numérico único da peça
} Peca;

typedef struct {
    Peca pecas[TAMANHO_FILA]; // Array que armazena os elementos da fila
    int inicio;               // Índice que aponta para o primeiro elemento da fila
    int fim;                  // Índice que aponta para o último elemento da fila
    int contador;             // Número atual de elementos na fila
} Fila;


// --- Protótipos das Funções ---

void inicializarFila(Fila *f);
int filaEstaCheia(Fila *f);
int filaEstaVazia(Fila *f);
Peca gerarPeca();
void enqueue(Fila *f);
void dequeue(Fila *f);
void exibirFila(Fila *f);
void exibirMenu();


// --- Função Principal (main) ---

int main() {
    // Inicializa o gerador de números aleatórios com uma "semente" baseada no tempo atual.
    // Isso garante que a sequência de peças geradas seja diferente a cada execução do programa.
    srand(time(NULL));

    Fila filaDePecas; // Cria a variável que irá conter nossa fila
    int opcao;        // Variável para armazenar a escolha do jogador

    // Prepara a fila para uso, definindo seus valores iniciais.
    inicializarFila(&filaDePecas);

    // Conforme os requisitos, a fila começa com um número fixo de elementos.
    printf("Inicializando o jogo e preenchendo a fila de peças...\n");
    for (int i = 0; i < TAMANHO_FILA; i++) {
        enqueue(&filaDePecas);
    }
    printf("--------------------------------------------------\n\n");


    // Loop principal do jogo. Continua executando até que o jogador escolha a opção 0 para sair.
    do {
        // Mostra o estado atual da fila para o jogador.
        exibirFila(&filaDePecas);

        // Exibe as opções de ação disponíveis.
        exibirMenu();

        // Lê a opção digitada pelo jogador.
        printf("Escolha uma acao: ");
        scanf("%d", &opcao);
        printf("\n");

        // Estrutura de decisão para executar a ação escolhida pelo jogador.
        switch (opcao) {
            case 1:
                // "Jogar Peça" - remove o elemento do início da fila.
                dequeue(&filaDePecas);
                break;
            case 2:
                // "Inserir Nova Peça" - adiciona um novo elemento ao fim da fila.
                enqueue(&filaDePecas);
                break;
            case 0:
                // "Sair" - encerra o programa.
                printf("Encerrando o Tetris Stack. Ate a proxima!\n");
                break;
            default:
                // Caso o jogador digite uma opção que não existe.
                printf("Opcao invalida! Por favor, escolha uma acao da lista.\n");
                break;
        }

        printf("--------------------------------------------------\n\n");

    } while (opcao != 0);

    return 0; // Retorna 0 para indicar que o programa terminou com sucesso.
}

// --- Implementação das Funções ---

void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = -1;
    f->contador = 0;
}

int filaEstaCheia(Fila *f) {
    return f->contador == TAMANHO_FILA;
}

int filaEstaVazia(Fila *f) {
    return f->contador == 0;
}

Peca gerarPeca() {
    static int proximoId = 0; // Variável estática para garantir IDs únicos e sequenciais.
    Peca novaPeca;

    char tiposDePeca[] = {'I', 'O', 'T', 'L', 'J', 'S', 'Z'};
    int numeroDeTipos = sizeof(tiposDePeca) / sizeof(tiposDePeca[0]);

    // Seleciona um tipo de peça aleatoriamente do array.
    novaPeca.nome = tiposDePeca[rand() % numeroDeTipos];
    novaPeca.id = proximoId; // Atribui o ID atual.

    proximoId++; // Incrementa o ID para a próxima peça a ser gerada.

    return novaPeca;
}

void enqueue(Fila *f) {
    if (filaEstaCheia(f)) {
        printf("Acao falhou: A fila de pecas futuras esta cheia!\n");
        return;
    }

    // Calcula a nova posição do 'fim' usando o operador módulo (%).
    // Isso garante que, se 'fim' estiver na última posição do array,
    // ele "dará a volta" para o início (posição 0).
    f->fim = (f->fim + 1) % TAMANHO_FILA;

    Peca nova = gerarPeca(); // Gera uma nova peça.
    f->pecas[f->fim] = nova; // Adiciona a peça na posição calculada.
    f->contador++;           // Incrementa o número de elementos na fila.

    printf("Nova peca inserida: [ %c %d ]\n", nova.nome, nova.id);
}

void dequeue(Fila *f) {
    if (filaEstaVazia(f)) {
        printf("Acao falhou: A fila de pecas esta vazia! Nao ha pecas para jogar.\n");
        return;
    }

    // Pega os dados da peça que está no início da fila para poder exibi-los.
    Peca pecaRemovida = f->pecas[f->inicio];

    // Avança o ponteiro de 'inicio' de forma circular.
    f->inicio = (f->inicio + 1) % TAMANHO_FILA;

    f->contador--; // Decrementa o número de elementos na fila.

    printf("Peca jogada: [ %c %d ]\n", pecaRemovida.nome, pecaRemovida.id);
}

void exibirFila(Fila *f) {
    printf("Fila de pecas: ");

    if (filaEstaVazia(f)) {
        printf("[ Vazia ]\n");
        return;
    }

    // Variável para percorrer a fila a partir do início.
    int indice_atual = f->inicio;

    // Loop que executa exatamente 'contador' vezes.
    for (int i = 0; i < f->contador; i++) {
        // Imprime a peça na posição atual.
        printf("[%c %d] ", f->pecas[indice_atual].nome, f->pecas[indice_atual].id);

        // Move para o próximo índice de forma circular.
        indice_atual = (indice_atual + 1) % TAMANHO_FILA;
    }
    printf("\n");
}

void exibirMenu() {
    printf("\nOpcoes de acao:\n");
    printf("1 - Jogar peca (dequeue)\n");
    printf("2 - Inserir nova peca (enqueue)\n");
    printf("0 - Sair\n");
}