#include <vector>
#include <cstdint>

enum GameResult {
    GameResult_Player1_Wins,
    GameResult_Player2_Wins,
    GameResult_Draw,
    GameResult_NotDone,
};

struct GameState {
    // cada coluna, usando bit field pra economizar o maximo possivel de memoria
    unsigned col1: 4;
    unsigned col2: 4;
    unsigned col3: 4;
    unsigned col4: 4;
    // numero de elementos em cada coluna
    unsigned len1: 3;
    unsigned len2: 3;
    unsigned len3: 3;
    unsigned len4: 3;

    // 'joga' neste tabuleiro na coluna e turno determinados, gerando o filho
    GameState play(unsigned col, bool turn);

    // verifica se é vitória do player1, player2
    GameResult result();
};

struct Node {
    GameState state;        // struct que armazena estado do tabuleiro
    uint32_t parents[4];    // índices dos pais deste nó
    uint32_t children[4];   // índices dos filhos deste nó
    uint8_t utility;        // função de utilidade minimax
};

struct GameTree {
    std::vector<Node> nodes;        // vetor que armazena todos os nós
    std::vector<uint32_t> leaves;   // vetor que armazena os índices dos nós folha

    // cria a árvore usando `initial` como raiz
    GameTree(GameState initial);

    // preenche os campos utility de acordo com o algoritmo minimax
    void create_minimax();

    // faz busca em profundidade a partir do nó, com máximo `depth` e retorna o primeiro movimento realizado
    unsigned dfs_next(Node node);
    
    // retorna movimento realizado para o nó filho mais favorável de acordo com campo `utility`
    unsigned minimax_next(Node node);
};

