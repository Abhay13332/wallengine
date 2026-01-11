#ifndef signals_setup_h
#define signals_setup_h

#include "includeall.hpp"
volatile sig_atomic_t  exitflag_sig=0;
void sigkill_handler(int sig){
if(sig==SIGINT || sig == SIGTERM){

   exitflag_sig=1;
   
}
}
void signals_bind(){
    signal(SIGINT,sigkill_handler);
    signal(SIGTERM,sigkill_handler);

}
#endif