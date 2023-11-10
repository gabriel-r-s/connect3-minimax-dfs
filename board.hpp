#include <vector>
#include <cstdint>
#include <climits>
#include <algorithm>

enum GameResult {
    GameResult_Win,
    GameResult_Loss,
    GameResult_Draw,
    GameResult_NotDone,
};

#define MIN false
#define MAX false

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
    std::array<uint32_t, 4> parents;    // índices dos pais deste nó
    std::array<uint32_t, 4> children;   // índices dos filhos deste nó
    int8_t utility;        // função de utilidade minimax
};

struct GameTree {
    std::vector<Node> nodes;        // vetor que armazena todos os nós
    std::vector<uint32_t> leaves;   // vetor que armazena os índices dos nós folha

    // cria a árvore usando `initial` como raiz
    GameTree(GameState initial);

    // preenche os campos utility de acordo com o algoritmo minimax
    void create_minimax() {
        std::vector<uint32_t> queue;
        bool min_max;
        for (Node leaf: leaves) {
            switch (leaf.state.result()) {
            // não é possível ganhar no turno do
            // adversário, então esse turno era nosso,
            // minimizamos o turno anterior
            case GameResult_Win:
                leaf.utility = 1;
                min_max = MIN;
                break;
            // só é possível perder no turno do adversário,
            // o contrário se aplica
            case GameResult_Loss:
                leaf.utility = -1;
                min_max = MAX;
                break;
            // só é possível empatar na última jogada, a 
            // do P2, então decidimos que o humano sempre
            // começa e esse turno é sempre nosso
            case GameResult_Draw:
                leaf.utility = 0;
                min_max = MIN;
                break;
            // estamos navegando as folhas, é impossível
            // não ter terminado
            case GameResult_NotDone:
                break;
            }
            minimax(leaf, min_max);
        }
    }

    void minimax(Node node, bool min_max) {
        for (parent: node.parents) {
            if (min_max == MIN) {
                parent.utility = std::min(parent.utility, node.utility);
            } else {
                parent.utility = std::max(parent.utility, node.utility);
            }
            minimax(parent, !min_max);
        }
    }

    // faz busca em profundidade a partir do nó, com máximo `depth` e retorna o primeiro movimento realizado
    unsigned dfs_next(Node node);
    
    // retorna movimento realizado para o nó filho mais favorável de acordo com campo `utility`
    uint8_t minimax_next(Node node) {
        for (uint8_t i = 0; i < node.size(), i++) {
            uint32_t child = node.children[i];
            if (child != -1 && nodes[child] == 1) {
                return i;
            }
        }
        return -1;
    }
};

