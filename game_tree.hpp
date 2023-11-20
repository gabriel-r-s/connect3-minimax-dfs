#include <cstdint>
#include "board.hpp"

struct GameManager
{
    GameState board; // tabuleiro atual do jogo
    // initializa o tabuleiro.
    GameManager();


    // atualiza o tabuleiro usando busca em profundidade como base.
    void dfs_next(uint8_t depth=16);

    // atualiza o tabuleiro do jogo utilizando minimax.
    void minimax_next(uint8_t depth=16);
};
