// este arquivo gera os primeiros n níveis da árvore e salva em um arquivo
#include "solver.hpp"
#include <vector>
#include <thread>
#include <iostream>
#include <queue>
#if defined __WIN32__ || defined __WIN64__
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <mutex>
#include <stdio.h>
#include <cmath>
typedef struct
{
    uint64_t key : 56; // a chave do elemento armazenado
    int8_t val = 0;    // note que o campo acima tem 56bits, e por questão de alinhamento o compilador alocou 64bits para ele, o que significa que os bits mais significativos são usados pelo valor, como nosso tabuleiro só usa os 49 bits menos significativos não à problema
} element;
const int num_threads = 32; // número de threads
const int num_level = 3;    // até qual nível da árvore calcular
using std::vector, std::thread, std::cout, std::queue, std::mutex;
vector<solver> s(num_threads);
thread **t;
queue<int> tq;
mutex tqm;
element *n;
size_t size;
void new_thread(int index)
{
    tqm.lock();
    tq.push(index);
    tqm.unlock();
}
int get_thread()
{
    int final = -1;
    while (final == -1)
    {
#if defined __WIN32__ || defined __WIN64__
        Sleep(100);
#else
        usleep(100000);
#endif
        tqm.lock();
        if (tq.size())
        {
            final = tq.front();
            tq.pop();
        }
        tqm.unlock();
    }
    return final;
}
void run_minimax(board b, element *e, int tindex)
{
    e->key = b.key();
    e->val = s[tindex].solve_minimax(b);
    t[tindex]->detach();
    delete t[tindex];
    new_thread(tindex);
}
void join_all()
{
    bool stay=true;
    while (stay)
    {
        tqm.lock();
            stay = tq.size() < num_threads;
        tqm.unlock();
#if defined __WIN32__ || defined __WIN64__
        Sleep(100);
#else
        usleep(100000);
#endif
    }
    tqm.unlock();
}
void generate_seq(string str, int index)
{
    for (int i = '1'; i < board::width + '1'; i++)
    {
        str[index] = i;
        if (index < str.size() - 1)
            generate_seq(str, index + 1);
        else
        {
            board b;
            if (b.play(str.c_str()) == str.size())
            {
                auto i = get_thread();
                t[i] = new thread(run_minimax, b, n + size++, i);
            }
        }
    }
}
int main()
{
    auto f = fopen64("game.tree", "wb");
    n = new element[int(pow(7, num_level))];
    t = new thread *[num_threads];
    for (int i = 0; i < num_threads; i++)
        new_thread(i);
    int level = 1;
    string seq = "1";
    while (level <= num_level)
    {
        printf("gerando level %d\n", level);
        generate_seq(seq, 0);
        seq += '1';
        level++;
    }
    join_all();

    for (; size--;)
        fwrite(n + size, sizeof(element), 1, f);
    fclose(f);
}