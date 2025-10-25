#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para um nó da árvore binária (cômodo)
typedef struct No {
    char nome[50];
    struct No *esquerda;
    struct No *direita;
} No;

// Estrutura para um nó da árvore de busca (pista)
typedef struct NoPista {
    char pista[50];
    struct NoPista *esquerda;
    struct NoPista *direita;
} NoPista;

// Função para criar um novo nó de cômodo
No* criarSala(const char* nome) {
    No* novo = (No*)malloc(sizeof(No));
    strcpy(novo->nome, nome);
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

// Função para criar um novo nó de pista
NoPista* criarPista(const char* pista) {
    NoPista* novo = (NoPista*)malloc(sizeof(NoPista));
    strcpy(novo->pista, pista);
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

// Função para inserir uma pista na BST
NoPista* inserirPista(NoPista* raiz, const char* pista) {
    if (raiz == NULL) {
        return criarPista(pista);
    }
    if (strcmp(pista, raiz->pista) < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (strcmp(pista, raiz->pista) > 0) {
        raiz->direita = inserirPista(raiz->direita, pista);
    }
    return raiz;
}

// Função para listar pistas em ordem alfabética
void emOrdem(NoPista* raiz) {
    if (raiz != NULL) {
        emOrdem(raiz->esquerda);
        printf("Pista: %s\n", raiz->pista);
        emOrdem(raiz->direita);
    }
}

// Função para criar a mansão
No* criarMansao() {
    No* raiz = criarSala("Hall de Entrada");
    raiz->esquerda = criarSala("Biblioteca");
    raiz->direita = criarSala("Sala de Jantar");
    raiz->esquerda->esquerda = criarSala("Escritorio");
    raiz->esquerda->direita = criarSala("Quarto de Hóspedes");
    raiz->direita->esquerda = criarSala("Cozinha");
    raiz->direita->direita = criarSala("Salão de Baile");
    return raiz;
}

// Função para explorar a mansão e coletar pistas
void explorarSalas(No* sala, NoPista** pistas) {
    if (sala == NULL) {
        printf("Você chegou a um beco sem saída!\n");
        return;
    }

    char escolha;
    while (1) {
        printf("\nVocê está em: %s\n", sala->nome);
        // Adiciona pistas em cômodos específicos
        if (strcmp(sala->nome, "Biblioteca") == 0) {
            *pistas = inserirPista(*pistas, "Livro Rasgado");
        } else if (strcmp(sala->nome, "Cozinha") == 0) {
            *pistas = inserirPista(*pistas, "Faca Ensanguentada");
        } else if (strcmp(sala->nome, "Salão de Baile") == 0) {
            *pistas = inserirPista(*pistas, "Sapato Manchado");
        }

        printf("Pistas coletadas até agora:\n");
        emOrdem(*pistas);

        if (sala->esquerda == NULL && sala->direita == NULL) {
            printf("Este é o fim do caminho!\n");
            break;
        }
        printf("Escolha uma direção (e - esquerda, d - direita, s - sair): ");
        scanf(" %c", &escolha);

        if (escolha == 'e' && sala->esquerda != NULL) {
            explorarSalas(sala->esquerda, pistas);
        } else if (escolha == 'd' && sala->direita != NULL) {
            explorarSalas(sala->direita, pistas);
        } else if (escolha == 's') {
            printf("Exploração encerrada.\n");
            break;
        } else {
            printf("Opção inválida ou caminho bloqueado! Tente novamente.\n");
        }
    }
}

// Funções para liberar memória
void liberarArvore(No* raiz) {
    if (raiz == NULL) return;
    liberarArvore(raiz->esquerda);
    liberarArvore(raiz->direita);
    free(raiz);
}

void liberarPistas(NoPista* raiz) {
    if (raiz == NULL) return;
    liberarPistas(raiz->esquerda);
    liberarPistas(raiz->direita);
    free(raiz);
}

int main() {
    No* mansao = criarMansao();
    NoPista* pistas = NULL;
    printf("Bem-vindo à Mansão Misteriosa!\n");
    explorarSalas(mansao, &pistas);
    liberarArvore(mansao);
    liberarPistas(pistas);
    return 0;
}