#include "board.hpp"
#include <iostream>
#include <stdlib.h>
using namespace std;
void print_board(const GameState &board)
{
    for (unsigned char i = 5; i <= 5; i--) // geramos um underflow.
    {
        for (unsigned j = 0; j < 7; j++)
            cout << (i >= board.len[j] ? "*" : board.cols[j] & (1 << i) ? "2"
                                                                        : "1");
        cout << '\n';
    }
}
int main()
{
    GameState board;
    while (true)
    {
        print_board(board);
        cout << "Linhas do jogador 1: " << board.get_v_lines(4, 0) << "\nLinhas do jogador 2: " << board.get_v_lines(4, 1) << '\n';
        char c = cin.get();
        cin.ignore();
        c -= '1';
        board = board.try_play(c);
    }
}