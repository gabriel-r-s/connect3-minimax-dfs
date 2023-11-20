#include <cstdint>
#include "board.hpp"

struct Node
{
    GameState board;   // struct que armazena estado do tabuleiro
    Node *children[4]={nullptr, nullptr, nullptr, nullptr}; // ponteiros dos filhos deste nó
    uint8_t utility;   // função de utilidade minimax
};

struct GameTree
{

    // cria a árvore salvando ela em um arquivo, ou carregando de um arquivo.
    GameTree(bool load = true, const char *filename);

    // preenche os campos utility de acordo com o algoritmo minimax
    void create_minimax();

    // faz busca em profundidade a partir do nó, com máximo `depth` e retorna o primeiro movimento realizado
    unsigned dfs_next(Node node, uint8_t depth=16);

    // retorna movimento realizado para o nó filho mais favorável de acordo com campo `utility`
    unsigned minimax_next(Node node, bool turn);
};
