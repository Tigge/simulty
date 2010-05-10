#include "Runnable.hpp"


int Runnable::run(void *data) {
    Runnable *runnable = (Runnable *)data;
    runnable->run();
    return 0; //TODO
}

void Runnable::start() {
     this->thread = SDL_CreateThread(Runnable::run, (void *)this);
}


