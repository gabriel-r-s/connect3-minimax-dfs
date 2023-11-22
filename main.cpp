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
    while (board.result() == GameResult_NotDone)
    {
        print_board(board);
        char c = cin.get();
        cin.ignore();
        c -= '1';
        board = board.try_play(c);
    }
    switch (board.result())
    {
    case GameResult_Player1_Wins:
        cout << "que legal, o jogador 1 acabou de destruir o jogador 2\n";
        break;
    case GameResult_Player2_Wins:
        cout << "que legal, o jogador 2 acabou de destruir o jogador 1\n";
        break;
    default:
        cout << "isso nao deveria acontecer\n";
        break;
    }
}