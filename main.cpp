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
        int score = s.mini_max(b, -b.width * b.height / 2, b.width * b.height / 2); // são os extremos da função de avaliação.
        auto temp = t.elapsed();
        final << sec << '\t' << expected_score << '\t' << score << '\t' << s.get_nodes() << '\t' << temp << '\n';
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
            outfile << "lista de jogadas\tpontos esperados\tpontos obtidos\ttabuleiros explorados\ttempo executado (nano-segundos)\n";
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
    thread t1(test_minimax, "Test_L3_R1");
    thread t2(test_minimax, "Test_L2_R1");
    thread t3(test_minimax, "Test_L2_R2");
    thread t4(test_minimax, "Test_L1_R1");
    thread t5(test_minimax, "Test_L1_R2");
    thread t6(test_minimax, "Test_L1_R3");
    timer t;
    while (t.elapsed()<3600*1e+9)
    {
        Sleep(1000);
    }
}