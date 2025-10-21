// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <windows.h>

// --- Definições Gerais ---
#define TAM_NOME 50
#define TAM_COR  20
#define MAX_MISSOES 5

// --- Estruturas ---
typedef struct {
    char nome[TAM_NOME];
    char cor[TAM_COR];
    int tropas;
} Territorio;

typedef struct {
    char descricao[120];
    char corObjetivo[TAM_COR];
    int territoriosNecessarios;
    int concluida;
} Missao;

// --- Protótipos de Funções ---
void limparBuffer();
void limpaTela();
void exibirMenu();
void menuBatalha();

Territorio *criarVetor(int capacidadeInicial);
Territorio *realocarVetor(Territorio *vet, int *capacidade, int necessario);
void cadastrarTerritorio(Territorio **vet, int *total, int *capacidade);
void listarTerritorios(const Territorio *vet, int total);

void gerarMissoes(Missao *missoes, int qtd, Territorio *territorios, int totalTerritorios);
void listarMissoes(const Missao *missoes, int totalMissoes);
int selecionarMissao(const Missao *missoes, int totalMissoes);

void realizarBatalhaLivre(Territorio *territorios, int totalTerritorios);
void realizarBatalhaMissao(Territorio *territorios, int totalTerritorios, Missao *missoes, int totalMissoes);
void checarMissao(Territorio *territorios, int totalTerritorios, Missao *missao);
int verificarVencedorFinal(Territorio *territorios, int totalTerritorios);

void liberarMemoria(Territorio *territorios);

// --- Função Principal ---
int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8");
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    srand((unsigned int) time(NULL));
    limpaTela();
    
    int capacidade = 2;
    int total = 0;
    Territorio *territorios = criarVetor(capacidade);

    Missao missoes[MAX_MISSOES];
    int missaoSelecionada = -1;

    int opcao;
    do {
        limpaTela();
        exibirMenu();
        printf("\nEscolha uma opção: ");
        if (scanf("%d", &opcao) != 1) {
            limparBuffer();
            opcao = -1;
        }
        limparBuffer();

        switch (opcao) {
            case 1:
                cadastrarTerritorio(&territorios, &total, &capacidade);
                break;
            case 2:
                listarTerritorios(territorios, total);
                break;
            case 3:
                menuBatalha();
                int sub;
                printf("\nEscolha uma opção: ");
                if (scanf("%d", &sub) != 1) {
                    limparBuffer();
                    sub = -1;
                }
                limparBuffer();

                if (sub == 1) {
                    realizarBatalhaLivre(territorios, total);
                } else if (sub == 2) {
                    gerarMissoes(missoes, MAX_MISSOES, territorios, total);
                    missaoSelecionada = selecionarMissao(missoes, MAX_MISSOES);
                    if (missaoSelecionada >= 0)
                        realizarBatalhaMissao(territorios, total, missoes, missaoSelecionada);
                }
                break;
            case 0:
                printf("\nEncerrando o jogo...\n");
                liberarMemoria(territorios);
                printf("\nMemória Liberada com Sucesso!!!");       
               
                break;
            default:
                printf("\nOpção inválida. Tente novamente.\n");
                Sleep(1500);
        }
        limpaTela();
    } while (opcao != 0);

    liberarMemoria(territorios);
    return 0;
}

// --- Implementações ---

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void limpaTela() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void exibirMenu() {
    printf("========================================\n");
    printf("    WAR ESTRUTURADO - DESAFIO MESTRE\n");
    printf("========================================\n");
    printf("1 - Cadastrar Território\n");
    printf("2 - Listar Territórios\n");
    printf("3 - Realizar Batalha\n");
    printf("0 - Sair\n");
    printf("========================================\n");
}

void menuBatalha() {
    printf("\n=== MENU DE BATALHA ===\n");
    printf("1 - Batalha Livre\n");
    printf("2 - Missões\n");
    printf("0 - Voltar\n");
}

// Criação e realocação de vetor dinâmico
Territorio *criarVetor(int capacidadeInicial) {
    return (Territorio *) calloc(capacidadeInicial, sizeof(Territorio));
}

