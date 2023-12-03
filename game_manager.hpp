// este arquivo contém uma implementação de uma classe para jogar  connect 4
#include "board.hpp"
#include "solver.hpp"
#include <stdlib.h>
#include <iostream>
#include <random>
#include <time.h>
using std::cout, std::cin, std::stoi;
typedef enum
{
    ia_type_minimax,
    ia_type_lfs,
    ia_type_random,
    ia_type_no_ia
} ia_type;
typedef enum
{
    player_type_ia,
    player_type_human
} player_type;
class game_manager
{
    typedef enum
    {
        player1_win,
        player2_win,
        game_draw,
        not_finished
    } game_state;
    ia_type player1_ia_type, player2_ia_type; // tipo da ia, minimax, lfs ou jogar aleatoriamente, ou sem ia
    player_type player1_type, player2_type;   // tipo dos jogadores
    board b;                                  // tabuleiro atual do jogo
    solver s;                                 // resolvedor do jogo
    game_state state = not_finished;          // estado atual do jogo

    // faz uma ou mais jogadas e atualiza o estado do jogo de acordo
    void try_play(string moves)
    {
        int pos = b.play(moves.c_str());
        if (pos < moves.size()) // alguma coisa aconteceu
        {
            int move = moves[pos] - '1';                               // a coluna desejada
            if (move < 0 || move >= board::width || !b.can_play(move)) // coluna inválida
            {
                cout << "coluna incorreta\nprecione enter para continuar\n";
                std::getchar();
            }
            else // é vitória, não precisa nem verificar
            {
                state = b.get_moves() % 2 ? player2_win : player1_win;
                b.play_col(move); // o tabuleiro vai ficar em um estado inválido, mas o jogo já acabou mesmo
            }
        }
        else if (b.get_moves() == board::width * board::height) // fim do jogo por empate
            state = game_draw;
    }
    // função para jogar aleatório
    void random_play()
    {
        int col = rand() % board::width;
        while (!b.can_play(col))
            col = rand() % board::width;
        try_play(std::to_string(col + 1));
    }
    // função para usar o minimax para jogar
    void minimax_play()
    {
        int col = board::width; // se todos os tabuleiros forem ruins ele jogará aleatoriamente
        int max = -((board::width * board::height + 1 - b.get_moves()) / 2);
        for (int i = 0; i < board::width; i++)
        {
            if (b.can_play(i))
            {
                if (b.wins(i))
                {
                    col = i;
                    break;
                }
                board b2 = b;
                b2.play_col(i);
                int temp = -s.solve_minimax(b2); // verificamos a pontuação desse tabuleiro, queremos minimisar, por isso negativo.
                if (temp > max)
                {
                    max = temp;
                    col = i;
                }
            }
        }
        if (col < board::width)
            try_play(std::to_string(col + 1));
        else // todas as opções são ruins
            random_play();
    }

    // função para usar a busca lfs para jogar
    void lfs_play()
    {
        int col = board::width; // se todos os tabuleiros forem ruins ele jogará aleatoriamente
        for (int i = 0; i < board::width; i++)
        {
            if (b.can_play(i))
            {
                if (b.wins(i))
                {
                    col = i;
                    break;
                }
                board b2 = b;
                b2.play_col(i);
                int temp = -s.solve_lfs(b2); // verificamos a pontuação desse tabuleiro, queremos minimisar, por isso negativo.
                if (temp == 1)               // achamos o que queríamos
                {
                    col = i;
                    break;
                }
            }
        }
        if (col < board::width)
            try_play(std::to_string(col + 1));
        else // foi ativado uma função de corte, vamos jogar aleatoriamente
            random_play();
    }

public:
    game_manager(ia_type p1_ia_type = ia_type_no_ia, ia_type p2_ia_type = ia_type_no_ia, player_type p1_type = player_type_human, player_type p2_type = player_type_human) : player1_ia_type(p1_ia_type), player2_ia_type(p2_ia_type), player1_type(p1_type), player2_type(p2_type)
    {
        srand(time(0));
    }
    // função para iniciar um jogo
    void start()
    {
        while (state == not_finished)
        {
            system("cls||clear");
            cout << b.prepare_printable();
            unsigned current_player = (b.get_moves() % 2) + 1;
            player_type pt = current_player == 1 ? player1_type : player2_type;
            ia_type it = current_player == 1 ? player1_ia_type : player2_ia_type;
            if (pt == player_type_human)
            {
                string move;
                std::getline(cin, move);
                try_play(move);
            }
            else // é ia
            {
                if (it == ia_type_minimax)
                    minimax_play();
                else if (it == ia_type_random)
                    random_play();
                else // só sobrou o lfs
                    lfs_play();
            }
        }
        system("cls||clear");
        cout << b.prepare_printable();
        if (state == player1_win)
            cout << "o jogador 1 acabou de ganhar\n";
        else if (state == player2_win)
            cout << "o jogador 2 acabou de ganhar\n";
        else
            cout << "o jogador, espere, deu empate, no final todo mundo perdeu.\n";
    }
};