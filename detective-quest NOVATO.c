#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para representar uma sala (nó da árvore binária)
typedef struct Sala {
    char nome[50];          // Nome da sala (ex.: "Hall de Entrada")
    struct Sala *esquerda;  // Ponteiro para o filho à esquerda
    struct Sala *direita;   // Ponteiro para o filho à direita
} Sala;

// Função para criar uma sala dinamicamente
// Aloca memória e inicializa os campos da sala
Sala* criarSala(const char* nome) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro: Falha na alocação de memória!\n");
        exit(1);
    }
    strncpy(novaSala->nome, nome, sizeof(novaSala->nome) - 1);
    novaSala->nome[sizeof(novaSala->nome) - 1] = '\0'; // Garante terminação da string
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

// Função para liberar a memória da árvore binária
void liberarArvore(Sala* sala) {
    if (sala == NULL) return;
    liberarArvore(sala->esquerda);
    liberarArvore(sala->direita);
    free(sala);
}

// Função para explorar as salas interativamente
// Permite ao jogador navegar pela árvore binária
void explorarSalas(Sala* sala) {
    if (sala == NULL) {
        printf("Erro: Sala inválida!\n");
        return;
    }

    char escolha;
    Sala* atual = sala;

    // Loop de navegação até o jogador sair ou chegar a um nó-folha
    while (atual != NULL) {
        printf("\nVocê está em: %s\n", atual->nome);

        // Verifica se é um nó-folha (sem caminhos à esquerda ou direita)
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Este é um cômodo sem saídas. Fim do caminho!\n");
            break;
        }

        // Exibe as opções disponíveis
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
}

// Função principal
int main() {
    // Criação do mapa da mansão (árvore binária)
    Sala* hall = criarSala("Hall de Entrada");
    Sala* salaEstar = criarSala("Sala de Estar");
    Sala* cozinha = criarSala("Cozinha");
    Sala* biblioteca = criarSala("Biblioteca");
    Sala* jardim = criarSala("Jardim");
    Sala* quarto = criarSala("Quarto");
    Sala* sotao = criarSala("Sótão");

    // Montagem da estrutura da árvore
    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;
    cozinha->esquerda = quarto;
    cozinha->direita = sotao;

    // Início do jogo
    printf("Bem-vindo ao Detective Quest!\n");
    printf("Explore a mansão para encontrar pistas.\n");
    explorarSalas(hall);

    // Liberação da memória
    liberarArvore(hall);

    return 0;
}