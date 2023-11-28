// esse arquivo contém a classe solver, uma classe que resolve o connect 4 usando minimax e busca em profundidade
#include "board.hpp"
class solver
{
private:
    // retorna a melhor pontuação dos tabuleiros
    int maximise(const board &b, int alpha, int beta)
    {
        for (int c = 0; c < b.width; c++) // calcula a pontuação dos tabuleiros atingíveis, e retorna o melhor
            if (b.can_play(c))
            {
                board b2 = b;
                b2.play(c);
                // a nossa função é cimétrica, 10 pontos é = -10 pontos para o jogador oponente
                int score = -mini_max(b2, -beta, -alpha);
                if (score >= beta)
                    return score; // encontramos um tabuleiro bom o suficiente
                if (score > alpha)
                    alpha = score; // atualisando o intervalo de busca
            }
        return alpha;
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
#define fscore(num_played) 21 - num_played / 2
        if (b.get_moves() < b.width * b.height) // verifica por empate
            return 0;
        // verifica se a próxima jogada resulta na vitória do jogador atual
        for (int c = 0; c < b.width; c++)
            if (b.can_play(c) && b.wins(c))
                return fscore(b.get_moves());
        // não é empate, não é vitória, bora calcular recursivo
        int max = fscore((b.get_moves() + 2)); // a pontuação máxima que o jogador atual pode atingir.
        if (beta > max)
            beta = max; // não faz sentido beta ser maior que a pontuação máxima
        if (alpha >= beta)
            return beta; // se o intervalo de busca está vasio não tem nada a ser buscado
        return maximise(b, alpha, beta);
    }
};