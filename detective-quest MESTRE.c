#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Estrutura para representar uma sala (nó da árvore binária do mapa)
typedef struct Sala {
    char nome[50];          // Nome da sala
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

// Estrutura para representar um nó da tabela hash (encadeamento)
typedef struct HashNode {
    char pista[50];         // Chave (pista)
    char suspeito[50];      // Valor (nome do suspeito)
    char descricao[100];    // Descrição narrativa da pista
    struct HashNode *proximo; // Ponteiro para o próximo nó (em caso de colisão)
} HashNode;

// Estrutura para a tabela hash
#define TAMANHO_HASH 10
typedef struct {
    HashNode *tabela[TAMANHO_HASH]; // Array de ponteiros para listas encadeadas
} TabelaHash;

// Função para criar uma sala dinamicamente
// Aloca memória, inicializa o nome e a pista (se fornecida), e define ponteiros filhos como NULL
Sala* criarSala(const char* nome, const char* pista) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro: Falha na alocação de memória para sala!\n");
        exit(1);
    }
    strncpy(novaSala->nome, nome, sizeof(novaSala->nome));
    if (pista != NULL) {
        strncpy(novaSala->pista, pista, sizeof(novaSala->pista));
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
    strncpy(novoNo->pista, pista, sizeof(novoNo->pista));
    novoNo->esquerda = NULL;
    novoNo->direita = NULL;
    return novoNo;
}

// Função para inicializar a tabela hash
TabelaHash* criarTabelaHash() {
    TabelaHash* tabela = (TabelaHash*)malloc(sizeof(TabelaHash));
    if (tabela == NULL) {
        printf("Erro: Falha na alocação de memória para tabela hash!\n");
        exit(1);
    }
    for (int i = 0; i < TAMANHO_HASH; i++) {
        tabela->tabela[i] = NULL;
    }
    return tabela;
}

// Função de hash simples baseada na soma dos valores ASCII
unsigned int funcaoHash(const char* pista) {
    unsigned int soma = 0;
    for (int i = 0; pista[i] != '\0'; i++) {
        soma += pista[i];
    }
    return soma % TAMANHO_HASH;
}

// Função para inserir uma pista na árvore BST
// Insere a pista em ordem alfabética, ignorando duplicatas para evitar redundâncias
PistaNode* inserirPista(PistaNode* raiz, const char* pista) {
    if (raiz == NULL) {
        return criarPistaNode(pista);
    }
    if (strcmp(pista, raiz->pista) < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (strcmp(pista, raiz->pista) > 0) {
        raiz->direita = inserirPista(raiz->direita, pista);
    }
    return raiz; // Ignora pista duplicada
}

// Função para inserir uma associação pista-suspeito na tabela hash
// Usa encadeamento para lidar com colisões, inserindo no início da lista
void inserirNaHash(TabelaHash* tabela, const char* pista, const char* suspeito, const char* descricao) {
    unsigned int indice = funcaoHash(pista);
    HashNode* novoNo = (HashNode*)malloc(sizeof(HashNode));
    if (novoNo == NULL) {
        printf("Erro: Falha na alocação de memória para nó da tabela hash!\n");
        exit(1);
    }
    strncpy(novoNo->pista, pista, sizeof(novoNo->pista));
    strncpy(novoNo->suspeito, suspeito, sizeof(novoNo->suspeito));
    strncpy(novoNo->descricao, descricao, sizeof(novoNo->descricao));
    novoNo->proximo = NULL;

    // Inserção no início da lista encadeada
    novoNo->proximo = tabela->tabela[indice];
    tabela->tabela[indice] = novoNo;
}

// Função para encontrar o suspeito e descrição associados a uma pista
// Retorna um ponteiro para o nó da tabela hash ou NULL se não encontrado
HashNode* encontrarSuspeito(TabelaHash* tabela, const char* pista) {
    unsigned int indice = funcaoHash(pista);
    HashNode* atual = tabela->tabela[indice];
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual;
        }
        atual = atual->proximo;
    }
    return NULL;
}

// Função para exibir as pistas em ordem alfabética com descrições
void exibirPistas(PistaNode* raiz, TabelaHash* tabela) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda, tabela);
        HashNode* node = encontrarSuspeito(tabela, raiz->pista);
        if (node) {
            printf(" - %s (Suspeito: %s, Detalhe: %s)\n", raiz->pista, node->suspeito, node->descricao);
        } else {
            printf(" - %s (Suspeito: Desconhecido)\n", raiz->pista);
        }
        exibirPistas(raiz->direita, tabela);
    }
}