Territorio *realocarVetor(Territorio *vet, int *capacidade, int necessario) {
    if (necessario <= *capacidade) return vet;
    int novaCapacidade = *capacidade * 2;
    Territorio *tmp = (Territorio *) realloc(vet, novaCapacidade * sizeof(Territorio));
    if (!tmp) {
        printf("Erro ao realocar memória.\n");
        return vet;
    }
    *capacidade = novaCapacidade;
    return tmp;
}

// Cadastro de territórios
void cadastrarTerritorio(Territorio **vet, int *total, int *capacidade) {
    if (*total >= *capacidade) {
        *vet = realocarVetor(*vet, capacidade, *total + 1);
    }

    Territorio *v = *vet;
    printf("\n--- Cadastrar Território ---\n");
    printf("Nome do território: ");
    fgets(v[*total].nome, TAM_NOME, stdin);
    v[*total].nome[strcspn(v[*total].nome, "\n")] = '\0';

    printf("Cor do exército: ");
    fgets(v[*total].cor, TAM_COR, stdin);
    v[*total].cor[strcspn(v[*total].cor, "\n")] = '\0';

    printf("Quantidade de tropas: ");
    scanf("%d", &v[*total].tropas);
    limparBuffer();

    if (v[*total].tropas < 0) v[*total].tropas = 0;

    (*total)++;
    printf("\nTerritório cadastrado com sucesso!\n");
    Sleep(1500);
}

void listarTerritorios(const Territorio *vet, int total) {
    printf("\n--- Lista de Territórios ---\n");
    if (total == 0) {
        printf("Nenhum território cadastrado.\n");
    } else {
        for (int i = 0; i < total; i++) {
            printf("[%d] %s - Cor: %s - Tropas: %d\n",
                   i, vet[i].nome, vet[i].cor, vet[i].tropas);
        }
    }
    printf("\nPressione Enter para continuar...");
    getchar();
}

// --- Missões ---
void gerarMissoes(Missao *missoes, int qtd, Territorio *territorios, int totalTerritorios) {
    const char *acoes[] = {"Conquistar", "Dominar", "Eliminar", "Controlar", "Tomar"};
    for (int i = 0; i < qtd; i++) {
        if (totalTerritorios == 0) {
            strcpy(missoes[i].corObjetivo, "Aleatória");
        } else {
            int idx = rand() % totalTerritorios;
            strcpy(missoes[i].corObjetivo, territorios[idx].cor);
        }
        int qtdTerr = (rand() % 4) + 2;
        snprintf(missoes[i].descricao, sizeof(missoes[i].descricao),
                 "%s %d territórios da cor %s",
                 acoes[rand() % 5], qtdTerr, missoes[i].corObjetivo);
        missoes[i].territoriosNecessarios = qtdTerr;
        missoes[i].concluida = 0;
    }
}

void listarMissoes(const Missao *missoes, int totalMissoes) {
    printf("\n--- Missões Disponíveis ---\n");
    for (int i = 0; i < totalMissoes; i++) {
        printf("[%d] %s\n", i, missoes[i].descricao);
    }
    printf("\n");
}

int selecionarMissao(const Missao *missoes, int totalMissoes) {
    listarMissoes(missoes, totalMissoes);
    int idx;
    printf("\nEscolha a missão: ");
    if (scanf("%d", &idx) != 1 || idx < 0 || idx >= totalMissoes) {
        limparBuffer();
        printf("\nMissão inválida.\n");
        return -1;
    }
    limparBuffer();
    printf("\nMissão selecionada: %s\n", missoes[idx].descricao);
    Sleep(1500);
    return idx;
}

