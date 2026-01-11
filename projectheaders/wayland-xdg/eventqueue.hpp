#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H
#include "queue"
enum fi_eventtypes{
  FI_KEYBOARD_PRESSED,
  FI_KEYBOARD_RELEASED,
  FI_MOUSE,
};

struct fi_event{
  fi_eventtypes eventtype;
  void *Data;


};
class fi_EventQueue{
  private:
    std::queue<fi_event> qu;

  public:
    void addEvent(fi_event event){
      qu.push(event);
    }  
    fi_event doEvent(){
      fi_event top= qu.front();
      qu.pop();
      return top;
    }
      
};
#endif