// Função para contar pistas por suspeito
void contarPistasPorSuspeito(TabelaHash* tabela, PistaNode* raiz, int* contagem, char suspeitos[][50], int* numSuspeitos) {
    if (raiz == NULL) return;
    contarPistasPorSuspeito(tabela, raiz->esquerda, contagem, suspeitos, numSuspeitos);
    HashNode* node = encontrarSuspeito(tabela, raiz->pista);
    if (node) {
        int i;
        for (i = 0; i < *numSuspeitos; i++) {
            if (strcmp(suspeitos[i], node->suspeito) == 0) {
                contagem[i]++;
                break;
            }
        }
        if (i == *numSuspeitos) {
            strcpy(suspeitos[*numSuspeitos], node->suspeito);
            contagem[*numSuspeitos] = 1;
            (*numSuspeitos)++;
        }
    }
    contarPistasPorSuspeito(tabela, raiz->direita, contagem, suspeitos, numSuspeitos);
}

// Função para verificar o suspeito acusado e determinar o resultado
// Verifica se o acusado é o Coronel e se há pelo menos duas pistas específicas
void verificarSuspeitoFinal(TabelaHash* tabela, PistaNode* pistasRaiz, const char* acusado) {
    int contagem[10] = {0}; // Suporta até 10 suspeitos
    char suspeitos[10][50];
    int numSuspeitos = 0;

    contarPistasPorSuspeito(tabela, pistasRaiz, contagem, suspeitos, &numSuspeitos);

    printf("\nResumo das pistas coletadas:\n");
    if (pistasRaiz == NULL) {
        printf("Nenhuma pista foi coletada.\n");
    } else {
        exibirPistas(pistasRaiz, tabela);
    }

    printf("\nContagem de pistas por suspeito:\n");
    int maxPistas = 0;
    char* suspeitoMaisCitado = NULL;
    for (int i = 0; i < numSuspeitos; i++) {
        printf(" - %s: %d pista(s)\n", suspeitos[i], contagem[i]);
        if (contagem[i] > maxPistas) {
            maxPistas = contagem[i];
            suspeitoMaisCitado = suspeitos[i];
        }
    }

    int pistasAcusado = 0;
    for (int i = 0; i < numSuspeitos; i++) {
        if (strcmp(suspeitos[i], acusado) == 0) {
            pistasAcusado = contagem[i];
            break;
        }
    }

    printf("\nVocê acusou: %s\n", acusado);
    if (strcmp(acusado, "Coronel") == 0 && pistasAcusado >= 2) {
        printf("Parabéns! Sua acusação está correta! O Coronel é o culpado.\n");
        printf("As pistas encontradas, como 'Pegadas de lama', 'Livro com marcação' e 'Botão de uniforme', apontam diretamente para ele.\n");
        printf("As pegadas mostram que ele esteve no local do crime, o livro revela seu motivo, e o botão confirma sua presença.\n");
    } else if (strcmp(acusado, "Coronel") == 0 && pistasAcusado < 2) {
        printf("Acusação insuficiente! Você tem apenas %d pista(s) contra o Coronel.\n", pistasAcusado);
        printf("Você precisa de pelo menos 2 pistas, como 'Pegadas de lama', 'Livro com marcação' ou 'Botão de uniforme', para confirmar sua culpa.\n");
    } else {
        printf("Acusação incorreta! %s não é o culpado.\n", acusado);
        if (pistasAcusado > 0) {
            printf("Embora você tenha %d pista(s) contra %s, elas não são suficientes para provar a culpa.\n", pistasAcusado, acusado);
        }
        printf("O verdadeiro culpado é o Coronel. Procure por pistas como 'Pegadas de lama', 'Livro com marcação' ou 'Botão de uniforme'.\n");
        if (suspeitoMaisCitado && maxPistas >= 1) {
            printf("Dica: O suspeito mais citado é %s, com %d pista(s).\n", suspeitoMaisCitado, maxPistas);
        }
    }
}

