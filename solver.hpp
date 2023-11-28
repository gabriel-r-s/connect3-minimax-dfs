// esse arquivo contém as funções que resolvem o jogo
#include "board.hpp"
/*
Implementação do minimax.
Retorna a pontuação do tabuleiro rescebido como argumento.
usa a função:
22-num_pedras_jogadas_ganhador
para calcular a pontuação do tabuleiro
*/
int mini_max(const board &b)
{
#define fscore(num_played) 21 - num_played / 2
    if (b.get_moves() < b.width * b.height) // verifica por empate
        return 0;
    // verifica se a próxima jogada resulta na vitória do jogador atual
    for (int c = 0; c < b.width; c++)
    {
        if (b.can_play(c) && b.wins(c))
            return fscore(b.get_moves());
    }
}