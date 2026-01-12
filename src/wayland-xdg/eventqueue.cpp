#include "wayland-xdg/eventqueue.hpp"

 void fi_EventQueue::addEvent(fi_event event){
      qu.push(std::move(event));
      
    } 
fi_event fi_EventQueue::doEvent(){
      fi_event top= std::move(qu.front());
      
      qu.pop();
      return top;
    }