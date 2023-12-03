// esse arquivo contém a classe solver, uma classe que resolve o connect 4 usando minimax e busca em profundidade
#include "board.hpp"
#include "hash_table.hpp"
#include <unordered_map>
#include "move_sorter.hpp"
#include <stdio.h>
using std::unordered_map;
#ifndef SOLVER_HPP
#define SOLVER_HPP
typedef struct
{
    uint64_t key : 56;
    int8_t val;
} element;

class solver
{
private:
    // tabuleiros iniciais
    unordered_map<uint64_t, int8_t> opening;
    // tabela para o minimax
    hash_table t;
    int explore_order[board::width];
    // contador de tabuleiros explorados
    unsigned long long explored_nodes;
    /*
    implementação do busca em profundidade limitada.
    retorna depth+1  se for pocível ganhar apartir do nó e ele for um nó seu, ou 0 caso contrário
    */
    int lfs(int depth, const board &b, bool oponent = false)
    {
        if (b.canWinNext() && !oponent)
            return depth + 1;
        if (!depth)
            return 0;
        int64_t final = 0;
        for (int i = 0; i < board::width; i++)
        {
            if (b.can_play(i) && !b.wins(i))
            {
                board b2 = b;
                b2.play_col(i);
                int temp = lfs(depth - 1, b2, !oponent);
                if (temp > final) // achamos um caminho mais curto
                    final = temp;
            }
        }
        return final;
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
        explored_nodes++;                           // contador de tabuleiros explorados
        uint64_t next = b.possibleNonLosingMoves(); // bitfield contendo 1 para cada coluna que não faz o oponente ganhar no próximo turno
        if (!next)
            return -(board::width * board::height - b.get_moves()) / 2; // você perdeu
        if (b.get_moves() == board::width * board::height)              // verifica por empate
            return 0;

        int min = -(board::width * board::height - 2 - b.get_moves()) / 2; // limite mínimo da pontuação, já que o oponente não pode ganhar no próximo movimento
        if (alpha < min)
        {
            alpha = min; // não faz sentido alpha ser menor que a pontuação mínima
            if (alpha >= beta)
                return alpha; // o intervalo de busca está vasio, não tem nada pra procurar
        }

        int max = (board::width * board::height - 1 - b.get_moves()) / 2; // calculamos a pontuação máxima que podemos obter, conciderando que não podemos ganhar com uma jogada
        if (int val = t.get(b.key()))
            max = val + b.min_score - 1; // sabemos que a pontuação de b é <= esse valor da tabela, então podemos definir o limite superior com base nisso
        if (beta > max)
        {
            beta = max; // não faz sentido beta ser maior que a pontuação máxima
            if (alpha >= beta)
                return beta; // o intervalo de busca está vasio, não tem nada pra procurar
        }
        move_sorter my_moves;
        for (int x = board::width; x--;) // ordena os movimentos em uma ordem do melhor para o pior usando a nossa função de pontuação como base (o nosso ordenador funciona melhor se inserirmos do pior para o melhor)
            if (uint64_t move = next & board::column_mask(explore_order[x]))
                my_moves.add(move, b.score(move));
        while (uint64_t move = my_moves.get()) // verificando o melhor movimento
        {
            board b2(b);
            b2.play(move);
            int score = -mini_max(b2, -beta, -alpha); // a função é oposta, 22 para mim é -22 para o oponente
            if (score >= beta)
                return score; // já encontramos uma pontuação boa o suficiente (não salvamos na tabela porque a pontuação pode ser maior que ela)
            if (score > alpha)
                alpha = score; // vamos diminuir a janela de busca
        }

        t.insert(b.key(), alpha - b.min_score + 1); // ou a pontuação é essa, ou ela é menor que essa
        return alpha;
    }

public:
    // construtor padrão, inicialisa a matriz de exploração e carrega o arquivo com os tabuleiros iniciais
    solver() : explored_nodes(0), explore_order{3, 4, 2, 5, 1, 6, 0}
    {
        auto f = fopen64("game.tree", "rb");
        if (f)
        {
            element e;
            while (fread(&e, sizeof(element), 1, f))
                opening[e.key] = e.val;
            fclose(f);
        }
    }
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
    resolve connect 4 usando minimax
    retorna a pontuação do tabuleiro recebido, maior é melhor para o jogador atual.
    weak, um resolvedor fraco vai fazer uma busca no intervalo -1, 1: o que retornará a primeira pontuação de vitória encontrada, ou <1 caso não seja pocível ganhar a partir do nó expecífico
    */
    int solve_minimax(board &b, bool weak = false)
    {
        if (b.canWinNext()) // para diminuir a quantidade de nós explorados o nosso minimax não busca por nós vitoriósos no próximo turno, vamos fazer isso por ele aqui.
            return (board::width * board::height + 1 - b.get_moves()) / 2;
        if (opening.find(b.key()) != opening.end()) // o tabuleiro está no livro de abertura
            return opening[b.key()];
        int alpha = weak ? -1 : -b.width * b.height / 2;
        int beta = weak ? 1 : b.width * b.height / 2;
        return mini_max(b, alpha, beta);
    }
    // resolve connect 4 usando lfs
    int solve_lfs(board b, int depth = 8)
    {
        return lfs(depth, b);
    }
    // retorna a quantidade de hits da tabela
    uint64_t get_hits()
    {
        return t.hit;
    }
    // retorna a quantidade de misses da tabela
    uint64_t get_misses()
    {
        return t.miss;
    }
};
#endif