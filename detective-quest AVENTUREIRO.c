#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para representar uma sala (nó da árvore binária do mapa)
typedef struct Sala {
    char nome[50];          // Nome da sala (ex.: "Hall de Entrada")
    char pista[50];         // Pista associada à sala (pode ser vazia)
    struct Sala *esquerda;  // Ponteiro para o filho à esquerda
    struct Sala *direita;   // Ponteiro para o filho à direita
} Sala;

// Estrutura para representar um nó da árvore BST de pistas
typedef struct PistaNode {
    char pista[50];            // Conteúdo da pista
    struct PistaNode *esquerda; // Ponteiro para o filho à esquerda
    struct PistaNode *direita;  // Ponteiro para o filho à direita
} PistaNode;

// Função para criar uma sala dinamicamente
// Aloca memória e inicializa nome e pista (opcional)
Sala* criarSala(const char* nome, const char* pista) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro: Falha na alocação de memória para sala!\n");
        exit(1);
    }
    strncpy(novaSala->nome, nome, sizeof(novaSala->nome) - 1);
    novaSala->nome[sizeof(novaSala->nome) - 1] = '\0'; // Garante terminação
    if (pista != NULL) {
        strncpy(novaSala->pista, pista, sizeof(novaSala->pista) - 1);
        novaSala->pista[sizeof(novaSala->pista) - 1] = '\0';
    } else {
        novaSala->pista[0] = '\0'; // Pista vazia
    }
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

// Função para criar um nó da BST de pistas
PistaNode* criarPistaNode(const char* pista) {
    PistaNode* novoNo = (PistaNode*)malloc(sizeof(PistaNode));
    if (novoNo == NULL) {
        printf("Erro: Falha na alocação de memória para pista!\n");
        exit(1);
    }
    strncpy(novoNo->pista, pista, sizeof(novoNo->pista) - 1);
    novoNo->pista[sizeof(novoNo->pista) - 1] = '\0';
    novoNo->esquerda = NULL;
    novoNo->direita = NULL;
    return novoNo;
}

// Função para inserir uma pista na árvore BST
// Mantém pistas ordenadas alfabeticamente
PistaNode* inserirPista(PistaNode* raiz, const char* pista) {
    if (raiz == NULL) {
        return criarPistaNode(pista);
    }
    if (strcmp(pista, raiz->pista) < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (strcmp(pista, raiz->pista) > 0) {
        raiz->direita = inserirPista(raiz->direita, pista);
    }
    return raiz; // Ignora duplicatas
}

// Função para exibir as pistas em ordem alfabética (emOrdem)
void exibirPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf(" - %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

// Função para liberar a memória da árvore binária do mapa
void liberarArvoreSalas(Sala* sala) {
    if (sala == NULL) return;
    liberarArvoreSalas(sala->esquerda);
    liberarArvoreSalas(sala->direita);
    free(sala);
}

// Função para liberar a memória da árvore BST de pistas
void liberarArvorePistas(PistaNode* raiz) {
    if (raiz == NULL) return;
    liberarArvorePistas(raiz->esquerda);
    liberarArvorePistas(raiz->direita);
    free(raiz);
}

// Função para explorar as salas e coletar pistas
void explorarSalasComPistas(Sala* sala, PistaNode** pistasRaiz) {
    if (sala == NULL) {
        printf("Erro: Sala inválida!\n");
        return;
    }

    char escolha;
    Sala* atual = sala;

    // Loop de navegação até o jogador sair
    while (atual != NULL) {
        printf("\nVocê está em: %s\n", atual->nome);
        // Verifica e coleta pista, se houver
        if (atual->pista[0] != '\0') {
            printf("Pista encontrada: %s\n", atual->pista);
            *pistasRaiz = inserirPista(*pistasRaiz, atual->pista);
        } else {
            printf("Nenhuma pista neste cômodo.\n");
        }

        // Verifica se é um nó-folha (sem caminhos à esquerda ou direita)
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Este é um cômodo sem saídas.\n");
        }

        // Exibe opções disponíveis
        printf("Escolha uma direção:\n");
        if (atual->esquerda != NULL) printf("  e - Ir para a esquerda\n");
        if (atual->direita != NULL) printf("  d - Ir para a direita\n");
        printf("  s - Sair da exploração\n");
        printf("Sua escolha: ");
        scanf(" %c", &escolha);

        // Processa a escolha do jogador
        if (escolha == 's' || escolha == 'S') {
            printf("Exploração encerrada.\n");
            break;
        } else if (escolha == 'e' || escolha == 'E') {
            if (atual->esquerda != NULL) {
                atual = atual->esquerda;
            } else {
                printf("Não há caminho à esquerda! Tente novamente.\n");
            }
        } else if (escolha == 'd' || escolha == 'D') {
            if (atual->direita != NULL) {
                atual = atual->direita;
            } else {
                printf("Não há caminho à direita! Tente novamente.\n");
            }
        } else {
            printf("Opção inválida! Use 'e', 'd' ou 's'.\n");
        }
    }

    // Exibe as pistas coletadas em ordem alfabética
    printf("\nPistas coletadas (em ordem alfabética):\n");
    if (*pistasRaiz == NULL) {
        printf("Nenhuma pista foi coletada.\n");
    } else {
        exibirPistas(*pistasRaiz);
    }
}

// Função principal
int main() {
    // Criação do mapa da mansão (árvore binária)
    Sala* hall = criarSala("Hall de Entrada", NULL);
    Sala* salaEstar = criarSala("Sala de Estar", "Pegadas de lama");
    Sala* cozinha = criarSala("Cozinha", "Faca afiada");
    Sala* biblioteca = criarSala("Biblioteca", "Livro com marcação");
    Sala* jardim = criarSala("Jardim", NULL);
    Sala* quarto = criarSala("Quarto", "Bilhete escondido");
    Sala* sotao = criarSala("Sótão", NULL);

    // Montagem da estrutura da árvore
    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;
    cozinha->esquerda = quarto;
    cozinha->direita = sotao;

    // Inicializa a árvore BST de pistas
    PistaNode* pistasRaiz = NULL;

    // Início do jogo
    printf("Bem-vindo ao Detective Quest (Nível Aventureiro)!\n");
    printf("Explore a mansão e colete pistas para desvendar o mistério.\n");
    explorarSalasComPistas(hall, &pistasRaiz);

    // Liberação da memória
    liberarArvoreSalas(hall);
    liberarArvorePistas(pistasRaiz);

    return 0;
}