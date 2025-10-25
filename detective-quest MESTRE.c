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

// Estrutura para um nó da tabela hash (pista -> suspeito)
typedef struct NoHash {
    char pista[50];
    char suspeito[50];
    struct NoHash *proximo;
} NoHash;

// Estrutura para a tabela hash
typedef struct {
    NoHash **tabela;
    int tamanho;
} TabelaHash;

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

// Função para criar a tabela hash
TabelaHash* criarTabelaHash(int tamanho) {
    TabelaHash* hash = (TabelaHash*)malloc(sizeof(TabelaHash));
    hash->tamanho = tamanho;
    hash->tabela = (NoHash**)calloc(tamanho, sizeof(NoHash*));
    return hash;
}

// Função de espalhamento simples (soma ASCII % tamanho)
int hash(const char* pista, int tamanho) {
    int soma = 0;
    for (int i = 0; pista[i] != '\0'; i++) {
        soma += pista[i];
    }
    return soma % tamanho;
}

// Função para inserir na tabela hash
void inserirNaHash(TabelaHash* hash, const char* pista, const char* suspeito) {
    int indice = hash(pista, hash->tamanho);
    NoHash* novo = (NoHash*)malloc(sizeof(NoHash));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->proximo = hash->tabela[indice];
    hash->tabela[indice] = novo;
}

// Função para contar suspeitos mais citados
void suspeitoMaisCitado(TabelaHash* hash) {
    int contagem[50] = {0}; // Contador para cada suspeito
    char* suspeitos[50]; // Armazena nomes dos suspeitos
    int numSuspeitos = 0;

    for (int i = 0; i < hash->tamanho; i++) {
        NoHash* atual = hash->tabela[i];
        while (atual != NULL) {
            int encontrado = 0;
            for (int j = 0; j < numSuspeitos; j++) {
                if (strcmp(suspeitos[j], atual->suspeito) == 0) {
                    contagem[j]++;
                    encontrado = 1;
                    break;
                }
            }
            if (!encontrado) {
                suspeitos[numSuspeitos] = atual->suspeito;
                contagem[numSuspeitos]++;
                numSuspeitos++;
            }
            atual = atual->proximo;
        }
    }

    int maxContagem = 0;
    char* maisCitado = NULL;
    for (int i = 0; i < numSuspeitos; i++) {
        if (contagem[i] > maxContagem) {
            maxContagem = contagem[i];
            maisCitado = suspeitos[i];
        }
    }
    if (maisCitado) {
        printf("Suspeito mais citado: %s (%d pistas)\n", maisCitado, maxContagem);
    } else {
        printf("Nenhum suspeito encontrado.\n");
    }
}

// Função para inserir pista na BST
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
void emOrdem(NoPista* raiz, TabelaHash* hash) {
    if (raiz != NULL) {
        emOrdem(raiz->esquerda, hash);
        printf("Pista: %s (Suspeito: ", raiz->pista);
        for (int i = 0; i < hash->tamanho; i++) {
            NoHash* atual = hash->tabela[i];
            while (atual != NULL) {
                if (strcmp(atual->pista, raiz->pista) == 0) {
                    printf("%s", atual->suspeito);
                    break;
                }
                atual = atual->proximo;
            }
        }
        printf(")\n");
        emOrdem(raiz->direita, hash);
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

// Função para explorar a mansão, coletar pistas e associar suspeitos
void explorarSalas(No* sala, NoPista** pistas, TabelaHash* hash) {
    if (sala == NULL) {
        printf("Você chegou a um beco sem saída!\n");
        return;
    }

    char escolha;
    while (1) {
        printf("\nVocê está em: %s\n", sala->nome);
        // Adiciona pistas e associa suspeitos em cômodos específicos
        if (strcmp(sala->nome, "Biblioteca") == 0) {
            *pistas = inserirPista(*pistas, "Livro Rasgado");
            inserirNaHash(hash, "Livro Rasgado", "Coronel");
        } else if (strcmp(sala->nome, "Cozinha") == 0) {
            *pistas = inserirPista(*pistas, "Faca Ensanguentada");
            inserirNaHash(hash, "Faca Ensanguentada", "Mordomo");
        } else if (strcmp(sala->nome, "Salão de Baile") == 0) {
            *pistas = inserirPista(*pistas, "Sapato Manchado");
            inserirNaHash(hash, "Sapato Manchado", "Coronel");
        }

        printf("Pistas coletadas até agora:\n");
        emOrdem(*pistas, hash);
        suspeitoMaisCitado(hash);

        if (sala->esquerda == NULL && sala->direita == NULL) {
            printf("Este é o fim do caminho!\n");
            break;
        }
        printf("Escolha uma direção (e - esquerda, d - direita, s - sair): ");
        scanf(" %c", &escolha);

        if (escolha == 'e' && sala->esquerda != NULL) {
            explorarSalas(sala->esquerda, pistas, hash);
        } else if (escolha == 'd' && sala->direita != NULL) {
            explorarSalas(sala->direita, pistas, hash);
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

void liberarHash(TabelaHash* hash) {
    for (int i = 0; i < hash->tamanho; i++) {
        NoHash* atual = hash->tabela[i];
        while (atual != NULL) {
            NoHash* temp = atual;
            atual = atual->proximo;
            free(temp);
        }
    }
    free(hash->tabela);
    free(hash);
}

int main() {
    No* mansao = criarMansao();
    NoPista* pistas = NULL;
    TabelaHash* hash = criarTabelaHash(10);
    printf("Bem-vindo à Mansão Misteriosa!\n");
    explorarSalas(mansao, &pistas, hash);
    liberarArvore(mansao);
    liberarPistas(pistas);
    liberarHash(hash);
    return 0;
}