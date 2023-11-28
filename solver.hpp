// esse arquivo contém a classe solver, uma classe que resolve o connect 4 usando minimax e busca em profundidade
#include "board.hpp"

class solver
{
private:
    // contador de tabuleiros explorados
    unsigned long long explored_nodes;

public:
    solver() : explored_nodes(0) {}
    // retorna o número de tabuleiros explorados
    unsigned long long get_nodes()
    {
        return explored_nodes;
    }
    /*
Implementação do minimax alpha beta.
Retorna a pontuação do tabuleiro rescebido como argumento.
usa a função:
22-num_pedras_jogadas_ganhador
para calcular a pontuação do tabuleiro
pr=pontuação real
vr= valor retornado
pr <= alpha então alpha>=vr>=pr
alpha<=pr<=beta então vr==pr
beta<=pr então beta<=vr<=pr
*/
    int mini_max(const board &b, int alpha, int beta)
    {
        explored_nodes++; // contador de tabuleiros explorados

        if (b.get_moves() == board::width * board::height) // verifica por empate
            return 0;

        for (int x = 0; x < board::width; x++) // check if current player can win next move
            if (b.can_play(x) && b.wins(x))
                return (board::width * board::height + 1 - b.get_moves()) / 2;

        int max = (board::width * board::height - 1 - b.get_moves()) / 2; // upper bound of our score as we cannot win immediately
        if (beta > max)
        {
            beta = max; // there is no need to keep beta above our max possible score.
            if (alpha >= beta)
                return beta; // prune the exploration if the [alpha;beta] window is empty.
        }

        for (int x = 0; x < board::width; x++) // compute the score of all possible next move and keep the best one
            if (b.can_play(x))
            {
                board b2(b);
                b2.play(x);
                int score = -mini_max(b2, -beta, -alpha); // explore opponent's score within [-beta;-alpha] windows:
                                                          // no need to have good precision for score better than beta (opponent's score worse than -beta)
                                                          // no need to check for score worse than alpha (opponent's score worse better than -alpha)

                if (score >= beta)
                    return score; // prune the exploration if we find a possible move better than what we were looking for.
                if (score > alpha)
                    alpha = score; // reduce the [alpha;beta] window for next exploration, as we only
                                   // need to search for a position that is better than the best so far.
            }

        return alpha;
    }
};