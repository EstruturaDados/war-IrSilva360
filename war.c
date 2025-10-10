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
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <windows.h>

struct Territorio { // Estrutura dos territorios
    char nome[30];
    char cor[10];
    int tropas;
};

void limparBufferEntrada() { // Função para limpar o Buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void exibirMapa(struct Territorio territorios[], int qtd) { //Função para exibir o mapa
    printf("\n============================================\n");
    printf("         MAPA DO MUNDO - ESTADO ATUAL\n");
    printf("============================================\n");
    for (int i = 0; i < qtd; i++) {
        printf("\n%d. %s (Exército %s, Tropas: %d)", 
               i + 1, territorios[i].nome, territorios[i].cor, territorios[i].tropas);
    }
    
}

void batalhar(struct Territorio *atacante, struct Territorio *defensor) { // Estrutura da Batalha
    int dadoAtaque = rand() % 6 + 1; // Simulação dos dados
    int dadoDefesa = rand() % 6 + 1;

    printf("\n%s (Atacante) vs %s (Defensor)\n", atacante->nome, defensor->nome);
    printf("\nE rolam os dados!!!\n");
    Sleep(2000); // Função para simular a ação de jogar os dados
    printf("\nDado do Atacante: %d\nDado do Defensor: %d\n", dadoAtaque, dadoDefesa); // mostrando os resultados

    if (dadoAtaque > dadoDefesa) { // Logica do jogo
        defensor->tropas--;
        printf("\nExército %s venceu! %s perdeu 1 tropa.\n", atacante->cor, defensor->nome);

        if (defensor->tropas <= 0) {
            defensor->tropas = 0;
            printf("\nExército %s conquistou o território %s!\n", atacante->cor, defensor->nome);
                   
        }
      
    } else {
        defensor->tropas--;
        printf("\nExército %s resistiu! %s perdeu 1 tropa.\n", defensor->cor, defensor->nome);

        if (defensor->tropas <= 0) {
            defensor->tropas = 0;
            printf("\nExército %s conquistou o território %s!\n", atacante->cor, defensor->nome);
        
        }
    }  
    
}

void cadastrarTerritorio(struct Territorio *t) { 
    printf("\nNome do Território: ");
    fgets(t->nome, 30, stdin);
    t->nome[strcspn(t->nome, "\n")] = '\0';

    printf("Cor do Exército: ");
    fgets(t->cor, 10, stdin);
    t->cor[strcspn(t->cor, "\n")] = '\0';

    do {
        printf("Quantidade de Tropas: ");
        scanf("%d", &t->tropas);
        limparBufferEntrada();
        if (t->tropas <= 0)
            printf("A quantidade deve ser positiva!\n");
    } while (t->tropas <= 0);
}

struct Territorio* menuCadastro(int *qtd) {
    struct Territorio *territorios = NULL;
    int opcao = 0;

#ifdef _WIN32 // Limpa a tela quando executado direto no terminal
    system("cls");
#else
    system("clear");
#endif
        // Menu do inicio do Jogo
        printf("\n=========================================================\n");
        printf("            WAR ESTRUTURADO - CADASTRO INICIAL\n");
        printf("=========================================================\n\n");

    do {
        
        printf("\n1. Cadastrar novo território\n");
        printf("2. Ir para o jogo\n");
        printf("0. Sair do Jogo\n");
        printf("\nEscolha uma opção: ");
        scanf("%d", &opcao);
        limparBufferEntrada();
        exibirMapa(territorios, *qtd);
           
        switch (opcao) { // Loop de cadastro
            case 1:
                (*qtd)++;
                territorios =realloc(territorios, (*qtd) * sizeof(struct Territorio)); // Solicitação de memoria os sistema
                if (!territorios) {
                    printf("Erro ao alocar memória!\n");
                    exit(1);
                }
                printf("\n\n--- Cadastro do Território %d ---\n", *qtd);
                cadastrarTerritorio(&territorios[*qtd - 1]);
                break;
            
            case 2:
                printf("\n\nIniciando o jogo...\n");
                break;

            case 0:
                printf("\n\n--- Jogo Encerrado ---\n");
                printf("\n\nPressione Enter para sair...");
                getchar();
                break;

            default:
                printf("Opção inválida!\n");
        }

    } while (opcao != 2 && opcao != 0); // Não permite sair sem cadastrar pelo menos 2

    return territorios;
}

void menuAtaque(struct Territorio *territorios, int qtd) { // Menu de Ataque
    int opcao, atacante, defensor;
    do {
       
        printf("\n1. ATACAR\n");
        printf("0. Voltar a Fase de Ataque\n");
        printf("\nEscolha uma opção: ");
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch (opcao) { // Logica do ataque
            
            case 1:
            
                exibirMapa(territorios, qtd);
                printf("\n\nEscolha o território atacante (1 a %d): ", qtd);
                scanf("%d", &atacante);
                printf("\nEscolha o território defensor (1 a %d): ", qtd);
                scanf("%d", &defensor);
                limparBufferEntrada();

                if (atacante < 1 || atacante > qtd ||
                    defensor < 1 || defensor > qtd ||
                    atacante == defensor) {
                    printf("\nEscolha inválida!\n");
                } else if (territorios[atacante - 1].tropas <= 1) {
                    printf("\n%s não possui tropas suficientes para atacar!\n",
                           territorios[atacante - 1].nome);
                } else {
                    batalhar(&territorios[atacante - 1], &territorios[defensor - 1]);
                }
                break;

            case 0:
                printf("\nVoltando a Fase de Ataque...\n");
                break;

            default:
                printf("\nOpção inválida!\n");
        }

    } while (opcao != 0);
}

int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8");
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    srand(time(NULL));

    int qtdTerritorios = 0;
    struct Territorio *territorios = menuCadastro(&qtdTerritorios);

    int opcaoPrincipal;


    do {
        // Menu do ataque
        printf("\n=========================================================\n");
        printf("                      --- BATALHA ---\n");
        printf("=========================================================\n\n");
        printf("\n1. Realizar Ataque\n");
        printf("0. Desistir da Batalha\n");
        printf("\nEscolha uma opção: ");
        scanf("%d", &opcaoPrincipal);
        limparBufferEntrada();

        switch (opcaoPrincipal) {
            case 1:
                menuAtaque(territorios, qtdTerritorios);
                break;

            case 0:
                
                break;

            default:
                printf("\nOpção inválida!\n");
        }

    } while (opcaoPrincipal != 0);

    free(territorios); // Liberação da Memoria
    printf("\nMemória Liberada com Sucesso!\n");
    printf("\n\nPressione Enter para Sair");
    getchar();

    return 0;
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
