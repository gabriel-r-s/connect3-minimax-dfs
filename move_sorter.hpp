// esse arquivo contém uma implementação da classe move_sorter, uma classe para ordenar movimentos da melhor pontuação para a pior, ou não fazer nada caso a pontuação seja igual, mantendo a ordem original
#include "board.hpp"
#ifndef MOVE_SORTER_HPP
#define MOVE_SORTER_HPP
class move_sorter
{
    struct
    {
        uint64_t move;
        int score;
    } moves[board::width]; // lista de movimentos
    uint64_t size = 0;     // quantidade de elementos no vetor
public:
    // função para adicionar os elementos, usa um insertion sort para ordenar os elementos do menor para o maior
    void add(uint64_t move, int score)
    {
        int pos = size++;                                  // pos = valor anterior de size
        for (; pos && moves[pos - 1].score > score; pos--) // varremos e movemos os elementos até encontrarmos a posição correta para o nosso elemento
            moves[pos] = moves[pos - 1];
        moves[pos].move = move;
        moves[pos].score = score;
    }
    // retorna o próximo movimento, ou 0 se não houver nenhum
    uint64_t get()
    {
        if (size)
            return moves[--size].move; // o novo valor de size
        return 0;
    }
};
#endif