// Função para ler e validar a escolha do jogador
char lerEscolha(Sala* atual) {
    char escolha;
    while (1) {
        printf("Escolha uma direção:\n");
        if (atual->esquerda != NULL) printf("  e - Ir para a esquerda\n");
        if (atual->direita != NULL) printf("  d - Ir para a direita\n");
        printf("  s - Sair da exploração\n");
        printf("Sua escolha: ");
        if (scanf(" %c", &escolha) != 1) {
            while (getchar() != '\n');
            printf("Entrada inválida! Use 'e', 'd' ou 's'.\n");
            continue;
        }
        while (getchar() != '\n');
        escolha = tolower(escolha);
        if ((escolha == 'e' && atual->esquerda != NULL) ||
            (escolha == 'd' && atual->direita != NULL) ||
            escolha == 's') {
            return escolha;
        }
        printf("Opção inválida ou caminho inexistente! Tente novamente.\n");
    }
}

// Função para explorar as salas e coletar pistas
// Navega pela árvore binária, exibe pistas com descrições e as insere na BST
void explorarSalas(Sala* sala, PistaNode** pistasRaiz, TabelaHash* tabela) {
    if (sala == NULL) {
        printf("Erro: Mapa da mansão não inicializado!\n");
        return;
    }

    Sala* atual = sala;
    while (atual != NULL) {
        printf("\nVocê está em: %s\n", atual->nome);
        if (atual->pista[0] != '\0') {
            HashNode* node = encontrarSuspeito(tabela, atual->pista);
            if (node) {
                printf("Pista encontrada: %s\n", atual->pista);
                printf("Detalhe: %s\n", node->descricao);
                *pistasRaiz = inserirPista(*pistasRaiz, atual->pista);
            } else {
                printf("Pista encontrada: %s (sem suspeito associado)\n", atual->pista);
            }
        } else {
            printf("Nenhuma pista neste cômodo.\n");
        }

        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Este é um cômodo sem saídas. Pressione 's' para sair.\n");
            char escolha;
            do {
                scanf(" %c", &escolha);
                escolha = tolower(escolha);
                if (escolha != 's') {
                    printf("Opção inválida! Pressione 's' para sair.\n");
                }
            } while (escolha != 's');
            break;
        }

        char escolha = lerEscolha(atual);
        if (escolha == 's') {
            break;
        } else if (escolha == 'e') {
            atual = atual->esquerda;
        } else if (escolha == 'd') {
            atual = atual->direita;
        }
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

// Função para liberar a memória da tabela hash
void liberarTabelaHash(TabelaHash* tabela) {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        HashNode* atual = tabela->tabela[i];
        while (atual != NULL) {
            HashNode* temp = atual;
            atual = atual->proximo;
            free(temp);
        }
    }
    free(tabela);
}

// Função principal
int main() {
    // Criação do mapa da mansão (árvore binária)
    Sala* hall = criarSala("Hall de Entrada", NULL);
    Sala* salaEstar = criarSala("Sala de Estar", "Pegadas de lama");
    Sala* cozinha = criarSala("Cozinha", "Faca afiada");
    Sala* biblioteca = criarSala("Biblioteca", "Livro com marcação");
    Sala* jardim = criarSala("Jardim", NULL);
    Sala* quarto = criarSala("Quarto", "Botão de uniforme");
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

    // Inicializa a tabela hash
    TabelaHash* tabela = criarTabelaHash();

    // Associações pista-suspeito com descrições narrativas
    inserirNaHash(tabela, "Pegadas de lama", "Coronel", "Pegadas frescas levam ao jardim, onde o crime ocorreu.");
    inserirNaHash(tabela, "Livro com marcação", "Coronel", "Um livro com anotações sobre um plano criminoso.");
    inserirNaHash(tabela, "Botão de uniforme", "Coronel", "Um botão militar encontrado perto da cena do crime.");
    inserirNaHash(tabela, "Faca afiada", "Mordomo", "Uma faca limpa, sem sinais de uso recente.");

    // Início do jogo
    printf("Bem-vindo ao Detective Quest (Nível Mestre)!\n");
    printf("Explore a mansão, colete pistas e descubra quem é o verdadeiro culpado.\n");
    printf("Dica: Leia as descrições das pistas para entender quem está por trás do crime!\n");
    explorarSalas(hall, &pistasRaiz, tabela);

    // Fase de julgamento
    char acusado[50];
    printf("\nFase de Julgamento\n");
    printf("Digite o nome do suspeito que você acusa (ex.: Coronel, Mordomo): ");
    scanf(" %[^\n]", acusado);
    verificarSuspeitoFinal(tabela, pistasRaiz, acusado);

    // Liberação da memória
    liberarArvoreSalas(hall);
    liberarArvorePistas(pistasRaiz);
    liberarTabelaHash(tabela);

    return 0;
}