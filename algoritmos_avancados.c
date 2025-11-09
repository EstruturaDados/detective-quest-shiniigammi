#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ==========================
// Definições de constantes
// ==========================
#define MAX_PISTA 50   // Tamanho máximo da string de uma pista
#define TAM_HASH 10    // Tamanho da tabela hash para armazenar suspeitos

// ==========================
// Estrutura da sala (Árvore Binária)
// ==========================
typedef struct Sala {
    char nome[50];           // Nome da sala
    struct Sala* esquerda;   // Ponteiro para a sala à esquerda
    struct Sala* direita;    // Ponteiro para a sala à direita
} Sala;

// ==========================
// Função para criar uma nova sala
// ==========================
Sala* criarSala(const char* nome) {
    Sala* nova = (Sala*)malloc(sizeof(Sala)); // Aloca memória para a sala
    strcpy(nova->nome, nome);                 // Copia o nome da sala
    nova->esquerda = NULL;                    // Inicializa ponteiro esquerda
    nova->direita = NULL;                     // Inicializa ponteiro direita
    return nova;                              // Retorna o ponteiro da nova sala
}

// ==========================
// Estrutura de pista (BST)
// ==========================
typedef struct Pista {
    char descricao[MAX_PISTA];  // Descrição da pista
    struct Pista* esquerda;     // Ponteiro para pista menor (alfabeticamente)
    struct Pista* direita;      // Ponteiro para pista maior (alfabeticamente)
} Pista;

// ==========================
// Função para inserir pista na BST
// ==========================
Pista* inserirPista(Pista* raiz, const char* descricao) {
    if (!raiz) {
        // Se a raiz for NULL, cria um novo nó
        Pista* nova = (Pista*)malloc(sizeof(Pista));
        strcpy(nova->descricao, descricao);
        nova->esquerda = NULL;
        nova->direita = NULL;
        return nova;
    }
    // Inserção recursiva: esquerda se menor, direita se maior
    if (strcmp(descricao, raiz->descricao) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, descricao);
    else if (strcmp(descricao, raiz->descricao) > 0)
        raiz->direita = inserirPista(raiz->direita, descricao);

    return raiz; // Retorna a raiz (inalterada se igual)
}

// ==========================
// Função para listar pistas em ordem alfabética (in-order traversal)
// ==========================
void listarPistas(Pista* raiz) {
    if (!raiz) return;
    listarPistas(raiz->esquerda);           // Visita a subárvore esquerda
    printf("- %s\n", raiz->descricao);      // Imprime a pista atual
    listarPistas(raiz->direita);            // Visita a subárvore direita
}

// ==========================
// Estrutura da tabela hash para associar pistas a suspeitos
// ==========================
typedef struct HashNode {
    char pista[MAX_PISTA];       // Nome da pista
    char suspeito[50];           // Suspeito associado à pista
    struct HashNode* proximo;    // Ponteiro para próximo nó (encadeamento)
} HashNode;

HashNode* tabelaHash[10] = {NULL};  // Inicializa tabela hash com NULL

// ==========================
// Função de hash simples
// ==========================
int funcaoHash(const char* pista) {
    int soma = 0;
    for (int i = 0; pista[i] != '\0'; i++)
        soma += pista[i];         // Soma os códigos ASCII de cada caractere
    return soma % 10;             // Retorna índice da tabela hash
}

// ==========================
// Inserir pista-suspeito na tabela hash
// ==========================
void inserirNaHash(const char* pista, const char* suspeito) {
    int idx = funcaoHash(pista);                // Calcula índice usando hash
    HashNode* novo = (HashNode*)malloc(sizeof(HashNode)); // Cria nó
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->proximo = tabelaHash[idx];            // Encadeia no início da lista
    tabelaHash[idx] = novo;                     // Atualiza a tabela
}

// ==========================
// Função para listar todas as pistas e suspeitos
// ==========================
void listarHash() {
    printf("\nPistas e Suspeitos:\n");
    for (int i = 0; i < 10; i++) {
        HashNode* atual = tabelaHash[i];
        while (atual) {
            printf("- %s: %s\n", atual->pista, atual->suspeito);
            atual = atual->proximo; // Passa para o próximo nó encadeado
        }
    }
}

