#ifndef SMPOINTER_H
#define SMPOINTER_H
#include <includeall.hpp>
template<typename Tp,void (*clean)(Tp*)>
struct Destructor{
    void operator()(Tp* instance)const {
       if(instance) clean(instance);
    }
};
template<typename Tp,void (*clean)(Tp*)>
using fi_unique=std::unique_ptr<Tp,Destructor<Tp, clean>>;



#endif