// --- Batalha Livre ---
void realizarBatalhaLivre(Territorio *territorios, int total) {
    if (total < 2) {
        printf("É necessário ao menos 2 territórios cadastrados.\n");
        Sleep(1500);
        return;
    }

    listarTerritorios(territorios, total);

    int atacante, defensor;
    printf("\nEscolha o desafiante: ");
    scanf("%d", &atacante);
    limparBuffer();
    if (atacante < 0 || atacante >= total || territorios[atacante].tropas == 0) {
        printf("Inválido ou Tropa Insuficiente.\n");
        printf("\nPressione Enter para continuar...");
        getchar();
        return;
    }

    printf("Escolha o desafiado: ");
    scanf("%d", &defensor);
    limparBuffer();
    if (defensor < 0 || defensor >= total || defensor == atacante || territorios[defensor].tropas == 0) {
        printf("Inválido ou Tropa Insuficiente.\n");
        printf("\nPressione Enter para continuar...");
        getchar();
        return;
    }

    printf("\nBatalha entre %s (ATACANTE) e %s (DEFENSOR)\n",
           territorios[atacante].nome, territorios[defensor].nome);
    Sleep(1500);
    printf("\nRolando os dados...\n");
    Sleep(1500);

    int dadoA = rand() % 6 + 1;
    int dadoD = rand() % 6 + 1;

    printf("%s tirou %d\n%s tirou %d\n", territorios[atacante].nome, dadoA,
           territorios[defensor].nome, dadoD);

    if (dadoA >= dadoD) {
        territorios[atacante].tropas++;
        territorios[defensor].tropas--;
        if (territorios[defensor].tropas < 0) territorios[defensor].tropas = 0;
        printf("\nExército %s VENCEU!\n", territorios[atacante].cor);
        printf("\n%s Incorporou 01 Tropa e %s PERDEU 01 Tropa\n", territorios[atacante].nome, territorios[defensor].nome);
    } else {
        territorios[defensor].tropas++;
        territorios[atacante].tropas--;
        if (territorios[atacante].tropas < 0) territorios[atacante].tropas = 0;
        printf("\nExército %s RESISTIU!\n", territorios[defensor].cor);
        printf("\n%s Incorporou 01 Tropa e %s PERDEU 01 Tropa\n", territorios[defensor].nome, territorios[atacante].nome);
    }

    if (verificarVencedorFinal(territorios, total)) return;
    printf("\nPressione Enter para continuar...");
    getchar();
}

// --- Batalha com Missão ---
void realizarBatalhaMissao(Territorio *territorios, int total, Missao *missoes, int idx) {
    Missao *m = &missoes[idx];
    printf("\nMissão ativa: %s\n\n", m->descricao);
    realizarBatalhaLivre(territorios, total);
    checarMissao(territorios, total, m);
}

void checarMissao(Territorio *territorios, int total, Missao *m) {
    int contador = 0;
    for (int i = 0; i < total; i++) {
        if (territorios[i].tropas > 0 && strcasecmp(territorios[i].cor, m->corObjetivo) == 0)
            contador++;
    }
    if (contador >= m->territoriosNecessarios) {
        printf("\nMissão concluída! Vitória!\n");
        m->concluida = 1;
    } else {
        printf("\nMissão ainda não concluída (%d/%d).\n", contador, m->territoriosNecessarios);
    }
    printf("\nPressione Enter para continuar...");
    getchar();
}

// --- Vencedor final ---
int verificarVencedorFinal(Territorio *territorios, int total) {
    int ativos = 0;
    int maior = -1, indice = -1;
    for (int i = 0; i < total; i++) {
        if (territorios[i].tropas > 0) ativos++;
        if (territorios[i].tropas > maior) {
            maior = territorios[i].tropas;
            indice = i;
        }
    }
    if (ativos <= 1) {
        printf("\nJogo encerrado! Vencedor: %s (%s) com %d tropas.\n",
               territorios[indice].nome, territorios[indice].cor, territorios[indice].tropas);
        printf("\nPressione Enter para sair...");
        getchar();
        return 1;
    }
    return 0;
}

// --- Libera memória ---
void liberarMemoria(Territorio *territorios) {
    free(territorios);
}

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
// Funções de interface com o usuário:
// Funções de lógica principal do jogo:
// Função utilitária:

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
//int main() {
    // 1. Configuração Inicial (Setup):
    // - Define o locale para português.
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    // - Define a cor do jogador e sorteia sua missão secreta.

    // 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
    // - A cada iteração, exibe o mapa, a missão e o menu de ações.
    // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
    //   - Opção 1: Inicia a fase de ataque.
    //   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
    //   - Opção 0: Encerra o jogo.
    // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.

    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.

//    return 0;
//}

// --- Implementação das Funções ---

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.

// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.
