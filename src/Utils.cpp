#include "Utils.h"

SimpleLoop::SimpleLoop(unsigned int flags) {
    loop = ev_loop_new(flags);
}

SimpleLoop::~SimpleLoop(){
    ev_loop_destroy(loop);
}