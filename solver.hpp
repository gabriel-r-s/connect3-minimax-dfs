// esse arquivo contém a classe solver, uma classe que resolve o connect 4 usando minimax e busca em profundidade
#include "board.hpp"
#include <fstream>
#include <string>
#include <chrono>
using std::ifstream, std::ofstream, std::string, std::chrono::steady_clock, std::chrono::duration, std::chrono::time_point;
class solver
{
#define fscore(num_played) 21 - num_played / 2
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
        explored_nodes++;
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
    // contador de tabuleiros explorados
    unsigned long long explored_nodes;

public:
    // faz um teste de um dataset se baseando nos datasets fornecidos no artigo do Pascal Pons(usa o minimax)
    void test_minimax(const char *in_filename)
    {
        // abrimos o arquivo de input do dataset
        ifstream in_file(in_filename, std::ios::in);
        if (in_file)
        {
            string out_filename = string(in_filename) + ".out";
            ofstream outfile(out_filename);
            if (outfile)
            {
                outfile << "lista de jogadas\tpontos esperados\tpontos obtidos\ttabuleiros explorados\ttempo executado (segundos)\n";
                outfile.close();
                string temp; // guarda as coisas importantes do momento
                while (std::getline(in_file, temp, ' '))
                {
                    explored_nodes = 0;
                    board b;
                    if (b.play(temp.c_str()) < temp.size()) // a jogada estava errada
                    {
                        outfile.open(out_filename, std::ios::app);
                        outfile << "erro, essa jogada estava corrompida\n";
                        outfile.close();
                        continue;
                    }
                    time_point<steady_clock, duration<float>> start = steady_clock::now();
                    int score = mini_max(b, -22, 22); // são os extremos da função de avaliação.
                    float exec_time = (steady_clock::now() - start).count();
                    outfile.open(out_filename, std::ios::app);
                    outfile << temp << '\t';
                    std::getline(in_file, temp);
                    outfile << temp << '\t' << score << '\t' << explored_nodes << '\t' << exec_time << '\n';
                    outfile.close();
                }
            }
            in_file.close();
        }
    }
};