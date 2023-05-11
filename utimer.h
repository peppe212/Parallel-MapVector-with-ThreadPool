//
// Created by Giuseppe Muschetta.
//

#ifndef THREAD_POOL_ULTIMATE_UTIMER_H
#define THREAD_POOL_ULTIMATE_UTIMER_H

#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <random>
#include <cmath>
#include <vector>
#include <ctime>
#include <cstdlib>


class utimer {
    std::chrono::system_clock::time_point start;
    std::chrono::system_clock::time_point stop;
    std::string message;
    using usecs = std::chrono::microseconds;
    using msecs = std::chrono::milliseconds;

private:
    long * us_elapsed;

public:

    utimer(const std::string m) : message(m),us_elapsed((long *)nullptr) {
        start = std::chrono::system_clock::now();
    }

    utimer(const std::string m, long * us) : message(m),us_elapsed(us) {
        start = std::chrono::system_clock::now();
    }

    ~utimer() {
        stop =
                std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed =
                stop - start;
        auto musec =
                std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

        if(us_elapsed == nullptr)
            std::cout << message << " computed in " << std::setw(15) << musec << " usec "
                      << std::endl;
        if(us_elapsed != nullptr)
            (*us_elapsed) = musec;
    }
};

#endif //THREAD_POOL_ULTIMATE_UTIMER_H
