#include <iostream>
#include <omp.h>

int main() {
    int n_threads = 0;

    #pragma omp parallel
    {
        #pragma omp atomic
        n_threads++;

        int thread_id = omp_get_thread_num();
        std::cout << "Hello from thread " << thread_id << std::endl;
    }

    std::cout << "Total threads used: " << n_threads << std::endl;
    return 0;
}