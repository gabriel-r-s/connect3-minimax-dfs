#include <windows.h>
#include "solver.hpp"
#include <fstream>
#include <sstream>
#include "timer.hpp"
#include <thread>
#include <iostream>
using std::ifstream, std::ofstream, std::string, std::stringstream;
using std::thread;
// testa uma linha de um dataset
string test_line(string sec, string expected_score, solver &s)
{
    stringstream final;
    board b;
    if (b.play(sec.c_str()) != sec.size()) // a jogada era inválida
        final << "erro\n";
    else
    {
        timer t;
        int score = s.solve_lfs(b);
        auto temp = t.elapsed();
        final << sec << '\t' << expected_score << '\t' << score << '\t' << s.get_nodes() << '\t' << temp << '\t' << s.get_hits() << '\t' << s.get_misses() << '\n';
    }
    return final.str();
}
// testa um dataset
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
            outfile << "lista de jogadas\tpontos esperados\tpontos obtidos\ttabuleiros explorados\ttempo executado (nano-segundos)\thash_table_hit\thash_table_miss\n";
            outfile.close();
            string sec, expected_score;
            solver s;
            while (std::getline(in_file, sec, ' ') && std::getline(in_file, expected_score, '\n'))
            {
                outfile.open(out_filename, std::ios::app);
                outfile << test_line(sec, expected_score, s);
                s.reset();
                outfile.close();
            }
        }
        in_file.close();
    }
}
int main()
{
    solver s;
    while(1)
    {
        string str;
        std::getline(cin, str);
        cout << test_line(str, "", s);
    }
}