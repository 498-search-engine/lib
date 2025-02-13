#include <iostream>
#include <core/thread.h> 
#include <unistd.h>

static int he = 1;

int hi_thread(char a) {
    int times = 0;

    for (int i = 0; i < 2; ++i) {
        std::cout << "thread " << a << " outputs " << he << " for the " << i << "th time" << std::endl;
        sleep(1);
    }

    return 3;
}

int main() {
    core::thread th(hi_thread, 'a');
    th.join();

    core::thread th3(hi_thread, 'c');

    std::cout << "12345" << std::endl;

    core::thread th2(hi_thread, 'b');
    th2.join();
}