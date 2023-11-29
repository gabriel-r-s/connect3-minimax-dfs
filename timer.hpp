// classe para contar tempo
#include <chrono>
using namespace std;
class timer
{
    chrono::time_point<chrono::high_resolution_clock> start = chrono::high_resolution_clock::now();
    public:
    double elapsed()
    {
        return (chrono::high_resolution_clock::now() - start).count();
    }
};