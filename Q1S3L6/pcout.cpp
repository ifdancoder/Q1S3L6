#include "pcout.h"

pcout::pcout() {
}

pcout::~pcout()
{
    std::lock_guard<std::mutex> guard(mutexCout);
    std::cout << this->str();
}