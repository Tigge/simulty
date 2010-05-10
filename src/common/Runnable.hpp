#ifndef _RUNNABLE_HPP_
#define _RUNNABLE_HPP_

#include "SDL/SDL_thread.h"

class Runnable {

    private:

    SDL_Thread *thread;
    static int run(void *data);

    public:

    virtual void run() = 0;
    void start();

};

#endif

