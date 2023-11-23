#include "game_tree.hpp"
#include <iostream>
#include <limits>
#include <stdlib.h>
using namespace std;
int main()
{
    GameManager manager;
    while (manager.board.result() == GameResult_NotDone)
    {
        //if(manager.board.turn==0)
            manager.minimax_next(7);
        /*else
        {
            char c=cin.get();
            cin.ignore();
            c -= '1';
            manager.board.try_play(c);
        }*/
        system("cls");
        manager.board.print();
    }
    switch (manager.board.result())
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