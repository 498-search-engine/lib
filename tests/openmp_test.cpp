#include <iostream>

#if __has_include(<omp.h>)
#include <omp.h>
#else
inline int omp_get_max_threads() { return 1; }
inline int omp_get_thread_num() { return 0; }
#endif

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