// ==========================
// Função para encontrar o suspeito mais citado
// ==========================
void suspeitoMaisCitado() {
    int contagens[10] = {0};          // Contagem de cada suspeito
    char suspeitos[10][50];           // Lista de suspeitos
    int total = 0;

    // Percorre toda a tabela hash
    for (int i = 0; i < 10; i++) {
        HashNode* atual = tabelaHash[i];
        while (atual) {
            int achou = 0;
            for (int j = 0; j < total; j++) {
                if (strcmp(suspeitos[j], atual->suspeito) == 0) {
                    contagens[j]++; // Incrementa contador se já existe
                    achou = 1;
                    break;
                }
            }
            if (!achou) {
                strcpy(suspeitos[total], atual->suspeito); // Novo suspeito
                contagens[total] = 1;
                total++;
            }
            atual = atual->proximo;
        }
    }

    // Encontra o suspeito com mais ocorrências
    int max = 0, idx = 0;
    for (int i = 0; i < total; i++) {
        if (contagens[i] > max) {
            max = contagens[i];
            idx = i;
        }
    }

    if (total > 0)
        printf("\nSuspeito mais citado: %s (%d pistas)\n", suspeitos[idx], max);
    else
        printf("\nNenhum suspeito registrado.\n");
}

// ==========================
// Função principal de exploração das salas
// ==========================
void explorarSalas(Sala* atual, Pista** arvorePistas, Sala* anterior) {
    if (!atual) return;

    while (1) {
        printf("\nVocê está na sala: %s\n", atual->nome);

        // Adicionar pistas automaticamente dependendo da sala
        if (strcmp(atual->nome, "Biblioteca") == 0) {
            printf("Você encontrou uma pista: 'Livro misterioso'\n");
            *arvorePistas = inserirPista(*arvorePistas, "Livro misterioso");
            inserirNaHash("Livro misterioso", "Sr. Oliveira");
        }
        if (strcmp(atual->nome, "Cozinha") == 0) {
            printf("Você encontrou uma pista: 'Chave dourada'\n");
            *arvorePistas = inserirPista(*arvorePistas, "Chave dourada");
            inserirNaHash("Chave dourada", "Sra. Silva");
        }

        // Mostra as opções de movimentação
        printf("Escolha a direção:\n");
        if (atual->esquerda) printf("[e] Esquerda\n");
        if (atual->direita) printf("[d] Direita\n");
        if (anterior) printf("[v] Voltar\n"); // Só mostra voltar se existir sala anterior
        printf("[0] Sair da exploração\n");
        printf("Digite sua escolha e pressione Enter: ");

        char escolha;
        scanf(" %c", &escolha);
        getchar(); // Limpa o buffer do Enter

        // Executa ação conforme escolha
        if (escolha == 'e' && atual->esquerda) {
            explorarSalas(atual->esquerda, arvorePistas, atual);
        } else if (escolha == 'd' && atual->direita) {
            explorarSalas(atual->direita, arvorePistas, atual);
        } else if (escolha == 'v' && anterior) {
            return; // Volta para a sala anterior
        } else if (escolha == '0') {
            printf("Saindo da exploração...\n");
            return; // Retorna ao main
        } else {
            printf("Opção inválida! Tente novamente.\n");
        }
    }
}

// ==========================
// Função principal (main)
// ==========================
int main() {
    // Criar mapa da mansão
    Sala* hall = criarSala("Hall de Entrada");
    hall->esquerda = criarSala("Biblioteca");
    hall->direita = criarSala("Cozinha");
    hall->esquerda->esquerda = criarSala("Quarto");
    hall->esquerda->direita = criarSala("Sala de Jogos");
    hall->direita->esquerda = criarSala("Despensa");
    hall->direita->direita = criarSala("Jardim");

    Pista* arvorePistas = NULL; // Inicializa árvore de pistas vazia

    printf("Bem-vindo ao jogo de exploração da mansão!\n");
    explorarSalas(hall, &arvorePistas, NULL); // Começa a exploração do hall

    printf("\nPistas coletadas em ordem alfabética:\n");
    listarPistas(arvorePistas); // Mostra todas as pistas coletadas

    listarHash();               // Mostra todas as pistas com suspeitos
    suspeitoMaisCitado();       // Mostra suspeito mais citado

    return 0;
}