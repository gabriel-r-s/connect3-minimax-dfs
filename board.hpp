#include <vector>
#include <cstdint>
#include <climits>
#include <algorithm>
#include <cstdlib>
#include <array>

enum Result {
    Result_CpuWin,
    Result_CpuLoss,
    Result_Draw,
    Result_NotDone,
};

#define MIN false
#define MAX true
#define CPU false
#define HUMAN true
#define CPU_WIN 0b000
#define CPU_LOSS 0b111

struct TriCheck {
    uint8_t c1, c2, c3, b1, b2, b3;
};

const TriCheck CHK_ALL[] = {
    // verticais
    {0, 0, 0, 0, 1, 2}, // {col, col, col, 0, 1, 2},
    {1, 1, 1, 0, 1, 2}, // {col, col, col, 1, 2, 3}, 
    {2, 2, 2, 0, 1, 2}, 
    {3, 3, 3, 0, 1, 2}, 
                        
    {0, 0, 0, 1, 2, 3}, 
    {1, 1, 1, 1, 2, 3},
    {2, 2, 2, 1, 2, 3},
    {3, 3, 3, 1, 2, 3},

    // horizontais
    {0, 1, 2, 0, 0, 0}, // {0, 1, 2, bit, bit, bit},
    {0, 1, 2, 1, 1, 1}, // {1, 2, 3, bit, bit, bit}, 
    {0, 1, 2, 2, 2, 2}, 
    {0, 1, 2, 3, 3, 3},

    {1, 2, 3, 0, 0, 0},
    {1, 2, 3, 1, 1, 1},
    {1, 2, 3, 2, 2, 2},
    {1, 2, 3, 3, 3, 3},

    // diagonais esq-dir
    {0, 1, 2, 0, 1, 2},
    {0, 1, 2, 1, 2, 3},
    {1, 2, 3, 0, 1, 2},
    {1, 2, 3, 1, 2, 3},

    // diagonais dir-esq
    {0, 1, 2, 2, 1, 0},
    {0, 1, 2, 3, 2, 1},
    {1, 2, 3, 2, 1, 0},
    {1, 2, 3, 3, 2, 1},
};
#define CHK_ALL_LEN (sizeof(CHK_ALL) / sizeof(CHK_ALL[0]))

struct Board {
    // cada coluna, usando bit field pra economizar o maximo possivel de memoria
    uint8_t col0: 4;
    uint8_t col1: 4;
    uint8_t col2: 4;
    uint8_t col3: 4;
    // numero de elementos em cada coluna
    uint8_t len0: 3;
    uint8_t len1: 3;
    uint8_t len2: 3;
    uint8_t len3: 3;
        
    Board() {
        col0 = 0;
        col1 = 0;
        col2 = 0;
        col3 = 0;
        len0 = 0;
        len1 = 0;
        len2 = 0;
        len3 = 0;
    }

    // 'joga' neste tabuleiro na coluna e turno determinados
    bool play(uint8_t col, bool turn) {
        uint8_t cols[] = { col0, col1, col2, col3 };
        uint8_t len[] = { len0, len1, len2, len3 };

        if (len[col] == 4) {
            return false;
        }

        cols[col] |= turn << len[col];
        len[col]++;

        col0 = cols[0];
        col1 = cols[1];
        col2 = cols[2];
        col3 = cols[3];
        len0 = len[0];
        len1 = len[1];
        len2 = len[2];
        len3 = len[3];
        return true;
    }

    Result result() {
        for (size_t i = 0; i < CHK_ALL_LEN; i++) {
            uint8_t mask = get_mask(CHK_ALL[i]);
            if (mask == UINT8_MAX) {
                continue;
            }
            if (mask == CPU_WIN) {
                return Result_CpuWin;
            }
            if (mask == CPU_LOSS) {
                return Result_CpuLoss;
            }
        }
        if (len0 == 4 && len1 == 4 && len2 == 4 && len3 == 4) {
            return Result_Draw;
        }
        return Result_NotDone;
    }

    uint8_t get_mask(TriCheck chk) {
        uint8_t col[] = { col0, col1, col2, col3 };
        uint8_t len[] = { len0, len1, len2, len3 };

        if (chk.b1 >= len[chk.c1] || chk.b2 >= len[chk.c2] || chk.b3 >= len[chk.c3]) {
            return UINT8_MAX;
        }
        uint8_t i1 = (col[chk.c1] & (1 << chk.b1)) != 0;
        uint8_t i2 = (col[chk.c2] & (1 << chk.b2)) != 0;
        uint8_t i3 = (col[chk.c3] & (1 << chk.b3)) != 0;
        
        uint8_t mask = i1 | (i2 << 1) | (i3 << 2);
        return mask;
    }

};


struct Node {
    Board state;                        // estado do tabuleiro
    std::array<uint32_t, 4> parents;    // índices dos pais deste nó, UINT32_MAX = invalido
    std::array<uint32_t, 4> children;   // índices dos filhos deste nó, UINT32_MAX = invalido
    int8_t utility;                     // função de utilidade minimax
};


struct GameTree {
    std::vector<Node> nodes;        // vetor que armazena todos os nós
    std::vector<uint32_t> leaves;   // vetor que armazena os índices dos nós folha

    // cria a árvore usando `initial` como raiz
    GameTree(Board initial);

    // preenche os campos utility de acordo com o algoritmo minimax
    void create_minimax() {
        std::vector<uint32_t> queue;
        bool min_max;
        for (uint32_t i: leaves) {
            Node leaf = nodes[i];
            switch (leaf.state.result()) {
            // não é possível ganhar no turno do
            // adversário, então esse turno era nosso,
            // minimizamos o turno anterior
            case Result_CpuWin:
                leaf.utility = 1;
                min_max = MIN;
                break;
            // só é possível perder no turno do adversário,
            // o contrário se aplica
            case Result_CpuLoss:
                leaf.utility = -1;
                min_max = MAX;
                break;
            // só é possível empatar na última jogada, a 
            // do P2, então decidimos que o humano sempre
            // começa e esse turno é sempre nosso
            case Result_Draw:
                leaf.utility = 0;
                min_max = MIN;
                break;
            // estamos navegando as folhas, é impossível
            // não ter terminado
            case Result_NotDone:
                fprintf(stderr, "galho localizada no array de folhas!\n");
                exit(1);
                break;
            }
            minimax(leaf, min_max);
        }
    }

    void minimax(Node node, bool min_max) {
        for (uint32_t i: node.parents) {
            if (i == UINT32_MAX) {
                continue;
            }
            Node &parent = nodes[i];
            if (min_max == MIN) {
                parent.utility = std::min(parent.utility, node.utility);
            } else {
                parent.utility = std::max(parent.utility, node.utility);
            }
            minimax(parent, !min_max);
        }
    }

    // retorna movimento realizado para o nó filho mais favorável de acordo com campo `utility`
    uint8_t minimax_next(Node node) {
        int8_t max = INT8_MIN;
        for (uint8_t i = 0; i < node.children.size(); i++) {
            uint32_t child = node.children[i];
            if (child != UINT32_MAX) {
                int8_t utility = nodes[child].utility;
                if (utility > max) {
                    max = utility;
                }
                return i;
            }
        }
        return UINT8_MAX;
    }

    // faz busca em profundidade a partir do nó, com máximo `depth` e retorna o primeiro movimento realizado
    unsigned dfs_next(Node node);
};

