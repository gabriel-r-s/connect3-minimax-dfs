#include <windows.h>
#include "solver.hpp"
#include <fstream>
#include <chrono>
#include <thread>
#include <iostream>
using std::ifstream, std::ofstream, std::string, std::chrono::steady_clock, std::chrono::duration, std::chrono::time_point;
using std::thread;
// testa uma linha de um dataset
string test_line(string sec, string expected_score)
{
    string final;
    solver s;
    board b;
    if (b.play(sec.c_str()) != sec.size()) // a jogada era inválida
        final += "erro\n";
    else
    {
        time_point<steady_clock, duration<float>> start_point = steady_clock::now();
        duration<float> dur;
        int score = s.mini_max(b, -b.width * b.height / 2, b.width * b.height / 2); // são os extremos da função de avaliação.
        dur = steady_clock::now() - start_point;
        float exec_time = dur.count() / 1e-6;
        final += sec + '\t' + expected_score + '\t' + std::to_string(score) + '\t' + std::to_string(s.get_nodes()) + '\t' + std::to_string(exec_time) + '\n';
    }
    return final;
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
            outfile << "lista de jogadas\tpontos esperados\tpontos obtidos\ttabuleiros explorados\ttempo executado (micro-segundos)\n";
            outfile.close();
            string sec, expected_score;
            while (std::getline(in_file, sec, ' ') && std::getline(in_file, expected_score, '\n'))
            {
                outfile.open(out_filename, std::ios::app);
                outfile << test_line(sec, expected_score);
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
    time_point<steady_clock, duration<float>> start = steady_clock::now();
    while (1)
    {
        duration<float> dur = steady_clock::now() - start;
        if (dur.count() >= 3600)
            break;
        Sleep(1000);
    }
}