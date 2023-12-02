#include "game_manager.hpp"
void setup_done(int p1_type, int p2_type)
{
    ia_type p1_it= p1_type==4? ia_type_no_ia : p1_type==3? ia_type_random : p1_type==2? ia_type_lfs : ia_type_minimax;
    ia_type p2_it= p2_type==4? ia_type_no_ia : p2_type==3? ia_type_random : p2_type==2? ia_type_lfs : ia_type_minimax;
    player_type p1t = p1_type==4? player_type_human : player_type_ia;
    player_type p2t = p2_type==4? player_type_human : player_type_ia;
    game_manager gm(p1_it, p2_it, p1t, p2t);
    gm.start();
}
void setup_game_p2(int p1_type)
{
    cout << "escolha o tipo do jogador 2:\n1: ia minimax\n2: ia lfs (busca limitada)\n3: jogadas aleatorias\n4: controlado por humano\n";
    string command;
    std::getline(cin, command);
    if (command.size())
    {
        switch (command[0])
        {
        case '1':
        case '2':
        case '3':
        case '4':
            setup_done(p1_type, command[0] - '0');
            break;
        default:
            cout << "quer algo diferente? Pois diy\n";
            break;
        }
    }
}
void setup_game()
{
    cout << "escolha o tipo do jogador 1:\n1: ia minimax\n2: ia lfs (busca limitada)\n3: jogadas aleatorias\n4: controlado por humano\n";
    string command;
    std::getline(cin, command);
    if (command.size())
    {
        switch (command[0])
        {
        case '1':
        case '2':
        case '3':
        case '4':
            setup_game_p2(command[0] - '0');
            break;
        default:
            cout << "quer algo diferente? Pois diy\n";
            break;
        }
    }
}
int main()
{
    string command;
    do
    {
        cout << "escolha o que fazer: \n1: jogar\n2: sair";
        std::getline(cin, command);
        if (command.size())
        {
            switch (command[0])
            {
            case '1':
                setup_game();
                break;
            case '2':
                break;
            default:
                cout << "pode parar, isso nem da pra fazer\n";
                break;
            }
        }
    } while (command != "2");
}