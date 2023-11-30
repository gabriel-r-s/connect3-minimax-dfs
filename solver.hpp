// esse arquivo contém a classe solver, uma classe que resolve o connect 4 usando minimax e busca em profundidade
#include "board.hpp"
#include "hash_table.hpp"
class solver
{
private:
    int explore_order[board::width];
    // contador de tabuleiros explorados
    unsigned long long explored_nodes;

    // tabela para o minimax
    hash_table t;

public:
    // construtor padrão, inicialisa a matriz de exploração
    solver() : explored_nodes(0), explore_order{3, 4, 2, 5, 1, 6, 0} {}
    // reseta a tabela hash usada pelo solver e a quantidade de tabuleiros explorados
    void reset()
    {
        t.reset();
        explored_nodes = 0;
    }
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

        for (int x = 0; x < board::width; x++) // verificando se o jogador atual pode ganhar no próximo movimento
            if (b.can_play(x) && b.wins(x))
                return (board::width * board::height + 1 - b.get_moves()) / 2;

        int max = (board::width * board::height - 1 - b.get_moves()) / 2; // calculamos a pontuação máxima que podemos obter, conciderando que não podemos ganhar com uma jogada
        if (int val = t.get(b.key()))
            max = val + b.min_score - 1; // nóz sabemos que a pontuação de b é <= esse valor da tabela, então podemos definir o limite superior do beta com base nisso
        if (beta > max)
        {
            beta = max; // não faz sentido beta ser maior que a pontuação máxima
            if (alpha >= beta)
                return beta; // o intervalo de busca está vasio, não tem nada pra procurar
        }

        for (int x = 0; x < board::width; x++) // verifica as pontuações dos próximos movimentos e retorna o melhor
            if (b.can_play(explore_order[x]))
            {
                board b2(b);
                b2.play(explore_order[x]);
                int score = -mini_max(b2, -beta, -alpha); // a função é oposta, 22 para mim é -22 para o oponente
                if (score >= beta)
                    return score; // já encontramos uma pontuação boa o suficiente (não salvamos na tabela porque a pontuação pode ser maior que ela)
                if (score > alpha)
                    alpha = score; // vamos diminuir a janela de busca
            }

        t.insert(b.key(), alpha - b.min_score + 1); // ou a pontuação é essa, ou ela é menor que essa
        return alpha;
    }
};